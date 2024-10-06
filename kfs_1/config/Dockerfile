FROM debian:latest

ENV PREFIX="/cross"
ENV TARGET=i386-elf
ENV PATH="$PATH:$PREFIX/bin"

RUN apt update
RUN apt install -y build-essential\
	bison flex\
	libgmp3-dev\
	libmpc-dev\
	libmpfr-dev\
	texinfo\
	libisl-dev\
	curl make\
	grub2 xorriso\
	nasm file

WORKDIR /tmp

RUN curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.43.tar.gz
RUN tar -xvzf binutils-2.43.tar.gz

WORKDIR /tmp/build_binutils

RUN ../binutils-2.43/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
RUN make
RUN make install

WORKDIR /tmp

RUN curl -O "https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.gz"
RUN tar -xvzf gcc-14.2.0.tar.gz

WORKDIR /tmp/build_gcc
RUN ../gcc-14.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers
RUN make all-gcc
RUN make all-target-libgcc
RUN make install-gcc
RUN make install-target-libgcc

WORKDIR /app