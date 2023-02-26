[Go to contents](../README.md#table-of_contents)

# ***Development***

[Preconditions](#preconditions)

[Examples source code](#examples-source-code)

[Creating empty application](./dev_create_empty_app.md#creating-empty-application)

[Creating application component](./dev_create_app_component.md#creating-application-component)

[Creating application with several components components](./dev_create_app_with_several_components.md#creating-application-with-several-components)



## ***Preconditions***

Let's suppose we have next preconditions:
   - Host OS: ***Ubuntu x86_64***
   - Target OS: ***Ubuntu x86_64***
   - CARPC prebuild path: ***/mnt/dev/carpc_product/linux-x86_64/delivery/***
   - Shell framework scripts: ***/mnt/dev/shell_fw/***
   - Python framework scripts: ***/mnt/dev/python_fw/***

Let's introduce next shell variables:

<pre>
ROOT_DIR=/mnt/dev/
TARGET_ARCH=x86_64
TARGET_OS=linux
CARPC_DELIVERY_DIR=${ROOT_DIR}/carpc_product/${TARGET_OS}-${TARGET_ARCH}/delivery/
CARPC_EXAMPLES_SOURCE_DIR=${ROOT_DIR}/carpc-examples/
CARPC_EXAMPLES_PRODUCT_DIR=${ROOT_DIR}/carpc-examples_product/${TARGET_OS}-${TARGET_ARCH}/
CARPC_EXAMPLES_DELIVERY_DIR=${CARPC_EXAMPLES_PRODUCT_DIR}/delivery/
SHELL_FW_DIR=${ROOT_DIR}/shell_fw/
PYTHON_FW_DIR=${ROOT_DIR}/python_fw/
</pre>

Next step we should create folder for our projects:

<pre>
mkdir -p ${CARPC_EXAMPLES_SOURCE_DIR}
</pre>

Copy build system to our project folder:

<pre>
cp -r ${CARPC_DELIVERY_DIR}/bin/build-system/* ${CARPC_EXAMPLES_SOURCE_DIR}
chmod +x ${CARPC_EXAMPLES_SOURCE_DIR}/build.sh
chmod +x ${CARPC_EXAMPLES_SOURCE_DIR}/build/generator/generator.py
</pre>

Create subfolder with CMakeList.txt for examples:

<pre>
mkdir ${CARPC_EXAMPLES_SOURCE_DIR}/examples
touch ${CARPC_EXAMPLES_SOURCE_DIR}/examples/CMakeList.txt
</pre>

This ***examples*** folder will containe all examples in separate folders (ex_01, ex_02, ...), so CMakeLists.txt file will be extended by new ***add_subdirectory(...)*** functions. Each subdirectory will contain additinal CMakeLists.txt file what will define target for current example and rules for it. The pattern example for mentioned CMakeLists.txt file could be found in ***${CARPC_EXAMPLES_SOURCE_DIR}/build/cmake/example/CMakeLists.txt***

Now we should include ***examples*** folder to build system. For this open ${CARPC_EXAMPLES_SOURCE_DIR}/CMakeList-projects.txt and change ***add_subdirectory( subfolder_with_project )*** to ***add_subdirectory( examples )***.



## ***Examples source code***

All source code for current examples below could be found in git:

<pre>
git clone https://github.com/dterletskiy/carpc-tutorial
</pre>
