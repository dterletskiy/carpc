#!/usr/bin/env bash



# Fetch predefined project ----------------------------------+
# Fetch destination directory ------------+                  |
# Fetch action ---------+                 |                  |
#                       |                 |                  |
#                       |                 |                  |
#                       |                 |                  |
# ./do_exp.sh --action=fetch  --source=builder     --target=builder
# ./do_exp.sh --action=fetch  --source=framework   --target=framework
# ./do_exp.sh --action=fetch  --source=examples    --target=examples
# ./do_exp.sh --action=fetch  --source=tutorial    --target=tutorial

# Fetch predefined project ----------------------------------+
# Project source directory to config -----+                  |
# Config action --------+                 |                  |
#                       |                 |                  |
#                       |                 |                  |
#                       |                 |                  |
# ./do_exp.sh --action=config --source=framework   --builder=builder/carpc-builder
# ./do_exp.sh --action=config --source=examples    --builder=builder/carpc-builder  --carpc_api=framework/_product_/deploy/include  --carpc_lib=framework/_product_/deploy/lib/carpc

# List of the targtes to build ------------------------------+
# Project source directory to build ------+                  |
# Build action ---------+                 |                  |
#                       |                 |                  |
#                       |                 |                  |
#                       |                 |                  |
# ./do_exp.sh --action=build  --source=framework
# ./do_exp.sh --action=build  --source=framework   --target=all
# ./do_exp.sh --action=build  --source=framework   --target=carpc-tracing,carpc-tools

# Project source directory to clean ------+                  |
# Clean action ---------+                 |                  |
#                       |                 |                  |
#                       |                 |                  |
#                       |                 |                  |
# ./do_exp.sh --action=clean  --source=framework
# ./do_exp.sh --action=pure   --source=framework



readonly SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
readonly TIMESTAMP=$(date +'%Y.%m.%d_%H.%M.%S')

readonly SHELL_FW=${SCRIPT_DIR}/shell_fw/
if [ ! -d "${SHELL_FW}/.git" ]; then
   git clone "https://github.com/dterletskiy/shell_fw.git" ${SHELL_FW}
   RETURN_CODE=$?
   if [ 0 -ne ${RETURN_CODE} ]; then
      echo "'shell framework' clone error."
      exit ${RETURN_CODE}
   fi
fi
source ${SHELL_FW}/global.sh



CARPC_API_DIR=""
CARPC_LIB_DIR=""

define_optional_argument "test" \
   --allowed="value_1 value_2 value_3" \
   --default="value_1 value_2"

define_required_argument "action" \
   --allowed="fetch clean pure config build install world run"

define_optional_argument "target"

define_optional_argument "source" \
   --default="."

define_optional_argument "destination"

define_optional_argument "builder" \
   --default="."

define_optional_argument "params"

define_optional_argument "jobs" \
   --default="8"

define_optional_argument "compiler" \
   --allowed="gnu clang" \
   --default="gnu"

define_optional_argument "os" \
   --allowed="linux android" \
   --default="linux"

define_optional_argument "sys_trace" \
   --allowed="yes no" \
   --default="yes"

define_optional_argument "msg_trace" \
   --allowed="yes no" \
   --default="yes"

define_optional_argument "colored_trace" \
   --allowed="yes no" \
   --default="yes"

define_optional_argument "dlt" \
   --allowed="yes no" \
   --default="yes"

define_optional_argument "gpb" \
   --allowed="yes no" \
   --default="yes"

define_optional_argument "rtti" \
   --allowed="yes no" \
   --default="yes"

define_optional_argument "debug" \
   --allowed="yes no" \
   --default="yes"

define_optional_argument "debug_stream" \
   --allowed="yes no" \
   --default="yes"

define_optional_argument "memory_hook" \
   --allowed="yes no" \
   --default="no"

define_optional_argument "instrumental" \
   --allowed="yes no" \
   --default="no"

define_optional_argument "carpc_api" \
   --default="/mnt/host/tda/_experimental_/framework/_product_/deploy/include/"

define_optional_argument "carpc_lib" \
   --default="/mnt/host/tda/_experimental_/framework/_product_/deploy/lib/carpc/"



declare -A DIRECTORIES=( )

function init_directories( )
{
   local LOCAL_SOURCE_DIR=${1}
   local -n LOCAL_DIRECTORIES_REF=${2}

   LOCAL_DIRECTORIES_REF[source]=${LOCAL_SOURCE_DIR}
   LOCAL_DIRECTORIES_REF[product]=${LOCAL_SOURCE_DIR}/_product_/
   LOCAL_DIRECTORIES_REF[build]=${LOCAL_DIRECTORIES_REF[product]}/build/
   LOCAL_DIRECTORIES_REF[gen]=${LOCAL_DIRECTORIES_REF[product]}/gen/
   LOCAL_DIRECTORIES_REF[deploy]=${LOCAL_DIRECTORIES_REF[product]}/deploy/
   LOCAL_DIRECTORIES_REF[doc]=${LOCAL_DIRECTORIES_REF[product]}/doc/
}



