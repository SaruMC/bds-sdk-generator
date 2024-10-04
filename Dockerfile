# Use an Ubuntu base image
FROM ubuntu:latest

# Install dependencies including LLVM, clang, and additional libraries
RUN apt-get update && apt-get install -y \
        build-essential \
        clang-17 \
        clang-tools-17 \
        cmake \
        git \
        libxml2-dev \
        ninja-build \
        unzip \
        wget \
        zlib1g-dev \
        libzstd-dev \
    && rm -rf /var/lib/apt/lists/*

# Install the latest libssl1.1 from an older repository
RUN apt-get update && \
    apt-get install -y software-properties-common && \
    apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 3B4FE6ACC0B21F32 && \
    add-apt-repository 'deb http://security.ubuntu.com/ubuntu bionic-security main' && \
    apt-get update && \
    apt-get install -y libssl1.1 && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Add Kitware APT repository and install the latest CMake
RUN apt-get update && \
    apt-get install -y apt-transport-https ca-certificates gnupg && \
    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add - && \
    apt-add-repository 'deb https://apt.kitware.com/ubuntu/ focal main' && \
    apt-get update && \
    apt-get install -y cmake && \
    rm -rf /var/lib/apt/lists/*

# Set a Build Argument for BDS Version
ARG BDS_VERSION=1.21.32.04

# Define and compare with the maximum allowed version (after that microsoft doesn't release the PDB file anymore..)
ENV MAX_BDS_VERSION=1.16.201.02

# Compare the BDS version with MAX_BDS_VERSION
# If BDS_VERSION is greater than MAX_BDS_VERSION, exit with an error
RUN if [ "$(printf '%s\n' "$MAX_BDS_VERSION" "$BDS_VERSION" | sort -V | head -n1)" != "$MAX_BDS_VERSION" ]; then \
        echo "BDS_VERSION ($BDS_VERSION) is greater than MAX_BDS_VERSION ($MAX_BDS_VERSION). Exiting..."; \
        exit 1; \
    else \
        echo "BDS_VERSION ($BDS_VERSION) is not greater than MAX_BDS_VERSION ($MAX_BDS_VERSION). Proceeding..."; \
    fi

# Set the working directory inside the container
WORKDIR /usr/src/app

# Copy the source files into the container
COPY . .

# Create the build directory
RUN mkdir -p build

# Change working directory to build (eq. cd build)
WORKDIR /usr/src/app/build

# Export the compilers explicitly
ENV CC=/usr/bin/clang-17
ENV CXX=/usr/bin/clang++-17

# Run CMake to configure the project, specifying the ninja build system
RUN cmake -G Ninja -DLLVM_DIR=/usr/lib/llvm-17/cmake -DCMAKE_C_COMPILER=/usr/bin/clang-17 -DCMAKE_CXX_COMPILER=/usr/bin/clang++-17 ..

# Compile the project
RUN ninja

# Move back to app directory for downloading and extracting PDB file
WORKDIR /usr/src/app

# Download the bedrock server zip file
# RUN wget https://www.minecraft.net/bedrockdedicatedserver/bin-win/bedrock-server-${BDS_VERSION}.zip -O bedrock-server.zip

RUN BDS_ZIP_URL="https://minecraft.azureedge.net/bin-win/bedrock-server-${BDS_VERSION}.zip" && \
    wget "$BDS_ZIP_URL" -O bedrock-server.zip

# Unzip the downloaded file to a specified directory
RUN unzip bedrock-server.zip -d bedrock-server

# Assuming there is a PDB file named bedrock_server.pdb in the extracted contents
# You can move it to a desired location or make it available to your application
RUN cp bedrock-server/bedrock_server.pdb bin/bedrock_server.pdb

ENTRYPOINT ["./bin/bds-sdk-generator", "./bin/bedrock_server.pdb"]