#!/usr/bin/python

import os
import stat
import time
import console



# link: https://stackoverflow.com/questions/9058305/getting-attributes-of-a-class
# Example:
#     print( class_attributes( ClassName ) )
def class_attributes( cls ):
   result: dict = { "system": { "methods": [ ], "attributes": [ ] }, "user": { "methods": [ ], "attributes": [ ] } }

   s_type: str = None
   f_type: str = None
   for name in cls.__dict__.keys( ):
      s_type = "user"
      if name[:2] == '__': s_type = "system"

      f_type = "attributes"
      if True == callable( cls.__dict__[ name ] ): f_type = "methods"

      result[ s_type ][ f_type ].append( name )

   return result

def string_to_int( string: str, exit_code: int = 1 ) -> int:
   if str != type( string ):
      console.debug.error( "value is not a string: ", string )
      if 0 != exit_code: exit( exit_code )
      else: return None

   try:
      number = int( string )
      return number
   except ValueError:
      console.debug.error( "wrong string value: ", string )
      if 0 != exit_code: exit( exit_code )
      else: return None

def to_string( container, new_line = True ):
   # if True == new_line: print( type( container ) )

   string: str = ""
   if dict == type( container ):
      vector = [ ]
      for key in container:
         vector.append( to_string( key, False ) + " -> " + to_string( container[key], False ) )
      string = "{ " + ", ".join( vector ) + " }"
   elif list == type( container ):
      vector = [ ]
      for item in container:
         vector.append( to_string( item, False ) )
      string = "[ " + ", ".join( vector ) + " ]"
   elif tuple == type( container ):
      vector = [ ]
      for item in container:
         vector.append( to_string( item, False ) )
      string = "( " + ", ".join( vector ) + " )"
   else: string = str( container )

   return string

def file_time( file ):
   try:
      st = os.stat( file )
   except IOError:
      return None
   else:
      return st[ stat.ST_MTIME ]
      # return time.asctime( time.localtime(st[stat.ST_MTIME]) )
