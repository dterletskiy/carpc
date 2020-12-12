#!/usr/bin/python

import shutil
import datetime

import console



def archive( dir_name: str ):
   output_filename: str = dir_name + "_" + datetime.datetime.now( ).strftime( "%Y-%m-%d_%H-%M-%S" )
   print( "date and time =", output_filename )
   shutil.make_archive( output_filename, 'zip', dir_name )
# def archive
