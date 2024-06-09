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
    git clone https://github.com/dterletskiy/carpc-builder.git <path_to_fenix>
    ```
    where `<path_to_fenix>` is the path where the Fenix build system repository will be cloned.

2. **Clone all CARPC repositories using the auxiliary script:**
    ```sh
    <path_to_fenix>/do.sh --source=<path_to_framework_location> --action=fetch --target=framework
    ```
    where `<path_to_framework_location>` is the path where all CARPC repositories will be cloned.

    After completing these steps, a `CMakeLists.txt` file will be generated in the `<path_to_framework_location>` directory. This file will include all subdirectories containing the CARPC components that were cloned during the previous step.

### Further Information

For more details about the build system and the auxiliary script, please refer to the section [Build system](../guide/build_system.md).