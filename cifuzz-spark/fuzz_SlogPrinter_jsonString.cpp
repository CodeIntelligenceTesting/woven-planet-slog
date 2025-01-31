#include <assert.h>
#include <cifuzz/cifuzz.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "slog_cc/printer/printer.h"
#include "slog_cc/primitives/record.h"

FUZZ_TEST_SETUP() {
  // No one-time setup is necessary for this harness.
}

FUZZ_TEST(const uint8_t *data, size_t size) {
  FuzzedDataProvider fdp(data, size);

  int32_t thread_id = fdp.ConsumeIntegral<int32_t>();
  int32_t call_site_id = fdp.ConsumeIntegral<int32_t>();
  int8_t severity = fdp.ConsumeIntegral<int8_t>();

  slog::SlogRecord slog_record(thread_id, call_site_id, severity);

  // Adding tags with different types
  slog_record.addTag(fdp.ConsumeRandomLengthString().c_str());
  slog_record.addTag(fdp.ConsumeRandomLengthString().c_str(), fdp.ConsumeIntegral<int>());
  slog_record.addTag(fdp.ConsumeRandomLengthString().c_str(), fdp.ConsumeFloatingPoint<double>());

  slog::SlogPrinter printer;
  printer.jsonString(slog_record);
}
