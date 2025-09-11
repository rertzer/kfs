FROM debian:bookworm-slim AS build

ENV PREFIX="/cross"
ENV TARGET=i386-elf
ENV PATH="$PATH:$PREFIX/bin"

ARG gnu_mirror="https://mirror.ibcp.fr/pub"
#ARG gnu_mirror="http://ftp.gnu.org"

RUN apt update && apt install -y --no-install-recommends\
	ca-certificates\
	build-essential\
	bison flex\
	curl make\
	libgmp3-dev\
	libmpc-dev\
	libmpfr-dev\
	texinfo\
	libisl-dev\
	grub2 xorriso\
	nasm file

WORKDIR /tmp

RUN curl -O $gnu_mirror/gnu/binutils/binutils-2.43.tar.gz && tar -xvzf binutils-2.43.tar.gz

WORKDIR /tmp/build_binutils

RUN ../binutils-2.43/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror \
	&& make -j8 \
	&& make install

WORKDIR /tmp

RUN curl -O $gnu_mirror/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.gz && tar -xvzf gcc-14.2.0.tar.gz

WORKDIR /tmp/build_gcc

RUN ../gcc-14.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers \
	&& make -j8 all-gcc \
	&& make -j8 all-target-libgcc \
	&& make install-gcc \
	&& make install-target-libgcc

FROM debian:bookworm-slim

ENV PREFIX="/cross"
ENV PATH="$PATH:$PREFIX/bin:$PREFIX/i386-elf/bin"

RUN apt update && apt install -y --no-install-recommends\
	libgmp3-dev\
	libmpc-dev\
	libmpfr-dev\
	libisl-dev\
	grub2 xorriso\
	nasm file

COPY --from=build /cross/bin/i386-elf-gcc /cross/bin/
COPY --from=build /cross/i386-elf /cross/i386-elf
COPY --from=build /cross/include /cross/include
COPY --from=build /cross/lib /cross/lib
COPY --from=build /cross/libexec /cross/libexec


WORKDIR /app
