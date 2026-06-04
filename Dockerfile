FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc-multilib \
    binutils \
    nasm \
    grub-pc-bin \
    xorriso \
    mtools \
    && apt-get clean

WORKDIR /os