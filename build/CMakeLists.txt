cmake_minimum_required( VERSION 3.16 FATAL_ERROR )



install(
   FILES ${ROOT_SOURCE_DIR}/build.sh
   DESTINATION bin/build-system
)

install(
   FILES ${ROOT_SOURCE_DIR}/CMakeLists.txt
   DESTINATION bin/build-system
)

install(
   FILES ${ROOT_SOURCE_DIR}/CMakeLists-compiler_definitions.txt
   DESTINATION bin/build-system
)

install(
   FILES ${ROOT_SOURCE_DIR}/Android.bp
   DESTINATION bin/build-system
)

install(
   DIRECTORY ${ROOT_SOURCE_DIR}/build/shell
   DESTINATION bin/build-system/build
)

install(
   DIRECTORY ${ROOT_SOURCE_DIR}/build/cmake
   DESTINATION bin/build-system/build
   PATTERN CMakeLists-projects.txt EXCLUDE
)
install(
   FILES ${ROOT_SOURCE_DIR}/build/cmake/CMakeLists-projects.txt
   DESTINATION bin/build-system
)

install(
   DIRECTORY ${ROOT_SOURCE_DIR}/build/generator
   DESTINATION bin/build-system/build
)

install(
   DIRECTORY ${ROOT_SOURCE_DIR}/thirdparty
   DESTINATION bin/build-system
)
