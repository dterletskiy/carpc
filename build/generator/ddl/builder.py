# https://github.com/python/mypy/issues/3661
from __future__ import annotations
from enum import Enum
import os
import sys

import pfw.base
import pfw.console

import ddl.data



DEBUG: bool = False



class Builder:
   def __init__( self, data_: data.Data, gen_dir: str, api_dir: str, imp_dir: str ):
      self.__data = data_
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
      attr_list = [ i for i in Builder.__dict__.keys( ) if i[:2] != pfw.base.class_ignore_field ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      name = "Builder { " + ", ".join( vector ) + " }"
      return name
   # def __str__

   def info( self, **kwargs ):
      tabulations: int = kwargs.get( "tabulations", 0 )
      pfw.console.debug.info( self.__class__.__name__, ":", tabs = ( tabulations + 0 ) )
      pfw.console.debug.info( "data:         \'", self.__data, "\'", tabs = ( tabulations + 1 ) )
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
      self.build_data_hpp( )
      self.build_data_cpp( )
   # def build

   def build_data_hpp( self ):
      code: str = "";

      code += "#pragma once\n";
      code += "\n"
      code += "#include \"carpc/common/IPC.hpp\"\n"
      code += "\n"


      code += "namespace " + self.__data.namespace( ) + " {\n"
      code += "\n"

      for struct in self.__data.structs( ):
         code += "   struct " + struct.name( ) + "\n"
         code += "   {\n"

         for method in struct.methods( ):
            code += "      " + method.return_type( ) + " " + method.name( ) + "( "
            function_arguments = method.arguments( )
            for count in range( len( function_arguments ) ):
               argument = function_arguments[count]
               code += argument.type( ) + "& " + argument.name( )
               code += ", " if len( function_arguments ) > ( count + 1 ) else " "
            code += ");\n"
            code += "\n"

         for field in struct.fields( ):
            code += "      " + field.type( ) + " " + field.name( ) + ";\n"
            code += "\n"


         code += "      bool to_stream( carpc::ipc::tStream& ) const;\n"
         code += "      bool from_stream( carpc::ipc::tStream& );\n"

         code += "   }; // struct " + struct.name( ) + "\n"
         code += "\n"

      code += "} // namespace " + self.__data.namespace( ) + "\n"
      code += "\n"

      self.generate( code, "Types.hpp" )
   # def build_data_hpp

   def build_data_cpp( self ):
      code: str = "";

      code += "#include \"Types.hpp\"\n";
      code += "\n"
      code += "using namespace " + self.__data.namespace( ) + ";\n"
      code += "\n"

      for struct in self.__data.structs( ):
         code += "bool " + struct.name( ) + "::to_stream( carpc::ipc::tStream& stream ) const\n"
         code += "{\n"
         code += "   return carpc::ipc::serialize( stream"
         for field in struct.fields( ):
            code += ", " + field.name( )
         code += " );\n"
         code += "}\n"
         code += "\n"

         code += "bool " + struct.name( ) + "::from_stream( carpc::ipc::tStream& stream )\n"
         code += "{\n"
         code += "   return carpc::ipc::deserialize( stream"
         for field in struct.fields( ):
            code += ", " + field.name( )
         code += " );\n"
         code += "}\n"
         code += "\n"

      self.generate( code, "Types.cpp" )
   # def build_data_cpp



   __data: Data = None
   __gen_dir: str = None
   __api_dir: str = None
   __imp_dir: str = None
# class Builder
