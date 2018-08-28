rm -rf build_cmake
rm -rf orca_lib
mkdir orca_lib
mkdir build_cmake
cd build_cmake
cmake ..
make
cp liborca_static.a ../orca_lib/
cp liborca_shared.so ../orca_lib/
