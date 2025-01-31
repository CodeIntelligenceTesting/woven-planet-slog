#include <assert.h>
#include "slog_cc/slog.h"
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <cifuzz/cifuzz.h>

// Function to check if SlogRecords are in ascending order based on time
static std::pair<bool, std::string> isAscending(const std::vector<slog::SlogRecord>& slog_records) {
    for (size_t i = 1; i < slog_records.size(); ++i) {
        const auto& prev = slog_records[i - 1];
        const auto& cur = slog_records[i];
        if (prev.time().elapsed_ns > cur.time().elapsed_ns) {
            return {false, slog::util::stringPrintf("time_elapsed_ns[%d] = %lld, time_elapsed_ns[%d] = %lld\nprev: %s\n cur: %s\n\n",
                                                    i - 1, prev.time().elapsed_ns, i, cur.time().elapsed_ns,
                                                    slog::SlogPrinter().debugString(prev).c_str(),
                                                    slog::SlogPrinter().debugString(cur).c_str())};
        }
    }
    return {true, ""};
}

FUZZ_TEST_SETUP() {
    // One-time setup if needed
}

FUZZ_TEST(const uint8_t *data, size_t size) {
    FuzzedDataProvider fdp(data, size);

    // Generate a number of SlogRecords
    std::vector<slog::SlogRecord> slog_records;
    size_t num_records = fdp.ConsumeIntegralInRange<size_t>(0, 100);

    for (size_t i = 0; i < num_records; ++i) {
        int32_t thread_id = fdp.ConsumeIntegral<int32_t>();
        int32_t call_site_id = fdp.ConsumeIntegral<int32_t>();
        int8_t severity = fdp.ConsumeIntegral<int8_t>();
        slog::SlogRecord record(thread_id, call_site_id, severity);
        // Populate record with data
        // Assuming a method to set time exists
        slog::SlogTimestamps timestamps;
        timestamps.elapsed_ns = fdp.ConsumeIntegral<int64_t>();
        record.set_time(timestamps);
        slog_records.push_back(record);
    }

    // Check if records are in ascending order
    isAscending(slog_records);
}
