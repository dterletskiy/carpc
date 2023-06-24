cmake_minimum_required( VERSION 3.16 FATAL_ERROR )



carpc_project( BUILD_SYSTEM )

install(
   FILES ${ROOT_SOURCE_DIR}/CMakeLists.txt
   DESTINATION bin/build-system
)

install(
   FILES ${ROOT_SOURCE_DIR}/Android.bp
   DESTINATION bin/build-system
)

install(
   DIRECTORY ${ROOT_SOURCE_DIR}/build/cmake
   DESTINATION bin/build-system/build
   PATTERN projects.cmake EXCLUDE
)
install(
   FILES ${ROOT_SOURCE_DIR}/build/cmake/projects.cmake
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
