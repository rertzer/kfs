#!/bin/env bash

apt update

apt install -y build-essential grub2 bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev curl make file

export PREFIX="/cross"
export TARGET=i386-elf
export PATH="$PREFIX/bin:$PATH"

mkdir /tmp
cd /tmp  

# install gdb
# curl -O "http://ftp.gnu.org/gnu/gdb/gdb-7.11.tar.gz"
# tar -xvzf gdb-7.11.tar.gz
# mkdir build-gdb
# cd build-gdb
# ../gdb-7.11/configure --target=$TARGET --prefix="$PREFIX" --disable-werror
# make all-gdb
# make install-gdb


# cd ..
#install binutils
curl -O "https://ftp.gnu.org/gnu/binutils/binutils-2.43.tar.gz"
tar -xvzf binutils-2.43.tar.gz
mkdir build-binutils
cd build-binutils
../binutils-2.43/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

cd ..
#install gcc
curl -O "https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.gz"
tar -xvzf gcc-14.2.0.tar.gz
mkdir build-gcc
cd build-gcc
../gcc-14.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
