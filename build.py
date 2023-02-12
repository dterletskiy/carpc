#!/usr/bin/python3



# Examples:
# 
# CONFIG=./configuration.cfg
# PFW=/mnt/dev/repos/github.com/dterletskiy/python_fw
# ARCH=x86_64
# OS=linux
# ACTION=clean_build
# TARGET=runtime
# 
# 
# 
# ./build.py --config=./${CONFIG} --include=${PFW} --arch=${ARCH} --os=${OS} --action=${ACTION}
# ./build.py --config=./${CONFIG} --arch=${ARCH} --os=${OS} --action=${ACTION}
# 
# In case if variable "INCLUDE" defined with path to "pfw" "--include" option could be omitted.
# If "INCLUDE" variable defined several times in configuration file all mentioned values will be used.



import os
import sys
import subprocess
import copy

import configuration



##########################################################################
#                                                                        #
#                          Begin configuration                           #
#                                                                        #
##########################################################################

MIN_PYTHON = (3, 8)
if sys.version_info < MIN_PYTHON:
   print( "Python minimal required version is %s.%s" % MIN_PYTHON )
   print( "Current version is %s.%s" % ( sys.version_info.major, sys.version_info.minor ) )
   sys.exit( )



configuration.configure( sys.argv[1:] )



import pfw.console
import pfw.shell
import pfw.size
import pfw.os.environment
import pfw.linux.os.process



os_map: dict = {
   "linux": 0,
   "android": 1
}

def extend_configuration( ):
   configuration.config.set_value( "tmp_dir", "/tmp" )

   source_dir = configuration.config.get_value( "source_dir" )
   product_dir = os.path.join( source_dir + "_product", f"{configuration.config.get_value( 'os' )}-{configuration.config.get_value( 'arch' )}" )
   configuration.config.set_value( "product_dir", product_dir )
   gen_dir = os.path.join( product_dir, "gen" )
   configuration.config.set_value( "gen_dir", gen_dir )
   build_dir = os.path.join( product_dir, "build" )
   configuration.config.set_value( "build_dir", build_dir )
   delivery_dir = os.path.join( product_dir, "delivery" )
   configuration.config.set_value( "delivery_dir", delivery_dir )
   documentation_dir = os.path.join( product_dir, "documentation" )
   configuration.config.set_value( "documentation_dir", documentation_dir )

   CC = "/usr/bin/cc"
   CXX = "/usr/bin/c++"
   if None == configuration.config.get_value( "compiler" ):
      configuration.config.set_value( "compiler", "gcc" )
   if "gcc" == configuration.config.get_value( "compiler" ):
      CC = "/usr/bin/cc"
      CXX = "/usr/bin/c++"
   elif "clang" == configuration.config.get_value( "compiler" ):
      CC = "/usr/bin/clang"
      CXX = "/usr/bin/clang++"
   else:
      CC = "/usr/bin/cc"
      CXX = "/usr/bin/c++"

   env_overwrite = {
         "CC": [ f"{CC}" ],
         "CXX": [ f"{CXX}" ]
      }
   env_add = {
         "LFS_VERSION": [
               "1.0"
            ],
         "PATH": [
               f"{configuration.value( 'delivery_dir' )}/bin",
               f"{configuration.value( 'carpc_delivery_dir' )}/bin"
            ],
         "LD_LIBRARY_PATH": [
               f"{configuration.value( 'delivery_dir' )}/lib",
               f"{configuration.value( 'carpc_delivery_dir' )}/lib"
            ],
         "LD_PRELOAD": [
               # f"{configuration.value( 'carpc_delivery_dir' )}/lib/libhooks.so"
            ]
      }
   environment: dict = pfw.os.environment.build( None, env_overwrite, env_add )
   configuration.config.set_value( "env", [ environment ] )
# def extend_configuration

extend_configuration( )
configuration.info( )



pfw.shell.execute( "/bin/echo ${LFS_VERSION}", env = configuration.value( "env" ), shell = True )



##########################################################################
#                                                                        #
#                           End configuration                            #
#                                                                        #
##########################################################################



