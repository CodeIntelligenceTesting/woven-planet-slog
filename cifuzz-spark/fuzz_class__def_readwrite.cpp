#include <assert.h>
#include <cifuzz/cifuzz.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "slog_cc/events/event.h"
#include "slog_cc/primitives/record.h"
#include "slog_cc/printer/printer.h"

FUZZ_TEST_SETUP() {
    // Initialization tasks if needed
}

FUZZ_TEST(const uint8_t *data, size_t size) {
    FuzzedDataProvider fdp(data, size);

    // Create a SlogEvent
    int8_t severity = fdp.ConsumeIntegral<int8_t>();
    int32_t event_id = fdp.ConsumeIntegral<int32_t>();
    slog::SlogEvent event(severity, event_id);

    // Add tags to the event
    std::string key = fdp.ConsumeRandomLengthString(10);
    int64_t int_value = fdp.ConsumeIntegral<int64_t>();
    double double_value = fdp.ConsumeFloatingPoint<double>();
    std::string string_value = fdp.ConsumeRandomLengthString(20);

    event.addTag(key, int_value);
    event.addTag(key, double_value);
    event.addTag(key, string_value);

    // Emit a value
    std::string emit_value = fdp.ConsumeRandomLengthString(30);
    event << emit_value;

    // Create a SlogRecord
    int32_t timestamp = fdp.ConsumeIntegral<int32_t>();
    int32_t thread_id = fdp.ConsumeIntegral<int32_t>();
    int8_t severity_level = fdp.ConsumeIntegral<int8_t>();
    slog::SlogRecord record(timestamp, thread_id, severity_level);

    // Convert record to JSON string
    std::string json_str = slog::SlogPrinter().jsonString(record);
}
