import pfw.base.struct
import pfw.console
import pfw.code



class Data:
   def __init__( self, name: str = None ):
      self.__name = name
      self.__structs = [ ]
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Data.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in Data.__dict__.keys( ) if i[:2] != pfw.base.struct.ignore_field
 ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      return "Data { " + ", ".join( vector ) + " }"
   # def __str__

   def info( self, **kwargs ):
      tabulations: int = kwargs.get( "tabulations", 0 )
      pfw.console.debug.info( self.__class__.__name__, ":", tabs = ( tabulations + 0 ) )
      pfw.console.debug.info( "name:      \'", self.__name, "\'", tabs = ( tabulations + 1 ) )
      self.__version.info( tabulations = tabulations + 1 )
      for struct in self.__structs: struct.info( tabulations = tabulations + 1 )
   # def info

   def set_name( self, name: str ):
      self.__name = name
   # def set_name

   def name( self ):
      return self.__name
   # def name

   def set_version( self, version: pfw.code.Version ):
      self.__version = version
   # def set_version

   def version( self ):
      return self.__version
   # def version

   def add_struct( self, struct: pfw.code.Struct ):
      self.__structs.append( struct )
   # def add_struct

   def structs( self ):
      return self.__structs
   # def structs

   def namespace( self ):
      # return "service::" + self.__name + "::V" + self.__version.string( "_" )
      return "service::" + self.__name
   # def namespace



   __name: str = None
   __version: pfw.code.Version = None
   __structs: list = [ ]
# class Data
