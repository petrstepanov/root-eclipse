# root-eclipse
A simple CERN ROOT based C++ project with GNU Makefile and CMake. Project can be easily set up in Eclipse for enchanced debugging and development.

## Setting up project in Eclipse (CMake)
Project contains test CMakeLists.txt to generate a ROOT-based eclipse project. It is better to generate Eclipse project out-of-source:
```
cd ../
mkdir root-eclipse-project
cd root-eclipse-project
cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../root-eclipse
```
## Adding ROOT sources to Eclipse


## Compiling with GNU Make
Alternatively, project contains a regular hand-written `Makefile-GNU` that can compile and install your program on MacOs or Linux. First, define your executable name on the line  ``
```
make Makefile_GNU
make install
```
