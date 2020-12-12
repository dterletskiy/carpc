#!/usr/bin/python

import os
import sys
import shutil
import stat
import subprocess
import time
# import datetime
from datetime import datetime
import enum

import console
import file
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
   def __init__( self, source, object, preproc ):
      # console.debug.trace( "Target::__init__" )
      self.__source = source
      self.__object = object
      self.__preproc = preproc
   # def __init__

   def __del__( self ):
      # console.debug.trace( "Target::__del__" )
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
      console.debug.info( tabulations * "\t", "source: ", self.__source )
      console.debug.info( tabulations * "\t", "object: ", self.__object )
      console.debug.info( tabulations * "\t", "preproc: ", self.__preproc )
   # def info

   def source( self ): return self.__source
   def object( self ): return self.__object
   def preproc( self ): return self.__preproc

   __source: str = None
   __object: str = None
   __preproc: str = None


########################################
# 
# class ProjectSettings
# 
########################################
class ProjectSettings:
   def __init__( self, root_dir: str, name: str, session: str, compiler: str ):
      # console.debug.trace( "ProjectSettings::__init__" )
      self.__name = name
      self.__session = "" #session
      self.__compiler = compiler
      self.__path = { }
      self.__path['project']        = os.path.join( root_dir, self.__name )
      self.__path['product']        = os.path.join( self.__path['project'] + "_product", self.__session )
      self.__path['preproc']        = os.path.join( self.__path['product'], "preproc" )
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
                  object = source.replace( self.__path['project'], self.__path['obj'] ).replace( "." + ext, "." + ExtentionsMap['object'][0] )
                  preproc = source.replace( self.__path['project'], self.__path['preproc'] )
                  self.__target_list.append( Target( source, object, preproc ) )
      # Building congiguration
      self.__settings = { }
      self.__settings = settings.init( ( self.__path['global_config'], self.__path['local_config'] ) )
   # def __init__

   def __del__( self ):
      pass
      # console.debug.trace( "ProjectSettings::__del__" )
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
      console.debug.header( self.__delimeter )
      console.debug.header( "Project information:" )
      console.debug.info( 1 * "\t", "name:", self.__name )
      console.debug.info( 1 * "\t", "session:", self.__session )
      console.debug.info( 1 * "\t", "compiler:", self.__compiler )
      console.debug.info( 1 * "\t", "path:" )
      for key, value in self.__path.items( ):
         console.debug.info( 2 * "\t", key, " -> ", value )
      console.debug.info( 1 * "\t", "targets:" )
      for target in self.__target_list:
         target.info( 2 )
      console.debug.info( 1 * "\t", "settings: ", self.__settings )
      console.debug.header( self.__delimeter )
   # def info

   def compile( self ) -> bool:
      start_compile: int = int( round(time.time() * 1000) )

      for target in self.__target_list:
         if False == self.compile_target( target ):
            return False

      finish_compile: int = int( round(time.time() * 1000) )
      console.debug.question( "Total compile time: %d ms" %(finish_compile - start_compile) )

      return True
   # def compile

   def compile_target( self, target ) -> bool:
      start_compile: int = int( round(time.time() * 1000) )

      console.debug.header( self.__delimeter )
      console.debug.info( "Processing:", target.source( ) )
      source_time = file.file_time( target.source( ) )
      object_time = file.file_time( target.object( ) )

      if None == source_time:
         console.debug.error( "Source doen't exist: ", target.source( ) )
         console.debug.header( self.__delimeter )
         return False
      elif None == object_time:
         console.debug.info( "Compiling..." )
      elif source_time > object_time:
         console.debug.info( "Compiling..." )
      else:
         console.debug.info( "Compiled" )
         console.debug.header( self.__delimeter )
         return True

      code: str = self.__compiler + " " + self.__settings['cflags'] + " -c -o " + target.object( ) + " " + target.source( )
      os.makedirs( os.path.dirname( target.object( ) ), exist_ok = True )
      console.debug.trace( code )
      result: bool = 0 == os.system( code )

      finish_compile: int = int( round(time.time() * 1000) )
      console.debug.question( "compile time: %d ms" %(finish_compile - start_compile) )

      console.debug.header( self.__delimeter )
      return result
   # def compile_target

   def preprocessing( self ) -> bool:
      start_compile: int = int( round(time.time() * 1000) )

      for target in self.__target_list:
         if False == self.preprocessing_target( target ):
            return False

      finish_compile: int = int( round(time.time() * 1000) )
      console.debug.question( "Total preprocessing time: %d ms" %(finish_compile - start_compile) )

      return True
   # def preprocessing

   def preprocessing_target( self, target ) -> bool:
      start_compile: int = int( round(time.time() * 1000) )

      console.debug.header( self.__delimeter )
      console.debug.info( "Processing:", target.source( ) )
      source_time = file.file_time( target.source( ) )
      preproc_time = file.file_time( target.preproc( ) )

      if None == source_time:
         console.debug.error( "Source doen't exist: ", target.source( ) )
         console.debug.header( self.__delimeter )
         return False
      elif None == preproc_time:
         console.debug.info( "Preprocessing..." )
      elif source_time > preproc_time:
         console.debug.info( "Preprocessing..." )
      else:
         console.debug.info( "Compiled" )
         console.debug.header( self.__delimeter )
         return True

      code: str = self.__compiler + " " + self.__settings['cflags'] + " -E -o " + target.preproc( ) + " " + target.source( )
      os.makedirs( os.path.dirname( target.preproc( ) ), exist_ok = True )
      console.debug.trace( code )
      result: bool = 0 == os.system( code )

      finish_compile: int = int( round(time.time() * 1000) )
      console.debug.question( "preprocessing time: %d ms" %(finish_compile - start_compile) )

      console.debug.header( self.__delimeter )
      return result
   # def compile_target

   def link( self ) -> bool:
      start_compile: int = int( round(time.time() * 1000) )

      console.debug.header( self.__delimeter )
      console.debug.info( "Linking..." )
      object_list: str = ""
      for target in self.__target_list:
         object_list += " " + target.object( )
      product_path: str = None
      if "shared" == self.__settings['type'] or "static" == self.__settings['type'] :
         product_path = os.path.join( self.__path['lib'], self.__settings['product'] )
      else:
         product_path = os.path.join( self.__path['exe'], self.__settings['product'] )
      os.makedirs( os.path.dirname( product_path ), exist_ok = True )
      code: str = self.__compiler + " -o " + product_path + object_list + self.__settings['ldflags']
      console.debug.trace( code )
      result: bool = 0 == os.system( code )
      console.debug.header( self.__delimeter )

      finish_compile: int = int( round(time.time() * 1000) )
      console.debug.question( "Total link time: %d ms" %(finish_compile - start_compile) )

      return result
   # def link

   def build( self ) -> bool:
      start_compile: int = int( round(time.time() * 1000) )

      if False == self.compile( ):
         console.debug.error( "compilation error" )
         return False

      if False == self.link( ):
         console.debug.error( "link error" )
         return False

      console.debug.ok( "Build SUCCESS" )

      finish_compile: int = int( round(time.time() * 1000) )
      console.debug.question( "Total build time: %d ms" %(finish_compile - start_compile) )

      return True
   # def build

   def clean( self ) -> bool:
      try: shutil.rmtree( self.__path['product'] )
      except: console.debug.error( "Error while deleting directory: ", self.__path['product'] )

   __name: str = None
   __session: str = None
   __path: dict = { }
   __target_list: list = [ ]
   __settings: dict = { }
   __compiler: str = None
   __delimeter: str = "------------------------------------------------------------------------------------------------------------------------------------------------------"



def init( names: list, projects: dict, root_dir: str, session: str, compiler: str ):
   console.debug.trace( "Building projects infrastructure..." )
   for name in names:
      projects[ name ] = ProjectSettings( root_dir, name, session, compiler )

def info_all( projects: dict ):
   console.debug.trace( "Project info..." )
   for name, project in projects.items( ):
      project.info( )

def clean_all( projects: dict ) -> bool:
   console.debug.trace( "Cleaning..." )
   for name, project in projects.items( ):
      project.clean( )

def preprocessing_all( projects: dict ) -> bool:
   console.debug.trace( "Preprocessing..." )
   for name, project in projects.items( ):
      if False == project.preprocessing( ): sys.exit( 1 )

def compile_all( projects: dict ) -> bool:
   console.debug.trace( "Compiling..." )
   for name, project in projects.items( ):
      if False == project.compile( ): sys.exit( 1 )

def link_all( projects: dict ) -> bool:
   console.debug.trace( "Linking..." )
   for name, project in projects.items( ):
      if False == project.link( ): sys.exit( 2 )
