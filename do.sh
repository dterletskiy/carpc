#!/usr/bin/env bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

SHELL_FW=${SCRIPT_DIR}/submodules/dterletskiy/shell_fw/
source ${SHELL_FW}/constants/console.sh
source ${SHELL_FW}/constants/constants.sh
source ${SHELL_FW}/base.sh
source ${SHELL_FW}/print.sh
source ${SHELL_FW}/ui.sh
source ${SHELL_FW}/drive.sh



declare -A DEFAULT=(
      # '--compiler' parameter
      [COMPILER]="gnu"
      # '--os' parameter
      [OS]="linux"
      # '--sys_trace' option
      [SYS_TRACE]="yes"
      # '--msg_trace' option
      [MSG_TRACE]="yes"
      # '--colored_trace' option
      [COLORED_TRACE]="yes"
      # '--dlt' option
      [DLT]="yes"
      # '--gpb' option
      [GPB]="yes"
      # '--rtti' option
      [RTTI]="yes"
      # '--memory_hook' option
      [MEMORY_HOOK]="no"
      # '--instrumental' option
      [INSTRUMENTAL]="no"
      # '--debug' option
      [DEBUG]="no"
      # '--debug_stream' option
      [DEBUG_STREAM]="no"
   )



ROOT_DIR=${SCRIPT_DIR}
SOURCE_DIR=${ROOT_DIR}/
REPOS_DIR=${SOURCE_DIR}/repos/
PRODUCT_DIR=${ROOT_DIR}/_product_/
BUILD_DIR=${PRODUCT_DIR}/build/
GEN_DIR=${PRODUCT_DIR}/gen/
INSTALL_DIR=${PRODUCT_DIR}/deploy/
DOC_DIR=${PRODUCT_DIR}/doc/



function define_compiler( )
{
   local LOCAL_COMPILER_TYPE=${1}
   local -n LOCAL_COMPILER=${2}

   local COMPILER_PATH="/usr/bin/"

   if [[ "${LOCAL_COMPILER_TYPE}" == "clang" ]]; then
      LOCAL_COMPILER["c"]="clang"
      LOCAL_COMPILER["cxx"]="clang++"
   elif [[ "${LOCAL_COMPILER_TYPE}" == "gnu" ]]; then
      LOCAL_COMPILER["c"]="cc"
      LOCAL_COMPILER["cxx"]="c++"
   else
      print_error "Undefined compiler type '${LOCAL_COMPILER_TYPE}'"
      exit 1
   fi

   LOCAL_COMPILER["c"]="${COMPILER_PATH}${LOCAL_COMPILER["c"]}"
   LOCAL_COMPILER["cxx"]="${COMPILER_PATH}${LOCAL_COMPILER["cxx"]}"

   # export CC=${LOCAL_COMPILER["c"]}
   # export CXX=${LOCAL_COMPILER["cxx"]}
}

function fetch( )
{
   declare -A PROJECTS=(
      ["carpc-builder"]="main"
      ["carpc-tracing"]="main"
      ["carpc-base"]="main"
      ["carpc-tools"]="main"
      ["carpc-osw"]="main"
      ["carpc-runtime"]="main"
      ["carpc-hooks"]="main"
      ["carpc-instrumental"]="main"
      ["carpc-servicebrocker"]="main"
      ["carpc-examples"]="scorpius"
      ["carpc-tutorial"]="scorpius"
   )
   

   for PROJECT in ${!PROJECTS[@]}; do
      local BRANCH=${PROJECTS[${PROJECT}]}
      git clone --recursive -b ${BRANCH} "git@github.com:dterletskiy/${PROJECT}.git" "${REPOS_DIR}/${PROJECT}"
   done
}

function update_build_variables( )
{
   declare -A PROJECT_COMPILER=( )
   define_compiler ${CMD_COMPILER} PROJECT_COMPILER

   LOCAL_BUILD_VARIABLES=""
   LOCAL_BUILD_VARIABLES+=" -D ROOT_GEN_DIR:STRING=${GEN_DIR}"
   LOCAL_BUILD_VARIABLES+=" -D TARGET_OS:STRING=${CMD_OS}"
   LOCAL_BUILD_VARIABLES+=" -D DLT_TRACE:STRING=${CMD_DLT}"
   LOCAL_BUILD_VARIABLES+=" -D SYS_TRACE:STRING=${CMD_SYS_TRACE}"
   LOCAL_BUILD_VARIABLES+=" -D MSG_TRACE:STRING=${CMD_MSG_TRACE}"
   LOCAL_BUILD_VARIABLES+=" -D COLORED_TRACE:STRING=${CMD_COLORED_TRACE}"
   LOCAL_BUILD_VARIABLES+=" -D DEBUG_STREAM:STRING=${CMD_DEBUG_STREAM}"
   LOCAL_BUILD_VARIABLES+=" -D MEMORY_HOOK:STRING=${CMD_MEMORY_HOOK}"
   LOCAL_BUILD_VARIABLES+=" -D INSTRUMENTAL:STRING=${CMD_INSTRUMENTAL}"
   LOCAL_BUILD_VARIABLES+=" -D USE_DEBUG:STRING=${CMD_DEBUG}"
   LOCAL_BUILD_VARIABLES+=" -D USE_GPB:STRING=${CMD_GPB}"
   LOCAL_BUILD_VARIABLES+=" -D USE_RTTI:STRING=${CMD_RTTI}"
   LOCAL_BUILD_VARIABLES+=" -D CMAKE_C_COMPILER:STRING=${PROJECT_COMPILER["c"]}"
   LOCAL_BUILD_VARIABLES+=" -D CMAKE_CXX_COMPILER:STRING=${PROJECT_COMPILER["cxx"]}"
   echo ${LOCAL_BUILD_VARIABLES}
}

