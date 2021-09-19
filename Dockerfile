FROM gcc:latest

RUN apt-get update \
 && apt-get install -y \
    cmake \
    curl \
    git \
    make \
    vim \
    wget \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/*
