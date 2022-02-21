readonly ENVIRONMENT_SETUP_ANDROID=./build/android-env.sh



declare -A COMMAND=(
   [INFO]="info"
   [CLEAN]="clean"
   [CONFIGURE]="configure"
   [BUILD]="build"
   [CLEAN_BUILD]="clean_build"
   [INSTALL]="install"
   [ARCHIVE]="archive"
   [START]="start"
   [STOP]="stop"
   [ADB_INSTALL]="adb_install"
)

function info( )
{
   echo "   ${COMMAND[INFO]}         - print project info"
   echo "   ${COMMAND[CLEAN]}        - clean product folder"
   echo "   ${COMMAND[CONFIGURE]}    - configure build"
   echo "   ${COMMAND[BUILD]}        - build project"
   echo "   ${COMMAND[CLEAN_BUILD]}  - clean build project"
   echo "   ${COMMAND[INSTALL]}      - install built project"
   echo "   ${COMMAND[ARCHIVE]}      - archive project"
   echo "   ${COMMAND[START]}        - start installed project"
   echo "   ${COMMAND[STOP]}         - stop installed project"
   echo "   ${COMMAND[ADB_INSTALL]}  - install to device via adb"
}

declare -A ARCH_MAP=(
      [x86]=0
      [x86_64]=1
      [arm]=2
      [aarch64]=3
   )

declare -A OS_MAP=(
      [linux]=0
      [android]=1
   )

readonly CORES=$( grep -c ^processor /proc/cpuinfo )

readonly EXPERIMENTAL=experimental
readonly APPLICATION=application
readonly SERVICEBROCKER=servicebrocker
readonly HMI=hmi
readonly CONTROLLER=controller
readonly CORE=core
readonly HYBRID=hybrid
readonly SYMBIONT=symbiont
readonly DLT_DAEMON="dlt-daemon"
