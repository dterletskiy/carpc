#!/usr/bin/python

import os
import sys
import datetime
import shutil
import errno
import ntpath

import base
import console



project_path: str = "/home/scorpion/Templates/RPC/"
git: str = "cd " + project_path + "; git "
archive_path: str = "/home/scorpion/Templates/RPC_Archive/"
exceptions: list = [ ".git", "git.py", "base.py", "console.py", "__pycache__" ]




def copy_recursively( _source_folder: str, _destination_folder: str ):
   console.debug.trace( "Copying directory content from '", _source_folder, "' to '", _destination_folder, "'" )
   for file_name in os.listdir( _source_folder ):
      source = os.path.join( _source_folder, file_name )
      destination = os.path.join( _destination_folder, file_name )

      try:
         shutil.copytree( source, destination )
      except OSError as err:
         # error caused if the source was not a directory
         if err.errno == errno.ENOTDIR:
            shutil.copy2( source, destination )
         else:
            console.debug.error( "% s" % err )
# def copy_recursively


def clean_dir( _dir: str ):
   console.debug.trace( "Cleaning directory: ", _dir )
   for file_name in os.listdir( _dir ):
      skip: bool = False
      for exception_file in exceptions:
         if file_name == exception_file:
            console.debug.warning( "skipping: ", file_name )
            skip = True
            continue
      if True == skip:
         continue

      file_path = os.path.join( _dir, file_name )
      try:
         if os.path.isfile( file_path ) or os.path.islink( file_path ):
            console.debug.trace( "deleting file: ", file_path )
            os.unlink( file_path )
         elif os.path.isdir( file_path ):
            console.debug.trace( "deleting directory: ", file_path )
            shutil.rmtree( file_path )
      except Exception as e:
         console.debug.error( 'Failed to delete %s. Reason: %s' % ( file_path, e ) )
# def clean_dir



archive_directories: list = [ ]
for archive in os.listdir( archive_path ):
   archive_directories.append( os.path.join( archive_path, archive ) )
archive_directories.sort( )
console.debug.info( "root: ", archive_directories )


os.system( git + "init" )
os.system( git + "commit --allow-empty -m 'Initial commit'" )
for archive_directory in archive_directories:
   console.debug.info( "---------- Processing: ", archive_directory, " ----------" )
   clean_dir( project_path )
   copy_recursively( archive_directory, project_path )
   os.system( git + "add ." )
   os.system( git + "status" )
   os.system( git + "commit -m 'version " + ntpath.basename( archive_directory ) + "'" )
   os.system( git + "status" )
