[Go to contents](../../README.md#table-of_contents)

## Cloning and Building the CARPC Framework

To set up and build the CARPC framework, follow the steps below.

First of all, let's define the working directories as follows:
- `FENIX_DIR`: Directory where the Fenix build system will reside.
- `FRAMEWORK_DIR`: Directory for all CARPC framework projects.

### Step 1: Clone the Fenix Build System

First, clone the Fenix build system:

```sh
git clone https://github.com/dterletskiy/carpc-builder.git ${FENIX_DIR}
```

### Step 2: Clone CARPC Framework Repositories

Next, assume that all source codes for the CARPC framework projects will be located in the `${FRAMEWORK_DIR}` directory. Clone all CARPC framework repositories using the auxiliary script:

```sh
${FENIX_DIR}/do.sh --source=${FRAMEWORK_DIR} --action=fetch --target=framework
```

### Step 3: Navigate to the Framework Directory

According to the Fenix build system instructions, the source path for the projects (in this case, `${FRAMEWORK_DIR}`) must be specified during the build process using the `--source` parameter. However, if this parameter is omitted, the auxiliary script will assume the current path as the source path. Therefore, change the directory to `${FRAMEWORK_DIR}`:

```sh
cd ${FRAMEWORK_DIR}
```

### Step 4: Configure the Project

On this step will be configured CMake build system according to parameters passed from the script and parsed cmake files:

```sh
${FENIX_DIR}/do.sh --action=config
```

This step creates a separate directory for all CMake temporary files and build artifacts. By default, this directory is named `_product_` and is located within the project source directory, as specified in the build script.

### Step 5: Build All Targets

```sh
${FENIX_DIR}/do.sh --action=build
```

The result of this step is a set of build artifacts located in the build directory `${FRAMEWORK_DIR}/_product_/build/`.

### Step 6: Install Build Artifacts

```sh
${FENIX_DIR}/do.sh --action=install
```

By default, according to the auxiliary build script implementation, all artifacts will be installed to `${FRAMEWORK_DIR}/_product_/deploy/`. This directory will contain the following subdirectories:

 - bin: Contains the `carpc` subdirectory with the `servicebroker` executable file.
 - etc: Contains the `carpc` subdirectory with configuration files for `dlt` and `servicebroker`.
 - include: Contains the `carpc` subdirectory with headers that provide the method descriptions for CARPC.
 - lib: Contains the `carpc` subdirectory with CARPC libraries.

The installation path for the artifacts can be changed by adding the parameter --destination=<path>, where <path> is the path to the desired installation directory.