function print_repositories( )
{
   local LOCAL_LIST_NAME=${1}
   local -n LOCAL_LIST_REF=${LOCAL_LIST_NAME}

   for PROJECT_NAME in "${LOCAL_LIST_REF[@]}"; do
      declare -n PROJECT=${PROJECT_NAME}
      print_ok "Repository: ${PROJECT_NAME}"
      for key in "${!PROJECT[@]}"; do
         echo "   $key: ${PROJECT[$key]}"
      done
   done   
}

function init_repositories_framework( )
{
   local LOCAL_LIST_NAME=${1}
   local -n LOCAL_LIST_REF=${LOCAL_LIST_NAME}

   declare -g -A REPO_TRACING=(
         [url]="git@github.com:dterletskiy/carpc-tracing.git"
         [branch]="main"
         [directory]="carpc-tracing"
      )
   declare -g -A REPO_BASE=(
         [url]="git@github.com:dterletskiy/carpc-base.git"
         [branch]="main"
         [directory]="carpc-base"
      )
   declare -g -A REPO_TOOLS=(
         [url]="git@github.com:dterletskiy/carpc-tools.git"
         [branch]="main"
         [directory]="carpc-tools"
      )
   declare -g -A REPO_OSW=(
         [url]="git@github.com:dterletskiy/carpc-osw.git"
         [branch]="main"
         [directory]="carpc-osw"
      )
   declare -g -A REPO_RUNTIME=(
         [url]="git@github.com:dterletskiy/carpc-runtime.git"
         [branch]="main"
         [directory]="carpc-runtime"
      )
   declare -g -A REPO_SERVICEBROCKER=(
         [url]="git@github.com:dterletskiy/carpc-servicebrocker.git"
         [branch]="main"
         [directory]="carpc-servicebrocker"
      )

   LOCAL_LIST_REF=( )
   LOCAL_LIST_REF+=( REPO_TRACING )
   LOCAL_LIST_REF+=( REPO_BASE )
   LOCAL_LIST_REF+=( REPO_TOOLS )
   LOCAL_LIST_REF+=( REPO_OSW )
   LOCAL_LIST_REF+=( REPO_RUNTIME )
   LOCAL_LIST_REF+=( REPO_SERVICEBROCKER )
}

function init_repositories_builder( )
{
   local LOCAL_LIST_NAME=${1}
   local -n LOCAL_LIST_REF=${LOCAL_LIST_NAME}

   declare -g -A REPO_BUILDER=(
         [url]="git@github.com:dterletskiy/carpc-builder.git"
         [branch]="main"
         [directory]="carpc-builder"
      )

   LOCAL_LIST_REF=( )
   LOCAL_LIST_REF+=( REPO_BUILDER )
}

function init_repositories_examples( )
{
   local LOCAL_LIST_NAME=${1}
   local -n LOCAL_LIST_REF=${LOCAL_LIST_NAME}

   declare -g -A REPO_EXAMPLES=(
         [url]="git@github.com:dterletskiy/carpc-examples.git"
         [branch]="scorpius"
         [directory]="carpc-examples"
      )

   LOCAL_LIST_REF=( )
   LOCAL_LIST_REF+=( REPO_EXAMPLES )
}

function init_repositories_tutorial( )
{
   local LOCAL_LIST_NAME=${1}
   local -n LOCAL_LIST_REF=${LOCAL_LIST_NAME}

   declare -g -A REPO_TUTORIAL=(
         [url]="git@github.com:dterletskiy/carpc-tutorial.git"
         [branch]="scorpius"
         [directory]="carpc-tutorial"
      )

   LOCAL_LIST_REF=( )
   LOCAL_LIST_REF+=( REPO_TUTORIAL )
}

declare -a PROJECTS_LIST=(
   "framework"
   "builder"
   "examples"
   "tutorial"
)

