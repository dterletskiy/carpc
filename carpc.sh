#!/usr/bin/env bash



# Fetch predefined project ----------------------------------+
# Fetch destination directory ------------+                  |
# Fetch action ---------+                 |                  |
#                       |                 |                  |
#                       |                 |                  |
#                       |                 |                  |
# ./carpc.sh --action=fetch  --source=builder     --target=builder
# ./carpc.sh --action=fetch  --source=framework   --target=framework
# ./carpc.sh --action=fetch  --source=examples    --target=examples
# ./carpc.sh --action=fetch  --source=tutorial    --target=tutorial

# Fetch predefined project ----------------------------------+
# Project source directory to config -----+                  |
# Config action --------+                 |                  |
#                       |                 |                  |
#                       |                 |                  |
#                       |                 |                  |
# ./carpc.sh --action=config --source=framework   --builder=builder/carpc-builder
# ./carpc.sh --action=config --source=examples    --builder=builder/carpc-builder  --carpc_api=framework/_product_/deploy/include  --carpc_lib=framework/_product_/deploy/lib/carpc

# List of the targtes to build ------------------------------+
# Project source directory to build ------+                  |
# Build action ---------+                 |                  |
#                       |                 |                  |
#                       |                 |                  |
#                       |                 |                  |
# ./carpc.sh --action=build  --source=framework
# ./carpc.sh --action=build  --source=framework   --target=all
# ./carpc.sh --action=build  --source=framework   --target=carpc-tracing,carpc-tools

# Project source directory to clean ------+                  |
# Clean action ---------+                 |                  |
#                       |                 |                  |
#                       |                 |                  |
#                       |                 |                  |
# ./carpc.sh --action=clean  --source=framework
# ./carpc.sh --action=pure   --source=framework

# ./carpc.sh --action=run --source=framework --target=servicebrocker



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
source ${SHELL_FW}/__init__



CARPC_API_DIR=""
CARPC_LIB_DIR=""

define_optional_argument "test" \
   --allowed="value_1 value_2 value_3" \
   --default="value_1 value_2"

define_required_argument "action" \
   --allowed="fetch clean pure config build deploy world run"

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

define_optional_argument "trace" \
   --allowed="yes no" \
   --default="yes"

define_optional_argument "rtti" \
   --allowed="yes no" \
   --default="yes"

define_optional_argument "debug" \
   --allowed="yes no" \
   --default="yes"

define_optional_argument "policy_std" \
   --allowed="yes no" \
   --default="no"

define_optional_argument "carpc_api" \
   --default="/mnt/host/tda/_experimental_/framework/_product_/deploy/include/"

define_optional_argument "carpc_lib" \
   --default="/mnt/host/tda/_experimental_/framework/_product_/deploy/lib/carpc/"



declare -A -g DIRECTORIES=( )

function init_directories( )
{
   local LOCAL_SOURCE_DIR=$(get_parameter_value_path "source")

   DIRECTORIES[source]=${LOCAL_SOURCE_DIR}
   DIRECTORIES[product]=${LOCAL_SOURCE_DIR}/_product_/
   DIRECTORIES[build]=${DIRECTORIES[product]}/build/
   DIRECTORIES[gen]=${DIRECTORIES[product]}/gen/
   DIRECTORIES[deploy]=${DIRECTORIES[product]}/deploy/
   DIRECTORIES[doc]=${DIRECTORIES[product]}/doc/

   print_map DIRECTORIES
}



declare -a REPOSITORIES=( )

function print_repositories( )
{
   for PROJECT_NAME in "${REPOSITORIES[@]}"; do
      declare -n PROJECT=${PROJECT_NAME}
      log_debug "Repository: ${PROJECT_NAME}"
      for key in "${!PROJECT[@]}"; do
         log_trace "   $key: ${PROJECT[$key]}"
      done
   done   
}

function init_repositories( )
{
   local LOCAL_TARGET=${1}
   init_repositories_${LOCAL_TARGET}
   print_repositories
}

