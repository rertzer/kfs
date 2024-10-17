FROM debian:bookworm-slim AS build

ENV PREFIX="/cross"
ENV TARGET=i386-elf
ENV PATH="$PATH:$PREFIX/bin"

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

RUN curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.43.tar.gz && tar -xvzf binutils-2.43.tar.gz

WORKDIR /tmp/build_binutils

RUN ../binutils-2.43/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror \
	&& make -j8 \
	&& make install

WORKDIR /tmp

RUN curl -O "https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.gz" && tar -xvzf gcc-14.2.0.tar.gz

WORKDIR /tmp/build_gcc

RUN ../gcc-14.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers \
	&& make -j8 all-gcc \
	&& make -j8 all-target-libgcc \
	&& make install-gcc \
	&& make install-target-libgcc

FROM debian:bookworm-slim

ENV PREFIX="/cross"
ENV PATH="$PATH:$PREFIX/bin"

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



RUN /bin/sh -c set -ex;\
	savedAptMark="$(apt-mark showmanual)";\
	apt-get update;\
	apt-get install -y --no-install-recommends dpkg-dev flex gnupg ;\
	rm -r /var/lib/apt/lists/*;\
	_fetch() { local fetch="$1";\
	shift;\
	local file="$1";\
	shift;\
	for mirror in $GCC_MIRRORS;\
	do if curl -fL "$mirror/$fetch" -o "$file";\
	then return 0;\
	fi;\
	done;\
	echo >&2 "error: failed to download '$fetch' from several mirrors";\
	return 1;\
	};\
	_fetch "gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.xz.sig" 'gcc.tar.xz.sig';\
	_fetch "gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.xz" 'gcc.tar.xz';\
	export GNUPGHOME="$(mktemp -d)";\
	for key in $GPG_KEYS;\
	do gpg --batch --keyserver keyserver.ubuntu.com --recv-keys "$key";\
	done;\
	gpg --batch --verify gcc.tar.xz.sig gcc.tar.xz;\
	gpgconf --kill all;\
	rm -rf "$GNUPGHOME";\
	mkdir -p /usr/src/gcc;\
	tar -xf gcc.tar.xz -C /usr/src/gcc --strip-components=1;\
	rm gcc.tar.xz*;\
	cd /usr/src/gcc;\
	./contrib/download_prerequisites;\
	{ rm *.tar.* || true;\
	};\
	for f in config.guess config.sub;\
	do wget -O "$f" "https://git.savannah.gnu.org/cgit/config.git/plain/$f?id=7d3d27baf8107b630586c962c057e22149653deb";\
	find -mindepth 2 -name "$f" -exec cp -v "$f" '{}' ';\
';\
	done;\
	dir="$(mktemp -d)";\
	cd "$dir";\
	extraConfigureArgs='';\
	dpkgArch="$(dpkg --print-architecture)";\
	case "$dpkgArch" in armel) extraConfigureArgs="$extraConfigureArgs --with-arch=armv5te --with-float=soft" ;\
;\
	armhf) extraConfigureArgs="$extraConfigureArgs --with-arch=armv7-a+fp --with-float=hard --with-mode=thumb" ;\
;\
	i386) extraConfigureArgs="$extraConfigureArgs --with-arch-32=i686";\
	;\
;\
	esac;\
	gnuArch="$(dpkg-architecture --query DEB_BUILD_GNU_TYPE)";\
	/usr/src/gcc/configure --build="$gnuArch" --disable-multilib --enable-languages=c,c++,fortran,go $extraConfigureArgs ;\
	make -j "$(nproc)";\
	make install-strip;\
	cd ..;\
	rm -rf "$dir" /usr/src/gcc;\
	apt-mark auto '.*' > /dev/null;\
	[ -z "$savedAptMark" ] || apt-mark manual $savedAptMark;\
	apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false # buildkit