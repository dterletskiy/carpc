function set_compiler( )
{
   local LOCAL_COMPILER=${1}
   case ${LOCAL_COMPILER} in
      "gcc")
         print_info "compiler: GCC"
         export CC=/usr/bin/cc
         export CXX=/usr/bin/c++
         # export CXX=/usr/bin/g++
      ;;
      "clang")
         print_info "compiler: Clang"
         export CC=/usr/bin/clang
         export CXX=/usr/bin/clang++
      ;;
      *)
         print_info "compiler: Default"
      ;;
   esac
}

function setup( )
{
   PROJECT_CONFIG[ROOT_DIR]=${PWD}
   PROJECT_CONFIG[ROOT_DIR_NAME]=${PWD##*/}
   PROJECT_CONFIG[SOURCE_DIR]=${PROJECT_CONFIG[ROOT_DIR]}
   PROJECT_CONFIG[PRODUCT_DIR]=${PROJECT_CONFIG[ROOT_DIR]}/../${PROJECT_CONFIG[ROOT_DIR_NAME]}_product/${TARGET_CONFIG[OS]}-${TARGET_CONFIG[ARCH]}
   PROJECT_CONFIG[GEN_DIR]=${PROJECT_CONFIG[PRODUCT_DIR]}/gen
   PROJECT_CONFIG[BUILD_DIR]=${PROJECT_CONFIG[PRODUCT_DIR]}/build
   PROJECT_CONFIG[DELIVERY_DIR]=${PROJECT_CONFIG[PRODUCT_DIR]}/delivery
   PROJECT_CONFIG[DOCUMENTATION_DIR]=${PROJECT_CONFIG[PRODUCT_DIR]}/documentation
   PROJECT_CONFIG[CMAKE_PARAMETERS]=" \
         -DOS_ANDROID:STRING=${OS_MAP[android]} \
         -DOS_LINUX:STRING=${OS_MAP[linux]} \
         -DOS_TARGET:STRING=${OS_MAP[${TARGET_CONFIG[OS]}]} \
      "
   if [ ${TARGET_CONFIG[OS]} == "android" ]; then
      PROJECT_CONFIG[ENVIRONMENT_SETUP]=${ENVIRONMENT_SETUP_ANDROID}
   fi
}

function setup_sdk( )
{
   local SDK=${1}

   # Check if 'SDK' is set
   # https://stackoverflow.com/questions/3601515/how-to-check-if-a-variable-is-set-in-bash
   if [ -z ${SDK+x} ]; then
      print_info "SDK is unset";
   elif [ -z ${SDK} ]; then
      print_info "SDK is empty";
   else
      print_info "SDK is set to '${SDK}'";
      if [ ! -f "${SDK}" ]; then
         print_error "'${SDK}' file does not exist"
         exit 1
      fi
      source ${SDK}
   fi
}

function print_project_info( )
{
   echo "TARGET ARCH:         ${TARGET_CONFIG[ARCH]}"
   echo "TARGET OS:           ${TARGET_CONFIG[OS]}"

   echo "ROOT_DIR:            ${PROJECT_CONFIG[ROOT_DIR]}"
   echo "ROOT_DIR_NAME:       ${PROJECT_CONFIG[ROOT_DIR_NAME]}"
   echo "SOURCE_DIR:          ${PROJECT_CONFIG[SOURCE_DIR]}"
   echo "PRODUCT_DIR:         ${PROJECT_CONFIG[PRODUCT_DIR]}"
   echo "BUILD_DIR:           ${PROJECT_CONFIG[BUILD_DIR]}"
   echo "DELIVERY_DIR:        ${PROJECT_CONFIG[DELIVERY_DIR]}"
   echo "DOCUMENTATION_DIR:   ${PROJECT_CONFIG[DOCUMENTATION_DIR]}"
   echo "CMAKE_PARAMETERS:    ${PROJECT_CONFIG[CMAKE_PARAMETERS]}"
   echo "ENVIRONMENT_SETUP:   ${PROJECT_CONFIG[ENVIRONMENT_SETUP]}"
   echo "CARPC:               ${PROJECT_CONFIG[CARPC]}"

   echo "AR:                  ${AR}"
   echo "AS:                  ${AS}"
   echo "CC:                  ${CC}"
   echo "CXX:                 ${CXX}"
   echo "LD:                  ${LD}"
   echo "RANLIB:              ${RANLIB}"
   echo "STRIP:               ${STRIP}"
}

function clean( )
{
   rm -R ${PROJECT_CONFIG[PRODUCT_DIR]}
}

function configure( )
{
   print_info "cmake configuration"
   cmake \
      -S ${PROJECT_CONFIG[SOURCE_DIR]} \
      -B ${PROJECT_CONFIG[BUILD_DIR]} \
      -DCMAKE_INSTALL_PREFIX=${PROJECT_CONFIG[DELIVERY_DIR]} \
      --graphviz=${PROJECT_CONFIG[DOCUMENTATION_DIR]}/graph \
      ${PROJECT_CONFIG[CMAKE_PARAMETERS]} \
      \
      -DROOT_SOURCE_DIR=${PROJECT_CONFIG[SOURCE_DIR]} \
      -DROOT_GEN_DIR=${PROJECT_CONFIG[GEN_DIR]} \
      -DROOT_BUILD_DIR=${PROJECT_CONFIG[BUILD_DIR]} \
      -DROOT_DELIVERY_DIR=${PROJECT_CONFIG[DELIVERY_DIR]} \
      -DROOT_DOCUMENTATION_DIR=${PROJECT_CONFIG[DOCUMENTATION_DIR]} \
      -DCARPC_INSTALL_DIR=${PROJECT_CONFIG[DELIVERY_DIR]} \
      -DCARPC_INCLUDE_DIR=${PROJECT_CONFIG[CARPC]}"/include" \
      -DCARPC_LIB_DIR=${PROJECT_CONFIG[CARPC]}"/lib" \
      -DPFW=${PROJECT_CONFIG[PFW]}

   local RESULT=$?
   if [ 0 -eq ${RESULT} ]; then
      print_ok "cmake configuration result: " ${RESULT}
   else
      print_error "cmake configuration result: " ${RESULT}
      exit ${RESULT}
   fi
   return ${RESULT}
}

function build( )
{
   print_info "cmake build"
   # cmake --build ${PROJECT_CONFIG[BUILD_DIR]} --target help
   # cmake --build ${PROJECT_CONFIG[BUILD_DIR]} --target ${ACTION[TARGET]} -j ${CORES}
   cmake --build ${PROJECT_CONFIG[BUILD_DIR]} -j ${CORES}

   local RESULT=$?
   if [ 0 -eq ${RESULT} ]; then
      print_ok "cmake build result: " ${RESULT}
   else
      print_error "cmake build result: " ${RESULT}
      exit ${RESULT}
   fi
   return ${RESULT}
}

function install( )
{
   print_info "cmake install"
   cmake --build ${PROJECT_CONFIG[BUILD_DIR]} --target install
   # cmake --install ${PROJECT_CONFIG[BUILD_DIR]} --prefix ${PROJECT_CONFIG[DELIVERY_DIR]}

   local RESULT=$?
   if [ 0 -eq ${RESULT} ]; then
      print_ok "cmake install result: " ${RESULT}
   else
      print_error "cmake install result: " ${RESULT}
      exit ${RESULT}
   fi
   return ${RESULT}
}

function gpb_generate( )
{
   local SOURCE_DIR=${1}
   local DESTINATION_DIR=${2}
   local PROTO_FILE=${3}

   protoc -I=${SOURCE_DIR} --cpp_out=${DESTINATION_DIR} ${SOURCE_DIR}/${PROTO_FILE}
}

function adb_install( )
{
   if [[ -z ${1+x} ]]; then
      echo "Set installation path"
      return 255
   fi

   local LOCAL_INSTALL_PATH=${1}

   echo ${ABD_TOOL} root
   echo ${ABD_TOOL} shell mkdir -p ${LOCAL_INSTALL_PATH}
   echo ${ABD_TOOL} push ${PROJECT_CONFIG[DELIVERY_DIR]}/bin/* ${LOCAL_INSTALL_PATH}
   echo ${ABD_TOOL} shell chmod +x ${LOCAL_INSTALL_PATH}/*
   echo ${ABD_TOOL} push ${PROJECT_CONFIG[DELIVERY_DIR]}/lib/* ${LOCAL_INSTALL_PATH}
   echo ${ABD_TOOL} push ${PROJECT_CONFIG[DELIVERY_DIR]}/etc/* ${LOCAL_INSTALL_PATH}
   echo ${ABD_TOOL} push ${SYSROOT}/usr/lib/${TARGET}/libc++_shared.so ${LOCAL_INSTALL_PATH}
}
