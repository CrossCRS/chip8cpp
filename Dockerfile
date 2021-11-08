FROM alpine:3.14

RUN apk add --no-cache \
        git \
        sfml-dev \
        mesa-dev \
        build-base \
        cmake 

WORKDIR /usr/src/

RUN git clone -b v1.85 --depth 1 https://github.com/ocornut/imgui && \
    git clone -b v2.3 --depth 1 https://github.com/eliasdaler/imgui-sfml && \
    cd imgui-sfml && \
    cmake -DBUILD_SHARED_LIBS=ON -DIMGUI_DIR=/usr/src/imgui . && \
    cmake --build . --target install

COPY . /usr/src/chip8
WORKDIR /usr/src/chip8

CMD ["cmake ."]