package(default_visibility = ["//visibility:public"])

py_library(
  name = "pkg",
  srcs = glob(["**/*.py"]),
  data = glob(["**/*"], exclude=["**/*.py", "**/* *", "BUILD", "WORKSPACE"]),
  # This makes this directory a top-level in the python import
  # search path for anything that depends on this.
  imports = ["."],
  deps = [],
)

