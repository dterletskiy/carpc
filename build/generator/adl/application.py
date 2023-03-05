import pfw.base.struct
import pfw.console



class Component:
   def __init__( self, name: str = None, location: str = None, creator: str = None ):
      self.__name = name
      self.__location = location
      self.__creator = creator
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Component.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in Component.__dict__.keys( ) if i[:2] != pfw.base.struct.ignore_field
 ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      return "Component { " + ", ".join( vector ) + " }"
   # def __str__

   def info( self, **kwargs ):
      tabulations: int = kwargs.get( "tabulations", 0 )
      pfw.console.debug.info( self.__class__.__name__, ":", tabs = ( tabulations + 0 ) )
      pfw.console.debug.info( "name:      \'", self.__name, "\'", tabs = ( tabulations + 1 ) )
      pfw.console.debug.info( "location:  \'", self.__location, "\'", tabs = ( tabulations + 1 ) )
      pfw.console.debug.info( "creator:   \'", self.__creator, "\'", tabs = ( tabulations + 1 ) )
   # def info

   def set_name( self, name: str ):
      self.__name = name
   # def set_name

   def name( self ):
      return self.__name
   # def name

   def set_location( self, location: str ):
      self.__location = location
   # def set_location

   def location( self ):
      return self.__location
   # def location

   def set_creator( self, creator: str ):
      self.__creator = creator
   # def set_creator

   def creator( self ):
      return self.__creator
   # def creator



   __name: str = None
   __location: str = None
   __creator: str = None
# class Component



class Thread:
   def __init__( self, name: str = None, watchdog: int = None ):
      self.__name = name
      self.__watchdog = watchdog
      self.__components = [ ]
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Thread.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in Thread.__dict__.keys( ) if i[:2] != pfw.base.struct.ignore_field
 ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      return "Thread { " + ", ".join( vector ) + " }"
   # def __str__

   def info( self, **kwargs ):
      tabulations: int = kwargs.get( "tabulations", 0 )
      pfw.console.debug.info( self.__class__.__name__, ":", tabs = ( tabulations + 0 ) )
      pfw.console.debug.info( "name:      \'", self.__name, "\'", tabs = ( tabulations + 1 ) )
      pfw.console.debug.info( "watchdog:  \'", self.__watchdog, "\'", tabs = ( tabulations + 1 ) )
      for component in self.__components: component.info( tabulations = tabulations + 1 )
   # def info

   def set_name( self, name: str ):
      self.__name = name
   # def set_name

   def name( self ):
      return self.__name
   # def name

   def set_components( self, components: list ):
      self.__components = components
   # def set_components

   def add_component( self, component: Component ):
      self.__components.append( component )
   # def add_component

   def components( self ):
      return self.__components
   # def components

   def set_watchdog( self, watchdog: int ):
      self.__watchdog = watchdog
   # def set_watchdog

   def watchdog( self ):
      return self.__watchdog
   # def watchdog



   __name: str = None
   __components: list = [ ]
   __watchdog: int = None
# class Thread



class Application:
   def __init__( self, name: str = None ):
      self.__name = name
      self.__threads = [ ]
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Application.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in Application.__dict__.keys( ) if i[:2] != pfw.base.struct.ignore_field
 ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      return "Application { " + ", ".join( vector ) + " }"
   # def __str__

   def info( self, **kwargs ):
      tabulations: int = kwargs.get( "tabulations", 0 )
      pfw.console.debug.info( self.__class__.__name__, ":", tabs = ( tabulations + 0 ) )
      pfw.console.debug.info( "name:      \'", self.__name, "\'", tabs = ( tabulations + 1 ) )
      for thread in self.__threads: thread.info( tabulations = tabulations + 1 )
   # def info

   def set_name( self, name: str ):
      self.__name = name
   # def set_name

   def name( self ):
      return self.__name
   # def name

   def set_threads( self, threads: list ):
      self.__threads = threads
   # def set_threads

   def add_thread( self, thread: Thread ):
      self.__threads.append( thread )
   # def add_thread

   def threads( self ):
      return self.__threads
   # def threads



   __name: str = None
   __threads: list = [ ]
# class Application
