# [Choice] Base image version: debian, bullseye, buster, stretch (refer to https://mcr.microsoft.com/v2/vscode/devcontainers/base/tags/list for full list)
ARG VARIANT
FROM mcr.microsoft.com/vscode/devcontainers/base:${VARIANT}

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
  && apt-get -y install --no-install-recommends \
  # supercharged debian
  fzf \
  silversearcher-ag \
  # platform io
  bc \
  build-essential \
  clang \
  curl \
  gcc \
  gdb \
  lldb \
  python3 \
  python3-dev \
  python3-distutils \
  python3-setuptools \
  python3-pip \
  simavr \
  srecord \
  xz-utils \
  && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

RUN pip3 install -U platformio
