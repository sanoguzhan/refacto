
FROM debian:buster

# Install dependencies
RUN apt-get -qq update; \
    apt-get install -qqy --no-install-recommends \
        ca-certificates  pkg-config libyaml-cpp-dev\
        autoconf automake cmake dpkg-dev file git make patch python3 \
        libc-dev libc++-dev libgcc-8-dev libstdc++-8-dev  \
        dirmngr gnupg2 lbzip2 wget xz-utils libtinfo5; \
    rm -rf /var/lib/apt/lists/*

# Signing keys
ENV GPG_KEYS 09C4E7007CB2EFFB A2C794A986419D8A B4468DF4E95C63DC D23DD2C20DD88BA2 8F0871F202119294 0FC3042E345AD05D

# Retrieve keys
RUN gpg --batch --keyserver ha.pool.sks-keyservers.net --recv-keys $GPG_KEYS

# Version info
ENV LLVM_RELEASE 10
ENV LLVM_VERSION 10.0.0

# Install Clang and LLVM
RUN chmod -R 700 /bin/sh


COPY install.sh .
RUN chmod +x install.sh 
RUN ./install.sh

#Install Required packages
COPY requires.sh .
RUN chmod +x requires.sh 
RUN ./requires.sh


RUN mkdir /code && mkdir /data
WORKDIR /code


