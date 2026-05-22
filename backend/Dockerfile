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
FROM debian:bookworm-slim
# Install runtime dependencies
RUN apt-get update -q && apt-get install -qy \
    libjsoncpp-dev \
    uuid-dev \
    zlib1g-dev \
    openssl \
    libssl-dev \
    libpq-dev \
    jq \
    && rm -rf /var/lib/apt/lists/*
RUN useradd -s /bin/bash skybridge_backend && mkdir /app && \
    chmod 744 -R /app && \
    chown skybridge_backend:skybridge_backend /app 

# Copy Drogon libraries
COPY --from=builder /usr/local/lib /usr/local/lib
RUN ldconfig

COPY --from=builder --chown=skybridge_backend:skybridge_backend --chmod=744 /app/build/SkyBridgeAirways-Backend /app/build/config.json /app/run.sh /app/
USER skybridge_backend
RUN mkdir -p /app/logs 
WORKDIR /app

CMD ["./run.sh"]