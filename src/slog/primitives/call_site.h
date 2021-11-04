#pragma once

#include <string>

#include "src/slog/util/inline_macro.h"

namespace avsoftware {

class SlogCallSite {
 public:
  SLOG_INLINE SlogCallSite(const std::string& function, const std::string& file,
                           int32_t line)
      : function_(function), file_(file), line_(line) {}

  SLOG_INLINE const std::string& function() const { return function_; }
  SLOG_INLINE const std::string& file() const { return file_; }
  SLOG_INLINE int32_t line() const { return line_; }

 private:
  std::string function_;
  std::string file_;
  int32_t line_;
};

}  // namespace avsoftware
