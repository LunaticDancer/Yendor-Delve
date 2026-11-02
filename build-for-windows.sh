rm -rf build/build_files/obj build/bin build/build_files/*.make Makefile
cd build && ./premake5 --os=windows --graphics=opengl11 gmake && cd ..
# Patch the compiler directly
sed -i 's/^  CC = gcc$/  CC = i686-w64-mingw32-gcc/' build/build_files/*.make
sed -i 's/^  CXX = g++$/  CXX = i686-w64-mingw32-g++/' build/build_files/*.make
sed -i 's/^  AR = ar$/  AR = i686-w64-mingw32-ar/' build/build_files/*.make
# Verify the patch landed
grep -n 'CC =\|CXX =\|AR =' build/build_files/yendor-delve.make build/build_files/raylib.make
# Build
make -j"$(nproc)" config=debug_x86