function fetch( )
{
   local LOCAL_TARGET=${1}

   found=0
   for item in "${PROJECTS_LIST[@]}"; do
      print_info "Processing item '${item}'"
      if [[ "${item}" == "${LOCAL_TARGET}" ]]; then
         print_info "'${item}' == '${LOCAL_TARGET}'"
         found=1
         break
      fi
   done

   if [[ ${found} -ne 1 ]]; then
      print_error "target for fetch action is not defined or defined invalid"
      exit 1
   fi

   declare -a REPOSITORIES=( )
   execute "init_repositories_${LOCAL_TARGET} REPOSITORIES"
   print_repositories REPOSITORIES

   mkdir -p ${DIRECTORIES[source]}

   SOURCE_CMAKE_FILE="${DIRECTORIES[source]}/CMakeLists.txt"
   if [ "builder" != ${LOCAL_TARGET} ]; then
      if [ -f ${SOURCE_CMAKE_FILE} ]; then
         print_error "Directory is not empty => repositories can't be cloned"
         exit 2
      fi
   fi

   if [ "builder" != ${LOCAL_TARGET} ]; then
      execute "echo \"cmake_minimum_required( VERSION 3.16 FATAL_ERROR )\" > ${SOURCE_CMAKE_FILE}"
   fi
   for REPOSITORY_NAME in "${REPOSITORIES[@]}"; do
      declare -n REPOSITORY=${REPOSITORY_NAME}
      execute "git clone --recursive -b ${REPOSITORY[branch]} \
         ${REPOSITORY[url]} \
         \"${DIRECTORIES[source]}/${REPOSITORY[directory]}\" \
      "
      if [ "builder" != ${LOCAL_TARGET} ]; then
         execute "echo \"fenix_add_subdirectory( ${REPOSITORY[directory]} )\" >> ${SOURCE_CMAKE_FILE}"
      fi
   done
}



function define_compiler_by_type( )
{
   local LOCAL_COMPILER_TYPE=${1}
   local -n LOCAL_COMPILER=${2}

   local COMPILER_PATH="/usr/bin/"

   if [[ "${LOCAL_COMPILER_TYPE}" == "clang" ]]; then
      LOCAL_COMPILER["c"]="clang"
      LOCAL_COMPILER["cxx"]="clang++"
   elif [[ "${LOCAL_COMPILER_TYPE}" == "gnu" ]]; then
      LOCAL_COMPILER["c"]="gcc"
      LOCAL_COMPILER["cxx"]="g++"
   else
      print_error "Undefined compiler type '${LOCAL_COMPILER_TYPE}'"
      exit 1
   fi

   LOCAL_COMPILER["c"]="${COMPILER_PATH}${LOCAL_COMPILER["c"]}"
   LOCAL_COMPILER["cxx"]="${COMPILER_PATH}${LOCAL_COMPILER["cxx"]}"

   # export CC=${LOCAL_COMPILER["c"]}
   # export CXX=${LOCAL_COMPILER["cxx"]}
}

function update_build_variables( )
{
   declare -A PROJECT_COMPILER=( )
   COMPILER_TYPE=$( get_parameter_value "compiler" )
   define_compiler_by_type ${COMPILER_TYPE} PROJECT_COMPILER

   LOCAL_BUILD_VARIABLES=""
   LOCAL_BUILD_VARIABLES+=" -D ROOT_GEN_DIR:STRING=${DIRECTORIES[gen]}"
   LOCAL_BUILD_VARIABLES+=" -D TARGET_OS:STRING=$( get_parameter_value "os" )"
   LOCAL_BUILD_VARIABLES+=" -D DLT_TRACE:STRING=$( get_parameter_value "dlt" )"
   LOCAL_BUILD_VARIABLES+=" -D SYS_TRACE:STRING=$( get_parameter_value "sys_trace" )"
   LOCAL_BUILD_VARIABLES+=" -D MSG_TRACE:STRING=$( get_parameter_value "msg_trace" )"
   LOCAL_BUILD_VARIABLES+=" -D COLORED_TRACE:STRING=$( get_parameter_value "colored_trace" )"
   LOCAL_BUILD_VARIABLES+=" -D DEBUG_STREAM:STRING=$( get_parameter_value "debug_stream" )"
   LOCAL_BUILD_VARIABLES+=" -D MEMORY_HOOK:STRING=$( get_parameter_value "memory_hook" )"
   LOCAL_BUILD_VARIABLES+=" -D INSTRUMENTAL:STRING=$( get_parameter_value "instrumental" )"
   LOCAL_BUILD_VARIABLES+=" -D USE_DEBUG:STRING=$( get_parameter_value "debug" )"
   LOCAL_BUILD_VARIABLES+=" -D USE_GPB:STRING=$( get_parameter_value "gpb" )"
   LOCAL_BUILD_VARIABLES+=" -D USE_RTTI:STRING=$( get_parameter_value "rtti" )"
   LOCAL_BUILD_VARIABLES+=" -D CMAKE_C_COMPILER:STRING=${PROJECT_COMPILER["c"]}"
   LOCAL_BUILD_VARIABLES+=" -D CMAKE_CXX_COMPILER:STRING=${PROJECT_COMPILER["cxx"]}"
   LOCAL_BUILD_VARIABLES+=" -D CMAKE_VERBOSE_MAKEFILE=TRUE"
   LOCAL_BUILD_VARIABLES+=" -D SOURCE_DIR=${DIRECTORIES[source]}"
   LOCAL_BUILD_VARIABLES+=" -D CARPC_API=$( get_parameter_value_path "carpc_api" )"
   LOCAL_BUILD_VARIABLES+=" -D CARPC_LIB=$( get_parameter_value_path "carpc_lib" )"
   echo ${LOCAL_BUILD_VARIABLES}
}

