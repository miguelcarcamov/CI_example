# Base image for the GitHub Actions workflow (and local parity): Ubuntu + C++ build
# tools. The workflow mounts the repo at /workspace and runs CMake/CTest there;
# this file does not copy or build the project itself.
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
  && apt-get install -y --no-install-recommends \
    ca-certificates \
    cmake \
    g++ \
    git \
    make \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

CMD ["bash"]
