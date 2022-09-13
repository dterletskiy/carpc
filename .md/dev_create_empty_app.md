[Go to previous menu](./development.md#development)

# ***Creating empty application***

   - [Implementation](#implementation)
   - [Building](#building)
   - [Verification](#verification)
   - [Source code](#source-code)



## ***Implementation***

### ***Example folder***

Let's create first example folder ***ex_01***:

<pre>
mkdir ${CARPC_EXAMPLES_SOURCE_DIR}/examples/ex_01
</pre>

### ***Building rules***

Afterward CMakeLists.txt file must be written what will define new target and rules for its building. In this case could be used template CMakeLists.txt file located in ***${CARPC_EXAMPLES_SOURCE_DIR}/build/cmake***.

<pre>
cp ${CARPC_EXAMPLES_SOURCE_DIR}/build/cmake/CMakeLists.txt ${CARPC_EXAMPLES_SOURCE_DIR}/examples/ex_01
</pre>

The only thing should be done on this stage is just rename project name ***project( ... )***. In this case let's call it ***EXAMPLE_01***.

According to rules defined in this CMakeList.txt file and base functiones used from build system there will be defined new executable target with name equal to folder name ***ex_01***. This target will be built from all ***.cpp*** files located in current example folder. Additionally to these source files will be added source files generated based on all ***.xdl*** filed located in example folder.

### ***adl implementation***

***CARPC*** framework and build system supports predefined description language for describing application structure, data and interfaces.

To implement simpliest application it's enough to implement *.adl file with minimal description inside. Based on this file build system will generate ***main.cpp*** file with ***main*** function and other required stuff for initializing ***CARPC runtime***.

Let's create ***ex_01.adl*** file and fill it by next content:

<pre>
author: dterletskiy;
version: 1.0.0;

application: example_01
{
};
</pre>

***author*** - is the author of current file and not strictly required.

***version*** - is the version of current file in format ***MAJOR.MINOR.PATCH***.

***application*** - application name (***example_01*** in current case) and application description structure what will be filled later in curly brackets.

Theoretically it's enough for building application. This application will not containe any ***Application Thread*** and ***Application Component*** so it will not perform any logic. The only thing what will be done during starting this application is ***CARPC runtime*** initialization: creating ***Application Process*** instance, reading parameters and initializing all required stuff according to them.

### ***Defining configuration***

There are several ways to pass parameters to application:
   1. using command line parameters
   2. using environment variables
   3. using configuration file

All these options could be used at the same time. The same parameter could be passed with different values using different variants. In this case more prioritized will be used according to previous list (first one with lowest priority, last one - highest)

By default configuration file should have the same name as exacutable target file with .cfg extantion and placed at the same folder. It will be searched during runtime. But its name and location could be changed and passed via command line parameter:

<pre>
config=/path_to_configuration_file
</pre>

All supported configuration parameters are listed in [Configuration Parameters](./configuration_parameters.md#configuration-parameters).

Lest's create configuration file ***ex_01.cfg*** and specify logging startegy to console and setup no IPC communication for current example:

<pre>
touch ${CARPC_EXAMPLES_SOURCE_DIR}/examples/ex_01/ex_01.cfg
</pre>

<pre>
ipc=false

trace_log=CONSOLE
</pre>

This filename will be used by default to identify and copy configuration file to delivery (installation) folder during executing ***install*** target.



## ***Building***

To build current application ***build.sh*** script should be called with next parameters:

<pre>
./build.sh --sfw=${SHELL_FW_DIR} --pfw=${PYTHON_FW_DIR} --carpc=${CARPC_DELIVERY_DIR} --arch=x86_64 --os=linux --action=clean_build
</pre>

As the result there will be ***${CARPC_EXAMPLES_DELIVERY_DIR}/bin/ex_01*** binary file.



## ***Verification***

To verify current application it is just enough to run it and have a look to produced traces:

<pre>
./build.sh --sfw=${SHELL_FW_DIR} --pfw=${PYTHON_FW_DIR} --carpc=${CARPC_DELIVERY_DIR} --arch=x86_64 --os=linux --action=start --target=ex_01
</pre>

Application will be started and stopped immidiatly because of missing and logic inside.

There will be next traces in console:

<pre>
[runtime] creating...
[runtime] created
[runtime] starting...
[runtime] started
[runtime] running...
[runtime] stopped
[runtime] destroyed
</pre>



## ***Source code***

Source code for current example could be fould here:

<pre>
https://github.com/dterletskiy/carpc-tutorial/examples/ex_01/
</pre>
