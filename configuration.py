import os
import sys
import copy
import re
import getopt
import argparse



class ConfigurationData:
   def __init__( self, name: str, required: bool, destination: str ):
      self.__name = copy.deepcopy( name )
      self.__required = copy.deepcopy( required )
      self.__description = copy.deepcopy( destination )
      self.__values = [ ]
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in ConfigurationData.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in ConfigurationData.__dict__.keys( ) if i[:2] != "__" ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      name = "ConfigurationData { " + ", ".join( vector ) + " }"
      return name
   # def __str__

   def __gt__( self, other ):
      return self.__name > other.__name
   # def __gt__

   def __lt__( self, other ):
      return self.__name < other.__name
   # def __lt__

   def __eq__( self, other ):
      return self.__name == other.__name if None != other else False
   # def __eq__

   def info( self, **kwargs ):
      print( self.__class__.__name__, ":" )
      print( "name:         \'", self.__name, "\'" )
      print( "values:       \'", self.__values, "\'" )
      print( "required:     \'", self.__required, "\'" )
      print( "description:  \'", self.__description, "\'" )
   # def info



   def get_name( self ):
      return self.__name
   # def get_name

   def get_value( self, index: int = 0 ):
      return self.__values[ index ] if index < len( self.__values ) else None
   # def get_value

   def get_values( self ):
      return self.__values
   # def get_values

   def set_value( self, value ):
      if None == value:
         return

      values_to_add: list = [ ]
      if isinstance( value, list ) or isinstance( value, tuple ):
         values_to_add = value
      elif isinstance( value, dict ):
         return
      else:
         values_to_add = [ value ]

      self.__values.extend( values_to_add )
   # def set_value

   def reset_value( self, name: str, value = None ):
      self.__values.clear( )

      if None == value:
         self.__values.append( value )
   # def reset_value

   def test_value( self, value ):
      if None == value:
         return False

      values_to_test: list = [ ]
      if isinstance( value, list ) or isinstance( value, tuple ):
         values_to_test = value
      elif isinstance( value, dict ):
         return
      else:
         values_to_test = [ value ]

      for value_to_test in values_to_test:
         if not ( value_to_test in self.__values ):
            return False

      return True
   # def test_value

   def get_required( self ):
      return self.__required
   # def get_required

   def get_description( self ):
      return self.__description
   # def get_description

   def is_satisfy( self ):
      result: bool = 0 < len( self.__values ) if True == self.__required else True

      if False == result:
         print( "configuration variable '%s' is not defined in command line paramenters and configuration file" % ( self.__name ) )
         print( "configuration variable '%s': '%s'" % ( self.__name, self.__description ) )

      return result
   # def is_satisfy



   __name: str = None
   __values: list = [ ]
   __required: bool = False
   __description: str = None
# class ConfigurationData



class ConfigurationContainer:
   def __init__( self, data_list: list = [ ], **kwargs ):
      self.__list = copy.deepcopy( data_list )
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in ConfigurationContainer.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in ConfigurationContainer.__dict__.keys( ) if i[:2] != "__" ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      name = "ConfigurationContainer { " + ", ".join( vector ) + " }"
      return name
   # def __str__

   def info( self, **kwargs ):
      print( self.__class__.__name__, ":" )
      for data in self.__list: data.info( )
   # def info



   def set_data( self, name: str, data: ConfigurationData ):
      self.__list.append( data )
   # def set_data

   def get_data( self, name: str ):
      for data in self.__list:
         if data.get_name( ) == name:
            return data

      return None
   # def get_data

   def test( self, name: str ):
      return None != self.get_data( name )
   # def test

   def delete_data( self, name: str ):
      for index in range( len( self.__list ) ):
         if self.__list[ index ].get_name( ) == name:
            del self.__list[ index ]
   # def delete_data

   def set_value( self, name: str, value ):
      data = self.get_data( name )
      if None == data:
         data = ConfigurationData( name, False, "" )
         self.__list.append( data )

      data.set_value( value )
   # def set_value

   def get_values( self, name: str ):
      data = self.get_data( name )
      return data.get_values( ) if None != data else [ ]
   # def get_values

   def get_value( self, name: str, index: int = 0 ):
      data = self.get_data( name )
      return data.get_value( index ) if None != data else None
   # def get_value

   def get_description( self, name: str ):
      data = self.get_data( name )
      return data.get_description( ) if None != data else None
   # def get_description

   def get_required( self, name: str ):
      data = self.get_data( name )
      return data.get_required( ) if None != data else None
   # def get_required

   def is_complete( self ):
      for data in self.__list:
         if False == data.is_satisfy( ):
            return False

      return True
   # def is_complete



   __list: list = [ ]
