
FROM python:3.9.1

# Install dependencies
RUN apt-get -qq update; \
    apt-get install -qqy --no-install-recommends \
        ca-certificates  pkg-config cppcheck\
         libsasl2-dev python3-dev libldap2-dev libssl-dev gcc libpq-dev python-dev ninja-build\
        llvm  binutils git autoconf automake build-essential libhdf5-serial-dev vim cmake \
         swig libncurses5-dev libedit-dev libpthread-stubs0-dev g++ make python3-venv python3-wheel\
        autoconf automake dpkg-dev file git make patch python3 python3-pip\
        libc-dev libc++-dev libgcc-8-dev libstdc++-8-dev libboost-all-dev libre2-dev\
        dirmngr gnupg2 lbzip2 wget xz-utils libtinfo5 nfs-common;\
    rm -rf /var/lib/apt/lists/*

# Signing keys
# ENV GPG_KEYS 09C4E7007CB2EFFB A2C794A986419D8A B4468DF4E95C63DC D23DD2C20DD88BA2 8F0871F202119294 0FC3042E345AD05D

# Retrieve keys
# RUN gpg --batch --keyserver ha.pool.sks-keyservers.net --recv-keys $GPG_KEYS

# Version info
ENV LLVM_RELEASE 10
ENV LLVM_VERSION 10.0.0

WORKDIR /code

# Install Clang and LLVM
RUN chmod -R 700 /bin/sh
RUN python3 -m pip install --upgrade pip && pip install wheel
RUN pip3 install -U pip setuptools wheel
RUN pip3 install conan 

COPY docker/install.sh .
RUN chmod +x install.sh 
RUN ./install.sh

#Install Required packages
COPY docker/requires.sh .
RUN chmod +x requires.sh 
RUN ./requires.sh

COPY docker/conanfile.txt ./code/conanfile.txt
RUN conan install . && conan profile update settings.compiler.libcxx=libstdc++11 default && conan install . --build=missing





