# https://github.com/python/mypy/issues/3661
from __future__ import annotations
from enum import Enum
import os
import sys

import pfw.base.struct
import pfw.console

import adl.application



DEBUG: bool = False



class Builder:
   def __init__( self, application_: application.Application, gen_dir: str, api_dir: str, imp_dir: str ):
      self.__application = application_
      self.__gen_dir = gen_dir
      self.__api_dir = api_dir
      self.__imp_dir = imp_dir
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Builder.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in Builder.__dict__.keys( ) if i[:2] != pfw.base.struct.ignore_field
 ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      name = "Builder { " + ", ".join( vector ) + " }"
      return name
   # def __str__

   def info( self, **kwargs ):
      tabulations: int = kwargs.get( "tabulations", 0 )
      pfw.console.debug.info( self.__class__.__name__, ":", tabs = ( tabulations + 0 ) )
      pfw.console.debug.info( "application:  \'", self.__application, "\'", tabs = ( tabulations + 1 ) )
      pfw.console.debug.info( "gen_dir:      \'", self.__gen_dir, "\'", tabs = ( tabulations + 1 ) )
   # def info

   def generate( self, code: str, file_name: str, subdirectory: str = None ):
      file = None
      if False == DEBUG:
         file_directory = os.path.join( self.__gen_dir, subdirectory ) if None != subdirectory else self.__gen_dir
         os.makedirs( file_directory, exist_ok = True )
         file = open( os.path.join( file_directory, file_name ), "w" )
         original_stdout = sys.stdout
         sys.stdout = file

      print( code )

      if None != file:
         sys.stdout = original_stdout
         file.close( )
   # def generate

   def build( self, **kwargs ):
      code: str = "";

      code += "// Application components\n"
      for thread in self.__application.threads( ):
         for component in thread.components( ):
            code += "#include \"" + component.location( ) + "\"\n"
      code += "// Framework\n"
      code += "#include \"carpc/application/main.hpp\"\n"
      code += "\n"

      code += "const carpc::application::Thread::Configuration::tVector services =\n"
      code += "{\n"
      threads = self.__application.threads( )
      for thread_count in range( len( threads ) ):
         thread = threads[thread_count]
         code += "   {\n"
         code += "      \"" + thread.name( ) + "\",\n"
         code += "      {\n"
         components = thread.components( )
         for component_count in range( len( components ) ):
            component = components[component_count]
            code += "         " + component.creator( )
            code += ",\n" if len( components ) > ( component_count + 1 ) else "\n"
         code += "      },\n"
         code += "      " + str( thread.watchdog( ) ) + "\n"
         code += "   }"
         code += ",\n" if len( threads ) > ( thread_count + 1 ) else "\n"

      code += "};\n"
      code += "\n"

      code += "bool test( int argc, char** argv, char** envp )\n"
      code += "{\n"
      code += "   return true;\n"
      code += "}\n"
      code += "\n"

      self.generate( code, "main.cpp" )
   # def build



   __application: Application = None
   __gen_dir: str = None
   __api_dir: str = None
   __imp_dir: str = None
# class Builder
