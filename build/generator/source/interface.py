# https://github.com/python/mypy/issues/3661
from __future__ import annotations
from enum import Enum

import pfw.base
import pfw.console



class Version:
   def __init__( self, major: int, minor: int, patch: int = None ):
      self.__major = major
      self.__minor = minor
      self.__patch = patch
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Version.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in Version.__dict__.keys( ) if i[:2] != pfw.base.class_ignore_field ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      name = "Version { " + ", ".join( vector ) + " }"
      return name
   # def __str__

   def info( self, **kwargs ):
      tabulations: int = kwargs.get( "tabulations", 0 )
      pfw.console.debug.info( self.__class__.__name__, ":", tabs = ( tabulations + 0 ) )
      pfw.console.debug.info( "major:  \'", self.__major, "\'", tabs = ( tabulations + 1 ) )
      pfw.console.debug.info( "minor:  \'", self.__minor, "\'", tabs = ( tabulations + 1 ) )
      pfw.console.debug.info( "patch:  \'", self.__patch, "\'", tabs = ( tabulations + 1 ) )
   # def info

   def major( self ):
      return self.__major
   # def major

   def minor( self ):
      return self.__minor
   # def minor

   def patch( self ):
      return self.__patch
   # def type

   def string( self, separator: str = "." ):
      return str(self.__major) + separator + str(self.__minor) + separator + str(self.__patch)
   # def string



   __major: int = None
   __minor: int = None
   __patch: int = None
# class Version



def type_builder( type: str ):
   if "string" == type:
      return "std::string"
   elif "size_t" == type:
      return "std::size_t"

   return type
# def type_builder



class Parameter:
   def __init__( self, type: str, name: str, value: str = None ):
      self.__type = type_builder( type )
      self.__name = name
      self.__value = value
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Parameter.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in Parameter.__dict__.keys( ) if i[:2] != pfw.base.class_ignore_field ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      name = "Parameter { " + ", ".join( vector ) + " }"
      return name
   # def __str__

   def info( self, **kwargs ):
      tabulations: int = kwargs.get( "tabulations", 0 )
      pfw.console.debug.info( self.__class__.__name__, ":", tabs = ( tabulations + 0 ) )
      pfw.console.debug.info( "type:   \'", self.__type, "\'", tabs = ( tabulations + 1 ) )
      pfw.console.debug.info( "name:   \'", self.__name, "\'", tabs = ( tabulations + 1 ) )
      pfw.console.debug.info( "value:  \'", self.__value, "\'", tabs = ( tabulations + 1 ) )
   # def info

   def type( self ):
      return self.__type
   # def type

   def name( self ):
      return self.__name
   # def type

   def value( self ):
      return self.__value
   # def type



   __type: str = None
   __name: str = None
   __value: str = None
# class Parameter



class Function:
   class eType( Enum ):
      def __str__( self ):
         return str( self.value )

      DEFAULT = "DEFAULT"
      REQUEST = "REQUEST"
      RESPONSE = "RESPONSE"
   # class eType

   def __init__( self, return_type: str = None, name: str = None, type: eType = eType.DEFAULT ):
      self.__return_type = return_type
      self.__name = name
      self.__type = type_builder( type )
      self.__arguments = [ ]
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Function.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in Function.__dict__.keys( ) if i[:2] != pfw.base.class_ignore_field ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      name = "Function { " + ", ".join( vector ) + " }"
      return name
   # def __str__

   def info( self, **kwargs ):
      tabulations: int = kwargs.get( "tabulations", 0 )
      pfw.console.debug.info( self.__class__.__name__, ":", tabs = ( tabulations + 0 ) )
      pfw.console.debug.info( "type:         \'", self.__type, "\'", tabs = ( tabulations + 1 ) )
      pfw.console.debug.info( "return_type:  \'", self.__return_type, "\'", tabs = ( tabulations + 1 ) )
      pfw.console.debug.info( "name:         \'", self.__name, "\'", tabs = ( tabulations + 1 ) )
      for argument in self.__arguments: argument.info( tabulations = tabulations + 1 )
   # def info

   def set_type( self, type: str ):
      self.__type = type_builder( type )
   # def set_type

   def set_return_type( self, return_type: str ):
      self.__return_type = type_builder( return_type )
   # def set_return_type

   def set_name( self, name: str ):
      self.__name = name
   # def set_name

   def set_arguments( self, arguments: list ):
      self.__arguments = arguments if None != arguments else [ ]
   # def set_arguments

   def add_argument( self, argument: Parameter ):
      self.__arguments.append( argument )
   # def add_argument

   def type( self ):
      return self.__type
   # def type

   def return_type( self ):
      return self.__return_type
   # def return_type

   def name( self ):
      return self.__name
   # def name

   def arguments( self ):
      return self.__arguments
   # def arguments



   __type: eType = None
   __return_type: str = None
   __name: str = None
   __arguments: list = [ ]
# class Function



class Connection:
   def __init__( self, request: Function, response: Function = None ):
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
      attr_list = [ i for i in Connection.__dict__.keys( ) if i[:2] != pfw.base.class_ignore_field ]
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

   def set_request( self, request: Function ):
      self.__request = request
      self.__name = self.__request.name( )
   # def set_request

   def set_response( self, response: Function ):
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
   __request: Function = None
   __response: Function = None
# class Connection



class Interface:
   def __init__( self ):
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
      attr_list = [ i for i in Interface.__dict__.keys( ) if i[:2] != pfw.base.class_ignore_field ]
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
      self.__version.info( );
      for connection in self.__connections: connection.info( tabulations = tabulations + 1 )
      for attribute in self.__attributes: attribute.info( tabulations = tabulations + 1 )
   # def info

   def set_version( self, version: Version ):
      self.__version = version
   # def set_version

   def set_name( self, name: str ):
      self.__name = name
   # def set_name

   def add_connection( self, connection: Connection ):
      self.__connections.append( connection )
   # def add_connection

   def add_request( self, request: Function ):
      self.__connections.append( Connection( request ) );
   # def add_request

   def add_response( self, response: Function, request_name: str ):
      for connection in self.__connections:
         if connection.request( ).name( ) == request_name:
            connection.set_response( response )
            return

      pfw.console.debug.error( "request was not defined: ", request_name )
   # def add_response

   def add_attribute( self, attribute: Parameter ):
      self.__attributes.append( attribute )
   # def add_attribute

   def version( self ):
      return self.__version
   # def version

   def name( self ):
      return self.__name
   # def name

   def connections( self ):
      return self.__connections
   # def connections

   def attributes( self ):
      return self.__attributes
   # def attributes



   __version: Version = None
   __name: str = None
   __connections: list = [ ]
   __attributes: list = [ ]
# class Interface
