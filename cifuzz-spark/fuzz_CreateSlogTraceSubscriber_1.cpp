#include <cifuzz/cifuzz.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "slog_cc/analysis_tools/tracing/slog_trace_subscriber.h"
#include "slog_cc/context/context.h"
#include "slog_cc/slog.h"
#include <unistd.h> // Include for usleep

// Inline the fib function from the example
int fib(int n) {
  SLOG_SCOPE("fib-" + std::to_string(n));
  int result = n <= 1 ? 1 : fib(n - 1) + fib(n - 2);
  SLOG(INFO) << "fib(" << n << ") = " << result;
  return result;
}

FUZZ_TEST_SETUP() {
  // No setup needed for this fuzz test
}

FUZZ_TEST(const uint8_t *data, size_t size) {
  FuzzedDataProvider fdp(data, size);

  // Use FuzzedDataProvider to generate a random file path
  std::string tempFilePath = "/tmp/slog-trace-" + std::to_string(fdp.ConsumeIntegral<uint32_t>()) + ".json";
  std::string threadName = fdp.ConsumeRandomLengthString(10);
  int n = fdp.ConsumeIntegralInRange<int>(0, 10);

  auto slog_trace_subscriber = slog::CreateSlogTraceSubscriber(
      tempFilePath, slog::SlogTraceConfig::kTrackScopesAndLogs);

  SLOG(INFO).addTag(slog::kSlogTagTraceThreadName, threadName.c_str());

  SLOG_SCOPE("Whole test scope");
  int kNumIterations = fdp.ConsumeIntegralInRange<int>(1, 10); // Remove constexpr
  for (int i = 0; i < kNumIterations; ++i) {
    SLOG_SCOPE("iteration-" + std::to_string(i) + "-of-" +
               std::to_string(kNumIterations));
    const int sleep_us = fdp.ConsumeIntegralInRange<int>(100, 1000);
    SLOG(INFO) << "iteration #" << i << "\n  sleeping "
               << slog::SlogTag("sleep_us", sleep_us) << " microseconds...";
    SLOG_SCOPE("Sleep scope").addTag("sleep_us", sleep_us);
    usleep(sleep_us);
  }

  fib(n);

  slog::SlogContext::getInstance()->waitAsyncSubscribers();
}
