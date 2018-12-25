# C++ Component/Plugin

# Build


Use
`conan export dependencies/nanomsg inexorgame/testing`
and
`conan export dependencies/nanomsgxx inexorgame/testing`
to create the nanomsg c++ conan package.

Then build all C++ dependencies (i.e. nanomsgxx) with

`conan install . -if=build --build missing`


Afterwards build core by changing into the build directory and executing cmake:

`cd build && cmake .. && cmake --build .`