# class ConfigurationContainer



def add_config( app_data, name, value ):
   app_data.set_value( name, value )

   if "antlr_outdir" == name or "pfw" == name:
      app_data.set_value( "include", value )
# def add_config



def process_cmdline( app_data, argv ):
   print( "Number of arguments:", len(sys.argv) )
   print( "Argument List:", str(sys.argv) )

   parser = argparse.ArgumentParser( description = 'App description' )

   parser.add_argument( "--version", action = "version", version = '%(prog)s 2.0' )

   parser.add_argument( "--config", dest = "config", type = str, action = "append", required = False, help = app_data.get_description( "config" ) )

   parser.add_argument( "--include", dest = "include", type = str, action = "append", help = app_data.get_description( "include" ) )
   parser.add_argument( "--pfw", dest = "pfw", type = str, action = "append", help = app_data.get_description( "pfw" ) )

   parser.add_argument( "--arch", dest = "arch", type = str, action = "store", required = False, help = app_data.get_description( "arch" ) )
   parser.add_argument( "--os", dest = "os", type = str, action = "store", required = False, help = app_data.get_description( "os" ) )
   parser.add_argument( "--compiler", dest = "compiler", type = str, action = "store", required = False, help = app_data.get_description( "compiler" ) )
   parser.add_argument( "--action", dest = "action", type = str, action = "store", required = False, help = app_data.get_description( "action" ) )
   parser.add_argument( "--target", dest = "target", type = str, action = "append", required = False, help = app_data.get_description( "target" ) )

   parser.add_argument( "--cfg", dest = "cfg", type = str, action = "store", required = False, help = app_data.get_description( "cfg" ) )
   parser.add_argument( "--trace", dest = "trace", type = str, action = "store", required = False, help = app_data.get_description( "trace" ) )

   # parser.print_help( )
   try:
      argument = parser.parse_args( )
   except argparse.ArgumentError:
      print( 'Catching an ArgumentError' )

   for key in argument.__dict__:
      add_config( app_data, key, argument.__dict__[ key ] )
# def process_cmdline



def process_config_file( app_data ):
   pattern: str = r"^\s*(.*)\s*:\s*(.*)\s*$"

   for config_file in app_data.get_values( "config" ):
      config_file_h = open( config_file, "r" )
      for line in config_file_h:
         match = re.match( pattern, line )
         if match:
            add_config( app_data, match.group( 1 ), match.group( 2 ) )
      config_file_h.close( )
# def process_config_file



def process_configuration( app_data, argv ):
   process_cmdline( app_data, argv )
   process_config_file( app_data )
   # app_data.info( )
   if False == app_data.is_complete( ):
      sys.exit( 1 )

   for path in reversed( app_data.get_values( "include" ) ):
      sys.path.insert( 0, path )
# def process_configuration



config: ConfigurationContainer = ConfigurationContainer(
      [
         # ConfigurationData( "env"                         , False , "Environment variables for current script" ),
         ConfigurationData( "config"                      , True  , "Path to configuration file" ),
         ConfigurationData( "include"                     , False , "Additional directory to search import packages" ),
         ConfigurationData( "pfw"                         , True  , "Python Framework directory location" ),
         ConfigurationData( "arch"                        , True  , "Target architecture" ),
         ConfigurationData( "os"                          , True  , "Target OS" ),
         ConfigurationData( "action"                      , True  , "Action" ),
         ConfigurationData( "target"                      , False , "Target for action" ),
         ConfigurationData( "cfg"                         , False , "Configuration file for starting target process" ),
         ConfigurationData( "trace"                       , False , "Trace strategy for starting target process" ),
         ConfigurationData( "source_dir"                  , True  , "Source directory" ),
         ConfigurationData( "tmp_dir"                     , False , "Directory for temporary files and artifacts" ),
         ConfigurationData( "compiler"                    , False , "Set compiler for building (default: gcc)" ),
      ]
   )



def values( name: str ):
   return config.get_values( name )
# def values

def value( name: str, index: int = 0 ):
   return config.get_value( name, index )
# def value



def configure( argv ):
   process_configuration( config, sys.argv[1:] )
# def configure

def info( ):
   config.info( )
# def info
