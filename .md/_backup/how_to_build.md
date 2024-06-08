[Go to contents](../README.md#table-of_contents)

# ***How to build***
   1. [Compilation in Linux for Linux target OS](#for-linux-target-os)
   2. [Cross compilation for Android target OS](#cross-compilation-in-linux-for-android-target-os)
   3. [Compilation in AOSP for Android OS](#compilation-in-aosp-for-android-os)

## ***For Linux target OS***

## ***Preconditions***:
Tested host OS:

<pre>
Ubuntu 18.04-22.04
Arch Linux
</pre>

## ***Required installations***:

 - C++ compilers (must support C++11):

<pre>
sudo apt install build-essential
sudo apt install clang
</pre>


 - cmake:

<pre>
sudo apt install cmake
</pre>


 - git:

<pre>
sudo apt install git
</pre>


 - python (minimal version 3.8.9) and python-pip:

<pre>
sudo apt install python3 python3-pip
</pre>


 - python-antlr4 runtime:

<pre>
pip install antlr4-python3-runtime
</pre>


 - dlt development headers and libraries for development and dlt-daemon and dlt-viewer for collecting traces in future:

<pre>
sudo apt install libdlt-dev dlt-daemon dlt-viewer
</pre>


 - google protobuf:

<pre>
sudo apt install libprotobuf-dev protobuf-compiler
</pre>


## ***Cloning git repositories***:

 - ***CARPC*** source code:

<pre>
git clone git@github.com:dterletskiy/carpc.git
</pre>


 - Initialize submodules:

<pre>
git submodule init
git submodule update
</pre>


## ***Build CARPC***

***carpc*** folder contains next stuff:
   - ***build system***: build/, CMakeList.txt, projects.cmake, Android.bp - scripts and configuration files for building carpc project and other projects based on carpc.
   - ***tracing project***: tracing/ - tracing library sources.
   - ***service brocker project***: servicebrocker/ - servicebrocker binary sources required for IPC/RPC.
   - ***carpc runtime project***: runtime/ - carpc runtime library sources.
   - ***thirdparty***: thirdparty/ - required thirdparty binaries.

There is ***CMakeList.txt*** file in ***carpc*** folder what is the main cmake build description and rule file. This file is used for building ***carpc*** framework.

Configuring:

<pre>
cmake -S /source_dir/ -B /build_dir/
</pre>

Where:
   - source_dir - path to source directory where main **CMakeList.txt** file is located.
   - build_dir - path to build directory where CMake-generated project binary tree will be generated.

Building:

<pre>
cmake --build /build_dir/ -j /jobs/
</pre>

Where:
   - build_dir - path to build directory where CMake-generated project binary tree is located after previous step.
   - jobs - number of jobs what will be used for building.




## ***Cross compilation in Linux for Android target OS***

@TDA: ToDo



## ***Compilation in AOSP for Android OS***

***CARPC*** sources could be built directly in AOSP using ***Android.bp*** file.