function init_repositories_framework( )
{
   declare -g -A REPO_TRACING=(
         [url]="git@github.com:dterletskiy/carpc-tracing.git"
         [branch]="experimental"
         [directory]="carpc-tracing"
      )
   declare -g -A REPO_BASE=(
         [url]="git@github.com:dterletskiy/carpc-base.git"
         [branch]="experimental"
         [directory]="carpc-base"
      )
   declare -g -A REPO_TOOLS=(
         [url]="git@github.com:dterletskiy/carpc-tools.git"
         [branch]="experimental"
         [directory]="carpc-tools"
      )
   declare -g -A REPO_OSW=(
         [url]="git@github.com:dterletskiy/carpc-osw.git"
         [branch]="experimental"
         [directory]="carpc-osw"
      )
   declare -g -A REPO_RUNTIME=(
         [url]="git@github.com:dterletskiy/carpc-runtime.git"
         [branch]="experimental"
         [directory]="carpc-runtime"
      )
   declare -g -A REPO_SERVICEBROCKER=(
         [url]="git@github.com:dterletskiy/carpc-servicebrocker.git"
         [branch]="experimental"
         [directory]="carpc-servicebrocker"
      )

   REPOSITORIES=( )
   REPOSITORIES+=( REPO_TRACING )
   REPOSITORIES+=( REPO_BASE )
   REPOSITORIES+=( REPO_TOOLS )
   REPOSITORIES+=( REPO_OSW )
   REPOSITORIES+=( REPO_RUNTIME )
   REPOSITORIES+=( REPO_SERVICEBROCKER )
}

function init_repositories_builder( )
{
   declare -g -A REPO_BUILDER=(
         [url]="git@github.com:dterletskiy/carpc-builder.git"
         [branch]="main"
         [directory]="carpc-builder"
      )

   REPOSITORIES=( )
   REPOSITORIES+=( REPO_BUILDER )
}

function init_repositories_examples( )
{
   declare -g -A REPO_EXAMPLES=(
         [url]="git@github.com:dterletskiy/carpc-examples.git"
         [branch]="scorpius"
         [directory]="carpc-examples"
      )

   REPOSITORIES=( )
   REPOSITORIES+=( REPO_EXAMPLES )
}

function init_repositories_tutorial( )
{
   declare -g -A REPO_TUTORIAL=(
         [url]="git@github.com:dterletskiy/carpc-tutorial.git"
         [branch]="scorpius"
         [directory]="carpc-tutorial"
      )

   REPOSITORIES=( )
   REPOSITORIES+=( REPO_TUTORIAL )
}

declare -a PROJECTS_LIST=(
      "framework"
      "builder"
      "examples"
      "tutorial"
   )

function project_exists( )
{
   local LOCAL_TARGET=${1}
   local item

   for item in "${PROJECTS_LIST[@]}"; do
      log_info "Processing item '${item}'"
      [[ ${item} == "$LOCAL_TARGET" ]] && return 0
   done

   return 1
}

function fetch( )
{
   local LOCAL_TARGET=${1}

   if ! project_exists "${LOCAL_TARGET}"; then
      log_error "target for fetch action is not defined or defined invalid"
      exit 1
   fi

   init_repositories "${LOCAL_TARGET}"

   mkdir -p ${DIRECTORIES[source]}

   local SOURCE_CMAKE_FILE="${DIRECTORIES[source]}/CMakeLists.txt"
   if [ "builder" != ${LOCAL_TARGET} ]; then
      if [ -f ${SOURCE_CMAKE_FILE} ]; then
         log_error "Directory is not empty => repositories can't be cloned"
         exit 2
      fi

      execute "echo \"cmake_minimum_required( VERSION 3.16 FATAL_ERROR )\" > ${SOURCE_CMAKE_FILE}"
      for REPOSITORY_NAME in "${REPOSITORIES[@]}"; do
         declare -n REPOSITORY=${REPOSITORY_NAME}
         execute "echo \"fenix_add_subdirectory( ${REPOSITORY[directory]} )\" >> ${SOURCE_CMAKE_FILE}"
      done
   fi

   for REPOSITORY_NAME in "${REPOSITORIES[@]}"; do
      declare -n REPOSITORY=${REPOSITORY_NAME}
      local COMMAND=(
            git clone --recursive
            -b "${REPOSITORY[branch]}"
            "${REPOSITORY[url]}"
            "${DIRECTORIES[source]}/${REPOSITORY[directory]}"
         )
      execute_arr COMMAND
   done
}



