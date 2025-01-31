#include <assert.h>
#include <cifuzz/cifuzz.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <memory>
#include "slog_cc/buffer/buffer.h"
#include "slog_cc/context/context.h"
#include "slog_cc/events/event.h"
#include "slog_cc/primitives/call_site.h"
#include "slog_cc/primitives/record.h"
#include "slog_cc/printer/printer.h"

FUZZ_TEST_SETUP() {
  // One-time initialization tasks
}

FUZZ_TEST(const uint8_t *data, size_t size) {
  FuzzedDataProvider fdp(data, size);

  // Create a shared SlogContext instance
  auto context = slog::SlogContext::getInstance();

  // Create a SlogBuffer with the context
  slog::SlogBuffer buffer(context);

  // Generate random function, file, and line for addOrReuseCallSiteVerySlow
  std::string function = fdp.ConsumeRandomLengthString(100);
  std::string file = fdp.ConsumeRandomLengthString(100);
  int line = fdp.ConsumeIntegral<int>();

  // Call addOrReuseCallSiteVerySlow
  context->addOrReuseCallSiteVerySlow(function, file, line);

  // Create a SlogRecord and convert it to JSON
  int32_t a = fdp.ConsumeIntegral<int32_t>();
  int32_t b = fdp.ConsumeIntegral<int32_t>();
  int8_t c = fdp.ConsumeIntegral<int8_t>();
  slog::SlogRecord record(a, b, c);
  slog::SlogPrinter printer;
  printer.jsonString(record);
}