function config( )
{
   BUILD_VARIABLES=$( update_build_variables )

   cmake \
      -S ${SOURCE_DIR} \
      -B ${BUILD_DIR} \
      --install-prefix ${INSTALL_DIR} \
      --graphviz=${DOC_DIR}/graph/project \
      ${BUILD_VARIABLES}
}

function build( )
{
   LOCAL_TARGET=${1}
   if [ -z ${LOCAL_TARGET+x} ]; then
      PARAMETER_TARGET=""
   elif [ -z ${LOCAL_TARGET} ]; then
      PARAMETER_TARGET=""
   else
      PARAMETER_TARGET="--target ${LOCAL_TARGET}"
   fi

   cmake --build ${BUILD_DIR} -j8 ${PARAMETER_TARGET}
}

function install( )
{
   build "install"
}

function clean( )
{
   build "clean"
}

function pure( )
{
   rm -rf \
      ${BUILD_DIR}/CMakeFiles \
      ${BUILD_DIR}/CMakeCache.txt \
      ${BUILD_DIR}/Makefile \
      ${BUILD_DIR}/install_manifest.txt \
      ${BUILD_DIR}/cmake_install.cmake
   rm -fr ${PRODUCT_DIR}
}

function run( )
{
   LOCAL_TARGET=${1}
   shift
   LOCAL_OPTIONS=${@}

   export LD_LIBRARY_PATH="${INSTALL_DIR}/lib/"
   ${INSTALL_DIR}/bin/${LOCAL_TARGET} --config=${INSTALL_DIR}/etc/${LOCAL_TARGET}.cfg ${LOCAL_OPTIONS}
}

function validate_parameters( )
{
   if [ -z ${CMD_ACTION+x} ]; then
      print_error "'--action' is not set"
      exit 1
   fi

   if [ -z ${CMD_TARGET+x} ]; then
      print_warning "'--target' is not set"
   fi

   if [ -z ${CMD_COMPILER+x} ]; then
      print_warning "'--compiler' is not set => '${DEFAULT[COMPILER]}' will be used"
      CMD_COMPILER=${DEFAULT[COMPILER]}
   fi

   if [ -z ${CMD_OS+x} ]; then
      print_warning "'--os' is not set => '${DEFAULT[OS]}' will be used"
      CMD_OS=${DEFAULT[OS]}
   fi

   if [ -z ${CMD_SYS_TRACE+x} ]; then
      print_warning "'--sys_trace' is not set => '${DEFAULT[SYS_TRACE]}' will be used"
      CMD_SYS_TRACE=${DEFAULT[SYS_TRACE]}
   else
      CMD_SYS_TRACE="yes"
   fi

   if [ -z ${CMD_MSG_TRACE+x} ]; then
      print_warning "'--msg_trace' is not set => '${DEFAULT[MSG_TRACE]}' will be used"
      CMD_MSG_TRACE=${DEFAULT[MSG_TRACE]}
   else
      CMD_MSG_TRACE="yes"
   fi

   if [ -z ${CMD_COLORED_TRACE+x} ]; then
      print_warning "'--colored_trace' is not set => '${DEFAULT[COLORED_TRACE]}' will be used"
      CMD_COLORED_TRACE=${DEFAULT[COLORED_TRACE]}
   else
      CMD_COLORED_TRACE="yes"
   fi

   if [ -z ${CMD_DLT+x} ]; then
      print_warning "'--dlt' is not set => '${DEFAULT[DLT]}' will be used"
      CMD_DLT=${DEFAULT[DLT]}
   else
      CMD_DLT="yes"
   fi

   if [ -z ${CMD_GPB+x} ]; then
      print_warning "'--gpb' is not set => '${DEFAULT[GPB]}' will be used"
      CMD_GPB=${DEFAULT[GPB]}
   else
      CMD_GPB="yes"
   fi

   if [ -z ${CMD_RTTI+x} ]; then
      print_warning "'--rtti' is not set => '${DEFAULT[RTTI]}' will be used"
      CMD_RTTI=${DEFAULT[RTTI]}
   else
      CMD_RTTI="yes"
   fi

   if [ -z ${CMD_DEBUG+x} ]; then
      print_warning "'--debug' is not set => '${DEFAULT[DEBUG]}' will be used"
      CMD_DEBUG=${DEFAULT[DEBUG]}
   else
      CMD_DEBUG="yes"
   fi

   if [ -z ${CMD_DEBUG_STREAM+x} ]; then
      print_warning "'--debug_stream' is not set => '${DEFAULT[DEBUG_STREAM]}' will be used"
      CMD_DEBUG_STREAM=${DEFAULT[DEBUG_STREAM]}
   else
      CMD_DEBUG_STREAM="yes"
   fi

   if [ -z ${CMD_MEMORY_HOOK+x} ]; then
      print_warning "'--memory_hook' is not set => '${DEFAULT[MEMORY_HOOK]}' will be used"
      CMD_MEMORY_HOOK=${DEFAULT[MEMORY_HOOK]}
   else
      CMD_MEMORY_HOOK="yes"
   fi

   if [ -z ${CMD_INSTRUMENTAL+x} ]; then
      print_warning "'--instrumental' is not set => '${DEFAULT[INSTRUMENTAL]}' will be used"
      CMD_INSTRUMENTAL=${DEFAULT[INSTRUMENTAL]}
   else
      CMD_INSTRUMENTAL="yes"
   fi
}

