cmake_minimum_required( VERSION 3.16 FATAL_ERROR )



fenix_add_subdirectory( carpc-tracing )
fenix_add_subdirectory( carpc-base )
fenix_add_subdirectory( carpc-tools )
fenix_add_subdirectory( carpc-osw )
fenix_add_subdirectory( carpc-runtime )
fenix_add_subdirectory( carpc-servicebrocker )
fenix_add_subdirectory( carpc-hooks )
fenix_add_subdirectory( carpc-instrumental )

fenix_add_subdirectory( carpc-examples )
fenix_add_subdirectory( carpc-tutorial )
