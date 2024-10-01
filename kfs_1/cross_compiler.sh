apt install -y build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev

export PREFIX="$PWD/cross"
export TARGET=i386-elf
export PATH="$PREFIX/bin:$PATH"

mkdir src
cd src
#download binutils gcc in $PWD/src

# install gdb
wget "http://ftp.gnu.org/gnu/gdb/gdb-7.11.tar.gz"
tar -xvzf gdb-7.11.tar.gz
mkdir gdb
cd gdb
../gdb-7.11/configure --target=$TARGET --prefix="$PREFIX" --disable-werror
make all-gdb
make install-gdb


#install binutils
cd ..
mkdir build-binutils
cd build-binutils
../binutils-?.?.?/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

#install gcc
cd ..
mkdir build-gcc
cd build-gcc
../gcc-?.?.?/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers
make all-gcc
make all-target-liggcc
make install-gcc
make install-target-libgcc
