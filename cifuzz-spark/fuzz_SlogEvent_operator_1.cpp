#include <cifuzz/cifuzz.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "slog_cc/slog.h"
#include "slog_cc/context/context.h"
#include "slog_cc/primitives/call_site.h"
#include "slog_cc/util/string_util.h"

using slog::SlogTag;
using slog::SlogContext;
using slog::SlogRecord;

FUZZ_TEST_SETUP() {
  // Setup tasks.
}

FUZZ_TEST(const uint8_t *data, size_t size) {
  FuzzedDataProvider fdp(data, size);

  // Fuzzing multiple SlogTags and logging.
  std::string tag_key1 = fdp.ConsumeRandomLengthString(10);
  std::string tag_value1 = fdp.ConsumeRandomLengthString(20);

  SLOG(INFO) << "Log with a tag: "
             << SlogTag(tag_key1.c_str(), tag_value1.c_str());

  // Ensure call site is valid before logging.
  auto context = SlogContext::getInstance();
  int call_site_id = context->addCallSite(tag_key1, tag_value1, 0);
  if (call_site_id >= 0) {
    context->createAsyncSubscriber(
      [](const SlogRecord& record) {
        // Process record.
      }
    );
    context->resetCallSites();
  }
}