function config( )
{
   execute "cmake \
      -S $( get_parameter_value_path "builder" ) \
      -B ${DIRECTORIES[build]} \
      --install-prefix ${DIRECTORIES[deploy]} \
      --graphviz=${DIRECTORIES[doc]}/graph/project \
      $( update_build_variables ) \
   "
}

function build( )
{
   local LOCAL_TARGET=${1}
   if [ -z ${LOCAL_TARGET+x} ]; then
      PARAMETER_TARGET=""
   elif [ -z ${LOCAL_TARGET} ]; then
      PARAMETER_TARGET=""
   else
      PARAMETER_TARGET="--target ${LOCAL_TARGET}"
   fi

   COMMAND="cmake --build ${DIRECTORIES[build]} --verbose -j$( get_parameter_value "jobs" 0 ) ${PARAMETER_TARGET}"
   execute ${COMMAND}
}

function install( )
{
   local LOCAL_DESTINATION=${1}
   if [ -z ${LOCAL_DESTINATION+x} ]; then
      PARAMETER_DESTINATION="--prefix ${DIRECTORIES[deploy]}"
   elif [ -z ${LOCAL_DESTINATION} ]; then
      PARAMETER_DESTINATION="--prefix ${DIRECTORIES[deploy]}"
   else
      PARAMETER_DESTINATION="--prefix ${LOCAL_DESTINATION}"
   fi

   COMMAND="cmake --install ${DIRECTORIES[build]} ${PARAMETER_DESTINATION}"
   execute ${COMMAND}

   # build "install"
}

function clean( )
{
   build "clean"
}

function pure( )
{
   rm -rf \
      ${DIRECTORIES[build]}/CMakeFiles \
      ${DIRECTORIES[build]}/CMakeCache.txt \
      ${DIRECTORIES[build]}/Makefile \
      ${DIRECTORIES[build]}/install_manifest.txt \
      ${DIRECTORIES[build]}/cmake_install.cmake
   rm -fr ${DIRECTORIES[product]}
}

function run( )
{
   LOCAL_TARGET=${1}
   shift
   LOCAL_OPTIONS=${@}

   export LD_LIBRARY_PATH="${DIRECTORIES[deploy]}/lib/;$( get_parameter_value "carpc_lib" )"
   ${DIRECTORIES[deploy]}/bin/${LOCAL_TARGET} \
      --config=${DIRECTORIES[deploy]}/etc/${LOCAL_TARGET}.cfg ${LOCAL_OPTIONS}
}



function get_parameter_value_path( )
{
   local LOCAL_PARAMETER_VALUE=${1}
   local LOCAL_PATH=$( get_parameter_value ${LOCAL_PARAMETER_VALUE} )
   echo $( adapt_path ${LOCAL_PATH} )
}

function adapt_path( )
{
   local LOCAL_PATH=${1}

   if [[ "$(realpath "${LOCAL_PATH}")" == "${LOCAL_PATH}" ]]; then
      # print_warning "'--${NAME}' is defined as absolute path => '${LOCAL_PATH}' will be used"
      :
   else
      # print_warning "'--${NAME}' is defined as relative path => current path + '${LOCAL_PATH}' will be used"
      LOCAL_PATH=${PWD}/${LOCAL_PATH}
   fi

   LOCAL_PATH=$(readlink -m "${LOCAL_PATH}")/
   echo ${LOCAL_PATH}
}



function main( )
{
   parse_arguments "$@"

   SOURCE_DIR=$( get_parameter_value_path "source" )
   init_directories ${SOURCE_DIR} DIRECTORIES
   print_map DIRECTORIES

   for ACTION in "${CMD_ACTION_DEFINED_VALUES[@]}"; do
      case ${ACTION} in
         fetch)
            for TARGET in "${CMD_TARGET_DEFINED_VALUES[@]}"; do
               fetch ${TARGET}
            done
         ;;
         config)
            config
         ;;
         build)
            build "${CMD_TARGET_DEFINED_VALUES[*]}"
         ;;
         install)
            install ${CMD_DESTINATION_DIR}
         ;;
         clean)
            clean
         ;;
         pure)
            pure
         ;;
         world)
            pure
            config
            build "${CMD_TARGET_DEFINED_VALUES[*]}"
            install ${CMD_DESTINATION_DIR}
         ;;
         run)
            run $( get_parameter_value "target" ) "$( get_parameter_value "params" )"
         ;;
         *)
            print_warning "undefined action: '${ACTION}'"
            exit 1
         ;;
      esac
   done
}

main "$@"