function parse_arguments( )
{
   print_header "Parsing arguments..."

   for option in "$@"; do
      print_info "Processing option '${option}'"
      case ${option} in
         --action=*)
            if [ -z ${CMD_ACTION+x} ]; then
               CMD_ACTION="${option#*=}"
               shift # past argument=value
               echo "CMD_ACTION: ${CMD_ACTION}"
            else
               print_error "'--action' is already set to '${CMD_ACTION}'"
               exit 1
            fi
         ;;
         --target=*)
            if [ -z ${CMD_TARGET+x} ]; then
               CMD_TARGET="${option#*=}"
               shift # past argument=value
               echo "CMD_TARGET: ${CMD_TARGET}"
            else
               print_error "'--target' is already set to '${CMD_TARGET}'"
               exit 1
            fi
         ;;
         --params=*)
            if [ -z ${CMD_OPTIONS+x} ]; then
               CMD_OPTIONS="${option#*=}"
               shift # past argument=value
               echo "CMD_OPTIONS: ${CMD_OPTIONS}"
            else
               print_error "'--params' is already set to '${CMD_OPTIONS}'"
               exit 1
            fi
         ;;
         --compiler=*)
            if [ -z ${CMD_COMPILER+x} ]; then
               CMD_COMPILER="${option#*=}"
               shift # past argument=value
               echo "CMD_COMPILER: ${CMD_COMPILER}"
            else
               print_error "'--compiler' is already set to '${CMD_COMPILER}'"
               exit 1
            fi
         ;;
         --sys_trace)
            CMD_SYS_TRACE=
            echo "CMD_SYS_TRACE: defined"
         ;;
         --msg_trace)
            CMD_MSG_TRACE=
            echo "CMD_MSG_TRACE: defined"
         ;;
         --colored_trace)
            CMD_COLORED_TRACE=
            echo "CMD_COLORED_TRACE: defined"
         ;;
         --dlt)
            CMD_DLT=
            echo "CMD_DLT: defined"
         ;;
         --gpb)
            CMD_GPB=
            echo "CMD_GPB: defined"
         ;;
         --rtti)
            CMD_RTTI=
            echo "CMD_RTTI: defined"
         ;;
         --debug)
            CMD_DEBUG=
            echo "CMD_DEBUG: defined"
         ;;
         --debug_stream)
            CMD_DEBUG_STREAM=
            echo "CMD_DEBUG_STREAM: defined"
         ;;
         --memory_hook)
            CMD_MEMORY_HOOK=
            echo "CMD_MEMORY_HOOK: defined"
         ;;
         --instrumental)
            CMD_INSTRUMENTAL=
            echo "CMD_INSTRUMENTAL: defined"
         ;;
         *)
            print_error "undefined option: '${option}'"
            shift # past argument=value
            exit 1
         ;;
      esac
   done

   validate_parameters
}

function main( )
{
   parse_arguments "$@"
   print_info "Processing action:" ${CMD_ACTION}

   case ${CMD_ACTION} in
      fetch)
         fetch
      ;;
      clean)
         clean
      ;;
      pure)
         pure
      ;;
      config)
         config
      ;;
      build)
         build ${CMD_TARGET}
      ;;
      install)
         install
      ;;
      world)
         pure
         config
         build
         install
      ;;
      run)
         run ${CMD_TARGET} "${CMD_OPTIONS}"
      ;;
      *)
         print_warning "undefined action: '${CMD_ACTION}'"
         exit 1
      ;;
   esac
}



cd ${ROOT_DIR}
main "$@"