declare -A -g COMPILER=( )
function define_compiler_by_type( )
{
   local LOCAL_COMPILER_TYPE=$(get_parameter_value "compiler")
   case ${LOCAL_COMPILER_TYPE} in
      clang)
         COMPILER["c"]="clang"
         COMPILER["cxx"]="clang++"
      ;;
      gnu)
         COMPILER["c"]="gcc"
         COMPILER["cxx"]="g++"
      ;;
      *)
         log_error "Undefined compiler type '${LOCAL_COMPILER_TYPE}'"
         exit 1
      ;;
   esac

   local COMPILER_PATH="/usr/bin/"
   COMPILER["c"]="${COMPILER_PATH}${COMPILER["c"]}"
   COMPILER["cxx"]="${COMPILER_PATH}${COMPILER["cxx"]}"

   # export CC=${COMPILER["c"]}
   # export CXX=${COMPILER["cxx"]}
}

function update_build_variables( )
{
   define_compiler_by_type

   LOCAL_BUILD_VARIABLES=""
   LOCAL_BUILD_VARIABLES+=" -D ROOT_GEN_DIR:STRING=${DIRECTORIES[gen]}"
   LOCAL_BUILD_VARIABLES+=" -D CARPC_BUILD_TRACE_ENABLED:STRING=$( get_parameter_value "trace" )"
   LOCAL_BUILD_VARIABLES+=" -D CARPC_BUILD_DEBUG:STRING=$( get_parameter_value "debug" )"
   LOCAL_BUILD_VARIABLES+=" -D CARPC_BUILD_RTTI_ENABLED:STRING=$( get_parameter_value "rtti" )"
   LOCAL_BUILD_VARIABLES+=" -D CARPC_BUILD_POLICY_STD:STRING=$( get_parameter_value "policy_std" )"
   LOCAL_BUILD_VARIABLES+=" -D CMAKE_C_COMPILER:STRING=${COMPILER["c"]}"
   LOCAL_BUILD_VARIABLES+=" -D CMAKE_CXX_COMPILER:STRING=${COMPILER["cxx"]}"
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
   local target=${1:-}
   local parameter_target=${target:+--target "${target}"}

   local -a COMMAND=(
      cmake --build "${DIRECTORIES[build]}"
      --verbose
      -j"$(get_parameter_value "jobs")"
      ${parameter_target}
   )

   execute_arr COMMAND
}

function deploy( )
{
   local target=${1:-}
   local deploy_target="deploy${target:+_${target}}"

   build "${deploy_target}"
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

   export LD_LIBRARY_PATH="${DIRECTORIES[deploy]}/lib/:${DIRECTORIES[deploy]}/lib/carpc/:$( get_parameter_value "carpc_lib" )"
   export PATH="${DIRECTORIES[deploy]}/bin/:${DIRECTORIES[deploy]}/bin/carpc/:${PATH}"
   ${LOCAL_TARGET} \
      --config=${DIRECTORIES[deploy]}/etc/carpc/${LOCAL_TARGET}.cfg ${LOCAL_OPTIONS}
}



function get_parameter_value_path( )
{
   echo $(adapt_path $(get_parameter_value "${1}"))
}

function adapt_path( )
{
   echo "$(readlink -m "${1}")/"
}




function main( )
{
   parse_arguments "$@"

   init_directories

   local ACTION=$(get_parameter_value "action")
   case "${ACTION}" in
      fetch)
         fetch $(get_parameter_value "target")
      ;;
      config)
         config
      ;;
      build)
         build $(get_parameter_value "target")
      ;;
      deploy)
         deploy $(get_parameter_value "target")
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
         build $(get_parameter_value "target")
         deploy $(get_parameter_value "target")
      ;;
      run)
         run $(get_parameter_value "target") "$(get_parameter_value "params")"
      ;;
      *)
         log_warning "undefined action: '${ACTION}'"
         exit 1
      ;;
   esac
}

main "$@"
