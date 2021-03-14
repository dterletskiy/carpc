#!/usr/bin/python

import shutil
import datetime

import console





def archive( dir_name: str, format: str = "zip" ):
   output_filename: str = dir_name + "_" + datetime.datetime.now( ).strftime( "%Y-%m-%d_%H-%M-%S" )
   print( "date and time =", output_filename )
   if "all" == format:
      shutil.make_archive( output_filename, "zip", dir_name )
      shutil.make_archive( output_filename, "tar", dir_name )
      shutil.make_archive( output_filename, "gztar", dir_name )
      shutil.make_archive( output_filename, "bztar", dir_name )
      shutil.make_archive( output_filename, "xztar", dir_name )
   else:
      shutil.make_archive( output_filename, format, dir_name )
# def archive
