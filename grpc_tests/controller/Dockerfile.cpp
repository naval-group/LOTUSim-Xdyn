FROM debian:9 AS builder
RUN echo "deb http://deb.debian.org/debian stretch-backports main" >>  /etc/apt/sources.list && \
    apt-get update && \
    apt-get install --yes --no-install-recommends \
        make \
        cmake \
        build-essential \
        g++ \
        gcc \
        libbz2-dev \
        libc-ares-dev \
        libssl-dev \
        ninja-build \
        ca-certificates \
        wget \
    && \
    apt-get -t stretch-backports install --yes --no-install-recommends \
        libgrpc++-dev \
        libgrpc++1 \
        libgrpc-dev \
        libgrpc6 \
        libprotobuf-dev \
        libprotoc-dev \
        protobuf-compiler-grpc

RUN wget https://github.com/google/googletest/archive/release-1.8.1.tar.gz -O googletest.tar.gz && \
    mkdir -p /opt && \
    mkdir -p /opt/googletest && \
    tar -xf googletest.tar.gz --strip 1 -C /opt/googletest && \
    rm -rf googletest.tar.gz

ADD code/grpc/controller.proto /

WORKDIR /work
ADD grpc_tests/controller/CMakeLists.txt\
    grpc_tests/controller/Controller.cpp\
    grpc_tests/controller/Controller.hpp\
    grpc_tests/controller/FromGrpc.cpp\
    grpc_tests/controller/FromGrpc.hpp\
    grpc_tests/controller/ToGrpc.cpp\
    grpc_tests/controller/ToGrpc.hpp\
    grpc_tests/controller/PID.cpp\
    /work/

RUN mkdir build \
 && cd build \
 && cmake -Wno-dev \
	          -G Ninja \
	          -DCMAKE_BUILD_TYPE=Relase \
	          -DCMAKE_INSTALL_PREFIX:PATH=/opt/grpc_demo \
	          /work
RUN cd build && ninja

FROM debian:9-slim
COPY --from=builder /work/build/controller_server /usr
ENTRYPOINT ["/usr/controller_server"]
