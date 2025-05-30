[Go to contents](../../README.md#table-of_contents)

## Getting sources

### CARPC Source Code Structure

The source code of the CARPC framework is divided into separate projects, each housed in its own repository. Each project can be cloned and built independently, provided it does not have dependencies on other projects. If dependencies are present, the dependent projects must also be cloned and built to ensure successful compilation of the target project. The division into separate projects follows the structure described in the section [Architecture](../about/architecture.md).

### Projects and Repositories

Below is a list of all projects and their respective repositories:

1. **tracing**          - https://github.com/dterletskiy/carpc-tracing

2. **base**             - https://github.com/dterletskiy/carpc-base

3. **osw**              - https://github.com/dterletskiy/carpc-osw

4. **tools**            - https://github.com/dterletskiy/carpc-tools

5. **runtime**          - https://github.com/dterletskiy/carpc-runtime

6. **servicebroker**    - https://github.com/dterletskiy/carpc-servicebroker

For building the CARPC framework, a dedicated **fenix** build system based on CMake is used. This build system includes additional functionality in the form of custom CMake methods, a set of parameters and variables that control the build process, and code generators for source code generation from descriptive interface and component files. The build system is also available in a separate repository:

7. **fenix**            - https://github.com/dterletskiy/carpc-builder

This build system is used not only for building the framework itself but also for applications and projects based on it. It includes an auxiliary script that simplifies the cloning of CARPC repositories and their build processes, as well as the build process for third-party projects using CARPC.

### Cloning CARPC Projects

To clone all constituent projects of the CARPC framework using the Fenix build system, follow these steps:

1. **Clone the build system:**
    ```sh
    mkdir -p <path_to_builder> && cd <path_to_builder>
    git clone https://github.com/dterletskiy/carpc-builder.git
    ```
    where `<path_to_builder>` is the path where the Fenix build system repository will be cloned.

2. **Clone all CARPC repositories using the auxiliary script:**
    This step could be done mabually:
    ```sh
    mkdir -p <path_to_framework> && cd <path_to_framework>
    git clone https://github.com/dterletskiy/carpc-tracing
    git clone https://github.com/dterletskiy/carpc-base
    git clone https://github.com/dterletskiy/carpc-osw
    git clone https://github.com/dterletskiy/carpc-tools
    git clone https://github.com/dterletskiy/carpc-runtime
    git clone https://github.com/dterletskiy/carpc-servicebroker
    ```

3. **Create CMakeLists.txt file**
    On the next step `CMakeLists.txt` file must be create and fullfilled with project directories to be included. This file will be passed to the `Fenix` build system.
    ```sh
    touch CMakeLists.txt
    echo "fenix_add_subdirectory( carpc-tracing )"
    echo "fenix_add_subdirectory( carpc-base )"
    echo "fenix_add_subdirectory( carpc-tools )"
    echo "fenix_add_subdirectory( carpc-osw )"
    echo "fenix_add_subdirectory( carpc-runtime )"
    echo "fenix_add_subdirectory( carpc-servicebrocker )"
    ```

### `carpc.sh` automation helper script.

All the previous steps, along with additional operations, can be performed using a helper script located in the current repository. This script automates the cloning of the build system, framework components, and tutorial examples, as well as their subsequent compilation and installation.

To clone the build system, framework components, and examples, run this script with the following parameters:
```sh
./carpc.sh --action=fetch --source=<dectination> --target=<project>
```
where <destination> is the directory where all repositories related to the project specified as <project> will be cloned, and `CMakeLists.txt` files will be generated if required. This path can be either absolute or relative. **For different projects must be used different directories.**

So, to clone build system, framework and tutorial you can do next:
```sh
./carpc.sh --action=fetch --source=builder --target=builder
./carpc.sh --action=fetch --source=framework --target=framework
./carpc/carpc.sh --action=fetch --source=tutorial --target=tutorial
```


For more details about the build system and the auxiliary script, please refer to the section [Build system](../guide/build_system.md).
