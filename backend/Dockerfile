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
    libpq-dev \
    && rm -rf /var/lib/apt/lists/*

# Install Drogon (cached separately)
WORKDIR /tmp
RUN git clone https://github.com/drogonframework/drogon.git && \
    cd drogon && \
    git checkout 4c5430757ea5451a7c38fbbef4b4bef7dbb47f2f && \
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
    ninja

# Runtime stage
# BUG: its not running the backend, idk why
FROM debian:bookworm-slim

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libjsoncpp-dev \
    uuid-dev \
    zlib1g-dev \
    openssl \ 
    libssl-dev \
    libpq-dev \
    && rm -rf /var/lib/apt/lists/*

# Copy Drogon libraries
COPY --from=builder /usr/local/lib /usr/local/lib
COPY --from=builder /app/build/SkyBridgeAirways-Backend /app/build/config.json /app/
RUN ldconfig && mkdir /app/logs
WORKDIR /app
EXPOSE 8080

CMD ["./SkyBridgeAirways-Backend"]