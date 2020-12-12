#!/usr/bin/python

import os
import stat
import time

import console



def file_time( file ):
   try:
      st = os.stat( file )
   except IOError:
      return None
   else:
      return st[ stat.ST_MTIME ]
      # return time.asctime( time.localtime(st[stat.ST_MTIME]) )
# def file_time

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

exceptions: list = [ ".git", "git.py", "base.py", "console.py", "__pycache__" ]

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
