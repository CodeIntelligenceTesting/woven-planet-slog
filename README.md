# Slog
Slog (reads "S-log") is a simple, fast, and cross-language structured logging. It's like Glog but allows structure via tags and async handling.

# Fuzzing with CI Fuzz

For fuzzing, we prepared a dev container setup. The setup and docker setup can be found in `.devcontainer`

## Spark
Spark automatically selects the top functions to test and automatically creates fuzz test for them. These fuzz tests are then built and run until a given goal (target coverage or number of tests) is reached.
```
cifuzz spark --build-dir . --candidate-excludes="external/**;bazel-out/**" --usage-excludes="external/**;bazel-out/**" --build-file cifuzz-spark/BUILD.bazel
```

For a verbose output, you can use the `-v` flag

Spark needs a compilation database which you can generated with the following command:
```
bazel run @hedron_compile_commands//:refresh_all
```

## Fuzzing candidates
To get the most important functions to test/fuzz:

```
cifuzz candidates --build-dir . --candidate-excludes="external/**;bazel-out/**" --num-candidates=1
```

The `--candidate-excludes` flag is there to filter out source files that we don't want to consider in the analysis.

## Create fuzz tests for selected functions/methods
You can create a fuzz test for selected functions or methods using the `create` command of CI Fuzz. 

```
cifuzz create SlogContext::getInstance --build-dir . --build-jobs=0 --usage-excludes="external/**;bazel-out/**" -v -o cifuzz-spark/fuzz.cpp --validate --fuzz-test-name //cifuzz-spark:fuzz_bin
```

When using the `--validate` flag, CI Fuzz will build and run the fuzz test to verify the created test and refine it if necessary.

For this to work the build system should be able to build the generated fuzz test. For example, for the previous command, you can add the following block to `cifuzz-spark/BUILD.bazel`:
```
cc_fuzz_test(
    name = "fuzz",
    srcs = [
        "fuzz.cpp",
    ],
    corpus = glob(
        ["fuzz_inputs/**"],
        allow_empty = True,
    ) + select({
        "@cifuzz//:collect_coverage": glob([".fuzz_cifuzz_corpus/**"], allow_empty = True),
        "//conditions:default": [],
    }),
    deps = [
        "@cifuzz",
        "//:slog_cc",
    ],
)
```

# Importing
## Into a C++ project built by Bazel
Requirements:
* C++14

Instructions: 
* Follow the example from `test_import/example_project_cc_via_bazel` directory.

## Into a Python project built by Bazel
Requirements:
* All C++ requirements from above
* Python 3.8
* pybind11 and pybind11_bazel

Instructions: 
* Follow the example from `test_import/example_project_py_via_bazel` directory.

# Contributing
## Installing tools for build
* `scripts/setup_dev_env.sh`

## Running tests
* `bazelisk test --test_output=errors //...` -- run all unit tests.

## Code formatting
* For the first time, install linters with `./scripts/install_linters.sh `
* `scripts/lint.sh ./ -i` -- automatically format all code.

## Releases.
TODO(vsbus): update this section.
CI automatically uploads a zip-archive to https://artifactory.pdx.l5.woven-planet.tech/repository/webapp/#/artifacts/browse/tree/General/mothership/slog/. This .zip archive could be imported into another bazel project, e.g. using the following URL: `https://artifactory.pdx.l5.woven-planet.tech/repository/mothership/slog/slog-v0.2.27.zip`.  The .zip file naming and content are matching a .zip file that could be created via GitHub releases at https://github.tri-ad.tech/level5/slog/releases.

