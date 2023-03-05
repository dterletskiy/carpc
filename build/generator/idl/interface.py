# https://github.com/python/mypy/issues/3661
from __future__ import annotations
from enum import Enum

import os

import pfw.base.struct
import pfw.console
import pfw.code



class Connection:
   def __init__( self, request: pfw.code.Function, response: pfw.code.Function = None ):
      self.__request = request
      self.__response = response
      self.__name = self.__request.name( )
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Connection.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in Connection.__dict__.keys( ) if i[:2] != pfw.base.struct.ignore_field
 ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      name = "Connection { " + ", ".join( vector ) + " }"
      return name
   # def __str__

   def info( self, **kwargs ):
      tabulations: int = kwargs.get( "tabulations", 0 )
      pfw.console.debug.info( self.__class__.__name__, ":", tabs = ( tabulations + 0 ) )
      pfw.console.debug.info( "name:   \'", self.__name, "\'", tabs = ( tabulations + 1 ) )
      self.__request.info( tabulations = tabulations + 1 )
      if None != self.__response:
         self.__response.info( tabulations = tabulations + 1 )
   # def info

   def set_request( self, request: pfw.code.Function ):
      self.__request = request
      self.__name = self.__request.name( )
   # def set_request

   def set_response( self, response: pfw.code.Function ):
      self.__response = response
   # def set_response

   def name( self ):
      return self.__name
   # def name

   def request( self ):
      return self.__request
   # def request

   def response( self ):
      return self.__response
   # def response



   __name: str = None
   __request: pfw.code.Function = None
   __response: pfw.code.Function = None
# class Connection



class Interface:
   def __init__( self, name: str = None ):
      self.__includes = [ ]
      self.__name = name
      self.__connections = [ ]
      self.__attributes = [ ]
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Interface.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in Interface.__dict__.keys( ) if i[:2] != pfw.base.struct.ignore_field
 ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      name = "Interface { " + ", ".join( vector ) + " }"
      return name
   # def __str__

   def info( self, **kwargs ):
      tabulations: int = kwargs.get( "tabulations", 0 )
      pfw.console.debug.info( self.__class__.__name__, ":", tabs = ( tabulations + 0 ) )
      pfw.console.debug.info( "name:         \'", self.__name, "\'", tabs = ( tabulations + 1 ) )
      pfw.console.debug.info( "is_ipc:       \'", self.__is_ipc, "\'", tabs = ( tabulations + 1 ) )
      self.__version.info( );
      pfw.console.debug.info( "includes:     \'", self.__includes, "\'", tabs = ( tabulations + 1 ) )
      for connection in self.__connections: connection.info( tabulations = tabulations + 1 )
      for attribute in self.__attributes: attribute.info( tabulations = tabulations + 1 )
   # def info

   def add_include( self, include: str ):
      ( file_name, file_extension ) = os.path.splitext( os.path.basename( include ) )
      file_dir = os.path.dirname( include )
      self.__includes.append( os.path.join( file_dir, file_name, "Types.hpp" ) )
   # def add_include

   def set_version( self, version: pfw.code.Version ):
      self.__version = version
   # def set_version

   def set_name( self, name: str ):
      self.__name = name
   # def set_name

   def set_is_ipc( self, is_ipc: str ):
      self.__is_ipc = is_ipc
   # def set_is_ipc

   def add_connection( self, connection: Connection ):
      self.__connections.append( connection )
   # def add_connection

   def add_request( self, request: pfw.code.Function ):
      self.__connections.append( Connection( request ) );
   # def add_request

   def add_response( self, response: pfw.code.Function, request_name: str ):
      for connection in self.__connections:
         if connection.request( ).name( ) == request_name:
            connection.set_response( response )
            return

      pfw.console.debug.error( "request was not defined: ", request_name )
   # def add_response

   def add_attribute( self, attribute: pfw.code.Parameter ):
      self.__attributes.append( attribute )
   # def add_attribute

   def includes( self ):
      return self.__includes
   # def includes

   def version( self ):
      return self.__version
   # def version

   def name( self ):
      return self.__name
   # def name

   def is_ipc( self ):
      return self.__is_ipc
   # def is_ipc

   def connections( self ):
      return self.__connections
   # def connections

   def attributes( self ):
      return self.__attributes
   # def attributes

   def namespace( self ):
      return "service::" + self.__name + "::V" + self.__version.string( "_" )
   # def namespace



   __includes: list = [ ]
   __version: pfw.code.Version = None
   __name: str = None
   __is_ipc: bool = False
   __connections: list = [ ]
   __attributes: list = [ ]
# class Interface
