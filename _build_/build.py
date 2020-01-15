#!/usr/bin/python

import os
import sys
import shutil
import stat
import subprocess
import time
import enum

import base
import settings



ExtentionsMap = {
   'header': [ 'hh', 'h', 'hpp', 'hxx' ],
   'source': [ 'cc', 'c', 'cpp', 'cxx' ],
   'object': [ 'o' ]
}



########################################
# 
# class Target
# 
########################################
class Target:
   def __init__( self, source, object ):
      # print( "Target::__init__" )
      self.__source = source
      self.__object = object
   # def __init__

   def __del__( self ):
      # print( "Target::__del__" )
      pass
   # # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Target.__dict__.keys( ) ]
      for attr_name in attr_list:
         if attr == attr_name:
            self.__dict__[ attr ] = value
            return
      raise AttributeError
   # def __setattr__

   def info( self, tabulations ):
      print( tabulations * "\t", "source: ", self.__source )
      print( tabulations * "\t", "object: ", self.__object )
   # def info

   def source( self ): return self.__source
   def object( self ): return self.__object

   __source: str = None
   __object: str = None


########################################
# 
# class ProjectSettings
# 
########################################
class ProjectSettings:
   def __init__( self, root_dir, name ):
      # print( "ProjectSettings::__init__" )
      self.__name = name
      self.__path = { }
      self.__path['project']        = os.path.join( root_dir, self.__name )
      self.__path['product']        = self.__path['project'] + "_product"
      self.__path['obj']            = os.path.join( self.__path['product'], "obj" )
      self.__path['lib']            = os.path.join( self.__path['product'], "lib" )
      self.__path['gen']            = os.path.join( self.__path['product'], "gen" )
      self.__path['exe']            = os.path.join( self.__path['product'], "exe" )
      self.__path['global_config']  = os.path.join( root_dir, "configuration.xml" )
      self.__path['local_config']   = os.path.join( self.__path['project'], "configuration.xml" )
      # Build target list
      self.__target_list = [ ]
      for root, dirs, files in os.walk( self.__path['project'] ):
         for file in files:
            for ext in ExtentionsMap['source']:
               if file.endswith( "." + ext ):
                  source = os.path.join( root, file )
                  object = source.replace( self.__path['project'], self.__path['obj'] ).replace( ext, ExtentionsMap['object'][0] )
                  self.__target_list.append( Target( source, object ) )
      # Building congiguration
      self.__settings = { }
      self.__settings = settings.init( ( self.__path['global_config'], self.__path['local_config'] ) )
   # def __init__

   def __del__( self ):
      pass
      # print( "ProjectSettings::__del__" )
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in ProjectSettings.__dict__.keys( ) ]
      for attr_name in attr_list:
         if attr == attr_name:
            self.__dict__[ attr ] = value
            return
      raise AttributeError
   # def __setattr__

   def info( self ):
      print( "Project information:" )
      print( 1 * "\t", "name:", self.__name )
      print( 1 * "\t", "path:" )
      for key, value in self.__path.items( ):
         print( 2 * "\t", key, " -> ", value )
      print( 1 * "\t", "targets:" )
      for target in self.__target_list:
         target.info( 2 )
      print( 1 * "\t", "settings: ", self.__settings )
   # def info

   def compile( self ) -> bool:
      for target in self.__target_list:
         if False == self.compile_target( target ):
            return False

      return True
   # def compile

   def compile_target( self, target ) -> bool:
      print( self.__delimeter )
      print( "Processing:", target.source( ) )
      source_time = base.file_time( target.source( ) )
      object_time = base.file_time( target.object( ) )
      if None == source_time:
         print( "Source doen't exist: ", target.source( ) )
         print( self.__delimeter )
         return False
      elif None == object_time:
         print( "Compiling..." )
      elif source_time > object_time:
         print( "Compiling..." )
      else:
         print( "Compiled" )
         print( self.__delimeter )
         return True
      code: str = COMPILER + " " + self.__settings['cflags'] + " -c -o " + target.object( ) + " " + target.source( )
      os.makedirs( os.path.dirname( target.object( ) ), exist_ok = True )
      print( code )
      result: bool = 0 == os.system( code )
      print( self.__delimeter )
      return result
   # def compile_target

   def link( self ) -> bool:
      print( self.__delimeter )
      print( "Linking..." )
      object_list: str = ""
      for target in self.__target_list:
         object_list += " " + target.object( )
      product_path: str = None
      if "shared" == self.__settings['type'] or "static" == self.__settings['type'] :
         product_path = os.path.join( self.__path['lib'], self.__settings['product'] )
      else:
         product_path = os.path.join( self.__path['exe'], self.__settings['product'] )
      os.makedirs( os.path.dirname( product_path ), exist_ok = True )
      code: str = COMPILER + " -o " + product_path + object_list + self.__settings['ldflags']
      print( code )
      result: bool = 0 == os.system( code )
      print( self.__delimeter )
      return result
   # def link

   def build( self ) -> bool:
      if False == self.compile( ):
         print( "Build ERROR: compilation error" )
         return False

      if False == self.link( ):
         print( "Build ERROR: link error" )
         return False

      print( "Build SUCCESS" )
      return True
   # def build

   def clean( self ) -> bool:
      try: shutil.rmtree( self.__path['product'] )
      except: print( "Error while deleting directory: ", self.__path['product'] )

   __name: str = ""
   __path: dict = { }
   __target_list: list = [ ]
   __settings: dict = { }
   __delimeter: str = "--------------------------------------------------"






os.system( "reset" )
# os.system( "source \"/home/scorpion/Source/RPC/_build_/environment-setup-core2-64-hcp3-linux-musl\"" )
print( os.environ['CXX'] )
COMPILER=os.environ['CXX']

def create_all( names: list, projects: dict, root_dir: str ):
   print( "Creating..." )
   for name in names:
      projects[ name ] = ProjectSettings( root_dir, name )

def info_all( projects: dict ):
   print( "Project info..." )
   for name, project in projects.items( ):
      project.info( )

def clean_all( projects: dict ) -> bool:
   print( "Cleaning..." )
   for name, project in projects.items( ):
      project.clean( )

def compile_all( projects: dict ) -> bool:
   print( "Compiling..." )
   for name, project in projects.items( ):
      if False == project.compile( ): sys.exit( 1 )

def link_all( projects: dict ) -> bool:
   print( "Linking..." )
   for name, project in projects.items( ):
      if False == project.link( ): sys.exit( 2 )



RootDir = "/home/scorpion/Source/RPC"
PROJECTS: dict = { }
create_all( [ "framework", "hooks", "servicebrocker", "application" ], PROJECTS, RootDir )
info_all( PROJECTS )
clean_all( PROJECTS )
compile_all( PROJECTS )
link_all( PROJECTS )
