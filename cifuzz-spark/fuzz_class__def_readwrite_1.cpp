#include <assert.h>
#include <cifuzz/cifuzz.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "slog_cc/primitives/call_site.h"
#include "slog_cc/primitives/record.h"
#include "slog_cc/printer/printer.h"

FUZZ_TEST_SETUP() {
  // No specific setup required for this fuzz test.
}

FUZZ_TEST(const uint8_t *data, size_t size) {
  FuzzedDataProvider fdp(data, size);

  // Fuzz the SlogCallSite and SlogRecord classes.
  std::string function = fdp.ConsumeRandomLengthString(100);
  std::string file = fdp.ConsumeRandomLengthString(100);
  int32_t line = fdp.ConsumeIntegral<int32_t>();
  slog::SlogCallSite callSite(function, file, line);

  int32_t timestamp = fdp.ConsumeIntegral<int32_t>();
  int32_t duration = fdp.ConsumeIntegral<int32_t>();
  int8_t severity = fdp.ConsumeIntegral<int8_t>();
  slog::SlogRecord record(timestamp, duration, severity);

  slog::SlogPrinter printer;
  std::string json = printer.jsonString(record);
}
