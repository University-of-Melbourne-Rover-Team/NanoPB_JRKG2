FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Download Linux support tools
RUN apt-get update && \
    apt-get clean &&  \
    apt-get install -y \
        build-essential \
        wget \
        curl \
        git  \
        python3 \
        python3-pip

# Set the dev directory
WORKDIR /home/dev

# Download and install RP2040 Toolchains
RUN apt-get -y install cmake \
        gcc-arm-none-eabi \
        libnewlib-arm-none-eabi 

# Clone and setup the RP2040 SDK
RUN git clone https://github.com/raspberrypi/pico-sdk /home/package/pico-sdk && \
     cd /home/package/pico-sdk && \
    git submodule update --init

ENV PICO_SDK_PATH=/home/package/pico-sdk

WORKDIR /home/dev


RUN git clone https://github.com/nanopb/nanopb/ /home/package/nanopb && \
    cd /home/package/nanopb && \
    git submodule update --init && \
    pip3 install --user -r requirements.txt

ENV NANOPB_PATH=/home/package/nanopb

# 7. Final working directory
WORKDIR /home/Desktop
