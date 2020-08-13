CSI = '\033['
OSC = '\033]'
BEL = '\007'



def code_to_chars( code ):
   return CSI + str(code) + 'm'

def set_title( title ):
   return OSC + '2;' + title + BEL

def clear_screen( mode = 2 ):
   return CSI + str(mode) + 'J'

def clear_line( mode = 2 ):
   return CSI + str(mode) + 'K'

class AnsiCodes( object ):
   def __init__( self ):
      # link: https://github.com/tartley/colorama/tree/master/colorama
      # the subclasses declare class attributes which are numbers.
      # Upon instantiation we define instance attributes, which are the same
      # as the class attributes but wrapped with the ANSI escape sequence
      for name in dir( self ):
         if not name.startswith('_'):
            value = getattr( self, name )
            setattr( self, name, code_to_chars( value ) )

class AnsiCursor( object ):
   def UP(self, n=1):
      return CSI + str(n) + 'A'
   def DOWN(self, n=1):
      return CSI + str(n) + 'B'
   def FORWARD(self, n=1):
      return CSI + str(n) + 'C'
   def BACK(self, n=1):
      return CSI + str(n) + 'D'
   def POS(self, x=1, y=1):
      return CSI + str(y) + ';' + str(x) + 'H'

class AnsiForeground( AnsiCodes ):
   BLACK          = 30
   RED            = 31
   GREEN          = 32
   YELLOW         = 33
   BLUE           = 34
   MAGENTA        = 35
   CYAN           = 36
   WHITE          = 37
   RESET          = 39
   LIGHTBLACK     = 90
   LIGHTRED       = 91
   LIGHTGREEN     = 92
   LIGHTYELLOW    = 93
   LIGHTBLUE      = 94
   LIGHTMAGENTA   = 95
   LIGHTCYAN      = 96
   LIGHTWHITE     = 97

class AnsiBackground( AnsiCodes ):
   BLACK          = 40
   RED            = 41
   GREEN          = 42
   YELLOW         = 43
   BLUE           = 44
   MAGENTA        = 45
   CYAN           = 46
   WHITE          = 47
   RESET          = 49
   LIGHTBLACK     = 100
   LIGHTRED       = 101
   LIGHTGREEN     = 102
   LIGHTYELLOW    = 103
   LIGHTBLUE      = 104
   LIGHTMAGENTA   = 105
   LIGHTCYAN      = 106
   LIGHTWHITE     = 107


class AnsiStyle( AnsiCodes ):
   BOLD           = 1
   DIM            = 2
   UNDERLINED     = 4
   BLINK          = 5
   REVERSE        = 7 # invert the foreground and background colors
   HIDDEN         = 8 # useful for passwords
   RESET          = 0



Foreground        = AnsiForeground( )
Background        = AnsiBackground( )
Style             = AnsiStyle( )
Cursor            = AnsiCursor( )




class AnsiFormat:
   QUESTION       = Foreground.MAGENTA
   PROMT          = Foreground.MAGENTA
   INTERACTION    = Foreground.MAGENTA
   HEADER         = Foreground.LIGHTBLUE
   TRACE          = Foreground.RESET
   INFO           = Foreground.LIGHTYELLOW
   OK             = Foreground.GREEN
   ERROR          = Foreground.RED
   WARNING        = Foreground.LIGHTBLUE
   RESET          = Background.RESET + Foreground.RESET + Style.RESET

Format            = AnsiFormat( )



class AnsiDebug:
   def __init__( self, is_colored: bool = True ):
      self.__is_colored = is_colored

   def header( self, *arguments ):
      self.write( Format.HEADER, *arguments )

   def question( self, *arguments ):
      self.write( Format.QUESTION, *arguments )

   def promt( self, *arguments ):
      self.write( Format.PROMT, *arguments )

   def trace( self, *arguments ):
      self.write( Format.TRACE, *arguments )

   def info( self, *arguments ):
      self.write( Format.INFO, *arguments )

   def ok( self, *arguments ):
      self.write( Format.OK, *arguments )

   def warning( self, *arguments ):
      self.write( Format.WARNING, *arguments )

   def error( self, *arguments ):
      self.write( Format.ERROR, *arguments )

   def write( self, ansi_format, *arguments ):
      string: str = ""
      if True == self.__is_colored:
         string: str = ansi_format

      for argument in arguments:
         string += str( argument )

      if True == self.__is_colored:
         string += Format.RESET
      print( string )

   def promt( self, string: str ):
      if True == self.__is_colored:
         return input( Format.PROMT + string + Format.RESET )
      else:
         return input( string )

   def colored( self, is_colored: bool ):
      _is_colored = self.__is_colored
      self.__is_colored = is_colored
      return _is_colored

   __is_colored: bool = True

debug             = AnsiDebug( True )


