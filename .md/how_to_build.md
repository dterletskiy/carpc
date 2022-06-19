[Go to contents](../README.md#table-of_contents)

# ***How to build***
   1. [Compilation in Linux for Linux target OS](#for-linux-target-os)
   2. [Cross compilation for Android target OS](#cross-compilation-in-linux-for-android-target-os)
   3. [Compilation in AOSP for Android OS](#compilation-in-aosp-for-android-os)

## ***For Linux target OS***

## ***Preconditions***:
Tested host OS:

<pre>
Ubuntu 18.04-21.04
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


 - dlt development headers and libraries:

<pre>
sudo apt install libdlt-dev
</pre>


 - google protobuf:

<pre>
sudo apt install libprotobuf-dev protobuf-compiler
</pre>


## ***Cloning git repositories***:

 - shell framework helper scripts:

<pre>
git clone git@github.com:dterletskiy/shell_fw.git
</pre>


 - python framework helper scripts:

<pre>
git clone git@github.com:dterletskiy/python_fw.git
</pre>


 - clone ***CARPC*** source code:

<pre>
git clone git@github.com:dterletskiy/carpc.git
</pre>


## ***Build CARPC***

Let's suppose you have cloned all previous stuff to ***/mnt/dev/*** folder. So, you have next ***/mnt/dev/*** folder content:
 - carpc
 - shell_fw
 - python_fw

***carpc*** folder contains next stuff:
   - ***build system***: build/, build.sh, CMakeList.txt, Android.bp - scripts and configuration files for building carpc project and other projects based on carpc.
   - ***tracing project***: tracing/ - tracing library sources.
   - ***service brocker project***: servicebrocker/ - servicebrocker binary sources required for IPC/RPC.
   - ***carpc runtime project***: framework/ - carpc runtime library sources.

There is ***buils.sh*** script in ***carpc*** folder. This script could be used to build ***CARPC*** framework libraries. For this it should be started with next parameters:

<pre>
./build.sh --sfw=/mnt/dev/shell_fw/ --pfw=/mnt/dev/python_fw/ --arch=x86_64 --os=linux --action=clean_build
</pre>

Where:
   - action - defineds action you are going to do. We will choose ***clean_build***, but also are supported next actions:
      - ***clean_build*** - do a clean build for all projects in carpc
      - ***build*** - do an incremental build for all projects in carpc
      - ***clean*** - cleaning last build for all projects in carpc
   - arch - defines target architecture: ***x86*** or ***x86_64*** are tested.
   - linux - defines target operating system: ***linux*** or ***android***.
   - sfw - path to shell framework scripts.
   - pfw - path to python framework scripts.

In case of compiling for other architecture and os corresponding cross-compiler setting environment must me

When build is complete its result could be found in folder ***/mnt/dev/carpc_product/linux-x86_64/***. This path contains next folders:
   - ***build*** - cmake build artifacts
   - ***delivery*** - build results: libraries, binaries, headers and some configurations
     - ***bin*** - binaries (servicebrocker and build system what could be used for building applications base on ***CARPC***)
     - ***etc*** - some configurations
     - ***include*** - headers
     - ***lib*** - libraries
   - ***gen*** - some generated files required for build

Built CARPC could be installed to the system (Linux) just by copying ***delivery*** stuff folder to ***~/.local/***.



## ***Cross compilation in Linux for Android target OS***

The common steps for cross compiling ***CARPC*** for Android OS are the same as for Linux. The only differense is there must be set some cross compiler definitions and sourced in build system scripts.

As for example you can see ***./build/android-env.sh*** script where are all required definitions for cross compilation ***CARPC*** source code on Linux OS for Android OS. This files is written according rules described in [Use the NDK with other build systems](https://developer.android.com/ndk/guides/other_build_systems).

Also must be downloaded Android NDK from [NDK Downloads](https://developer.android.com/ndk/downloads), extracted and mentioned in ***./buil/android-env.sh***.

***./buil/android-env.sh*** is mentioned in ***./build/constants.sh*** as:

<pre>
readonly ENVIRONMENT_SETUP_ANDROID=./build/android-env.sh
</pre>

and sourced in case if defined parameter ***--os=linux***.



## ***Compilation in AOSP for Android OS***

***CARPC*** sources could be built directly in AOSP using ***Android.bp*** file.
