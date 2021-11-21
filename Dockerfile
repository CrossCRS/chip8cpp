FROM ubuntu:rolling

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Warsaw

RUN apt-get update && apt-get install -y \
        git \
        libsfml-dev \
        libgl1-mesa-dev \
        libx11-dev \
        libxrandr-dev \
        libxi-dev \
        libudev-dev \
        build-essential \
        curl \
        zip \
        unzip \
        tar \
        pkg-config \
        ninja-build \
        cmake 

COPY . /usr/src/chip8
WORKDIR /usr/src/chip8
RUN rm -rf Chip8CPP build/

RUN ./extern/vcpkg/bootstrap-vcpkg.sh && cmake -G Ninja -B build -S . -DCMAKE_TOOLCHAIN_FILE=extern/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE:STRING=Debug