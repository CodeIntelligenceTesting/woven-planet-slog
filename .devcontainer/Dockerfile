# Use Ubuntu 20.04 as the base image (which includes Python 3.8)
FROM ubuntu:20.04

# Disable interactive prompts during package installs
ENV DEBIAN_FRONTEND=noninteractive

##############################
# Install prerequisites, Git, build-essential, zstd, and Python 3.8 & pip
##############################
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
      software-properties-common \
      wget \
      curl \
      gnupg \
      lsb-release \
      git \
      build-essential \
      lcov \
      zstd && \
    apt-get install -y --no-install-recommends \
      python3.8 \
      python3-pip && \
    rm -rf /var/lib/apt/lists/*

##############################
# Install Golang (official installer)
##############################
# Set the desired Go version (adjust as needed)
ENV GO_VERSION=1.20.5
RUN wget https://golang.org/dl/go${GO_VERSION}.linux-amd64.tar.gz && \
    tar -C /usr/local -xzf go${GO_VERSION}.linux-amd64.tar.gz && \
    rm go${GO_VERSION}.linux-amd64.tar.gz
# Update PATH so that Go commands are available
ENV PATH="/usr/local/go/bin:${PATH}"


##############################
# Install Bazelisk using Go install and create a symlink named 'bazel'
##############################
# Set GOBIN to /usr/local/bin so that the installed binary is in PATH
ENV GOBIN=/usr/local/bin
RUN go install github.com/bazelbuild/bazelisk@latest && \
    ln -s /usr/local/bin/bazelisk /usr/local/bin/bazel

RUN --mount=type=secret,id=CIFUZZ_CREDENTIALS,required sh -c "$(curl -fsSL http://downloads.code-intelligence.com/assets/install-cifuzz.sh)" $(cat /run/secrets/CIFUZZ_CREDENTIALS) latest  
RUN cifuzz completion bash > /etc/bash_completion.d/cifuzz

RUN wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh && \
    ./llvm.sh 15 && \
    rm llvm.sh

RUN ln -sf /usr/bin/clang-15 /usr/bin/clang && \
    ln -sf /usr/bin/clang++-15 /usr/bin/clang++ && \
    ln -sf /usr/bin/llvm-ar-15 /usr/bin/llvm-ar && \
    ln -sf /usr/bin/llvm-symbolizer-15 /usr/bin/llvm-symbolizer && \
    ln -sf /usr/bin/llvm-cov-15 /usr/bin/llvm-cov && \
    ln -sf /usr/bin/llvm-objdump-15 /usr/bin/llvm-objdump && \
    ln -sf /usr/bin/llvm-profdata-15 /usr/bin/llvm-profdata

# Default command (start a bash shell)
CMD ["/bin/bash"]