def configure( **kwargs ):
   command = ""
   command += f" cmake"
   command += f" -S {configuration.value( 'source_dir' )}"
   command += f" -B {configuration.value( 'build_dir' )}"
   command += f" -DCMAKE_INSTALL_PREFIX={configuration.value( 'delivery_dir' )}"
   command += f" --graphviz={configuration.value( 'documentation_dir' )}/graph"

   command += f" -DOS_ANDROID:STRING={os_map['android']}"
   command += f" -DOS_LINUX:STRING={os_map['linux']}"
   command += f" -DOS_TARGET:STRING={os_map[configuration.value('os')]}"

   command += f" -DROOT_SOURCE_DIR={configuration.value( 'source_dir' )}"
   command += f" -DROOT_GEN_DIR={configuration.value( 'gen_dir' )}"
   command += f" -DROOT_BUILD_DIR={configuration.value( 'build_dir' )}"
   command += f" -DROOT_DELIVERY_DIR={configuration.value( 'delivery_dir' )}"
   command += f" -DROOT_DOCUMENTATION_DIR={configuration.value( 'documentation_dir' )}"
   command += f" -DCARPC_DELIVERY_DIR={configuration.value( 'carpc_delivery_dir' )}"
   command += f" -DCARPC_INCLUDE_DIR={configuration.value( 'carpc_delivery_dir' )}/include"
   command += f" -DCARPC_LIB_DIR={configuration.value( 'carpc_delivery_dir' )}/lib"
   command += f" -DCARPC_BIN_DIR={configuration.value( 'carpc_delivery_dir' )}/bin"
   command += f" -DPFW={configuration.value( 'pfw' )}"

   pfw.shell.execute( command, output = pfw.shell.eOutput.PTY, env = configuration.value( "env" ) )
# def configure

def build( **kwargs ):
   kw_target = kwargs.get( "target", configuration.value( "target" ) )

   if None == kw_target:
      kw_target = "all"

   command = f"cmake"
   command += f" --build {configuration.value( 'build_dir' )}"
   command += f" --target {kw_target}"
   command += f" -j 8"

   pfw.shell.execute( command, output = pfw.shell.eOutput.PTY )
# def build

def install( **kwargs ):
   command = f"cmake"
   command += f" --build {configuration.value( 'build_dir' )}"
   command += f" --target install"

   pfw.shell.execute( command, output = pfw.shell.eOutput.PTY )
# def install

def clean( **kwargs ):
   command = f"rm -rf {configuration.value( 'product_dir' )}"

   pfw.shell.execute( command, output = pfw.shell.eOutput.PTY )
# def clean

def start_target( **kwargs ):
   kw_target = kwargs.get( "target", configuration.value( "target" ) )
   kw_blocking = kwargs.get( "blocking", True )
   kw_config = kwargs.get( "config", configuration.value( "cfg" ) )
   kw_trace = kwargs.get( "trace", configuration.value( "trace" ) )

   kw_config = kw_config if None != kw_config else os.path.join( configuration.value( 'delivery_dir' ), "etc", f"{kw_target}.cfg" )
   kw_trace = kw_trace if None != kw_trace else "CONSOLE"

   if None == kw_target:
      pfw.console.debug.error( "'target' is not defined" )
      return

   if None != pid:
   pid: int = pfw.linux.os.process.is_running( kw_target )
      pfw.console.debug.warning( "process '%s' is running with pid '%d'" % ( kw_target, pid ) )
      return

   # command = f"{configuration.value( 'delivery_dir' )}/bin/{kw_target}"
   command = f"{kw_target}"
   command += f" config={kw_config}"
   command += f" trace={kw_trace}"
   command += "&" if False == kw_blocking else ""

   pfw.shell.execute( command, output = pfw.shell.eOutput.PTY, env = configuration.value( "env" ) )
# def start_target

def stop_target( **kwargs ):
   kw_target = kwargs.get( "target", configuration.value( "target" ) )

   if None == kw_target:
      pfw.console.debug.error( "'target' is not defined" )
      return

   pfw.linux.os.process.kill( kw_target )
# def stop_target

def start_dlt_daemon( **kwargs ):
   kw_dlt_daemon = kwargs.get( "name", "dlt-daemon" )
   kw_dlt_daemon_config = kwargs.get( "config", None )

   pid: int = pfw.linux.os.process.is_running( kw_dlt_daemon )
   if None != pid:
      pfw.console.debug.warning( "process '%s' is running with pid '%d'" % ( kw_dlt_daemon, pid ) )
      return

   command = f"{kw_dlt_daemon} -d"
   if None != kw_dlt_daemon_config:
      command += f" -c {kw_dlt_daemon_config}"

   pfw.shell.execute( command, output = pfw.shell.eOutput.PTY, env = configuration.value( "env" ) )
# def start_dlt_daemon

def stop_dlt_daemon( **kwargs ):
   kw_dlt_daemon = kwargs.get( "name", "dlt-daemon" )

   pfw.linux.os.process.kill( kw_dlt_daemon )
# def stop_dlt_daemon

def main( ):
   action = configuration.value( "action" )

   if "clean" == action:
      clean( )
   elif "config" == action:
      configure( )
   elif "build" == action:
      configure( )
      build( )
   elif "install" == action:
      install( )
   elif "world" == action:
      clean( )
      configure( )
      build( )
      install( )
   elif "start" == action:
      start_target( )
   elif "stop" == action:
      stop_target( )
   else:
      pfw.console.debug.error( "undefined 'action': ", action )

# def main



if __name__ == "__main__":
   pfw.console.debug.ok( "------------------------- BEGIN -------------------------" )
   main( )
   pfw.console.debug.ok( "-------------------------- END --------------------------" )

