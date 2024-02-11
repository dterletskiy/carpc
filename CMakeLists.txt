cmake_minimum_required( VERSION 3.16 FATAL_ERROR )



set( FENIX_SUBDIR "./repos/carpc-builder/" )
include( ${FENIX_SUBDIR}/init.cmake )



fenix_project( TARGET fenix PROJECT FENIX )

fenix_add_subdirectory( repos )

fenix_get_all_targets( TARGETS )
generate_dependencies_graph( ${PROJECT_GEN_DIR} ${CMAKE_PROJECT_NAME} "${TARGETS}" )
