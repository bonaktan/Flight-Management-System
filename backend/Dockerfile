# Build stage
FROM debian:bookworm-slim AS builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    clang \
    cmake \
    ninja-build \
    git \
    libjsoncpp-dev \
    uuid-dev \
    zlib1g-dev \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Install Drogon (cached separately)
WORKDIR /tmp
RUN git clone https://github.com/drogonframework/drogon.git && \
    cd drogon && \
    git submodule update --init && \
    mkdir build && cd build && \
    cmake -G Ninja \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_BUILD_TYPE=Release \
    .. && \
    ninja && ninja install && \
    cd ../.. && rm -rf drogon

# Copy only dependency files first (cached separately from source)
WORKDIR /app
COPY CMakeLists.txt ./
COPY vcpkg.json* ./
COPY conanfile.* ./

# Configure CMake to download dependencies
RUN mkdir build && cd build && \
    cmake -G Ninja \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    .. || true

# Now copy source code and build
COPY . .
RUN cd build && \
    cmake -G Ninja \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    .. && \
    ninja && ls

# Runtime stage
# BUG: its not running the backend, idk why
FROM alpine:latest

RUN apk add --no-cache \
    libstdc++ \
    libgcc \
    libuuid \
    jsoncpp \
    openssl

COPY --from=builder /app/build/SkyBridgeAirways-Backend /app/

WORKDIR /app
EXPOSE 8080

CMD ["/app/SkyBridgeAirways-Backend"]