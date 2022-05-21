#!/usr/bin/python3

import io

from antlr4 import *
from antlr4.error.ErrorListener import *

from AdlLexer import AdlLexer
from AdlParser import AdlParser
from AdlParserListener import AdlParserListener

import pfw.base
import pfw.console
import pfw.shell

from adl import application
from adl import builder



class AdlListener( AdlParserListener ):
   def __init__( self, output ):
      self.output = output

   def enterContent( self, ctx: AdlParser.ContentContext ):
      # pfw.console.debug.trace( "enterContent" )
      pass

   def exitContent( self, ctx: AdlParser.ContentContext ):
      # pfw.console.debug.trace( "exitContent" )
      pass


   def enterElement( self, ctx: AdlParser.ElementContext ):
      # pfw.console.debug.trace( "enterElement" )
      pass

   def exitElement( self, ctx: AdlParser.ElementContext ):
      # pfw.console.debug.trace( "exitElement" )
      pass


   def enterComponent( self, ctx: AdlParser.ComponentContext ):
      # pfw.console.debug.info( ctx.IDENTIFIER( ).getText( ) )
      # pfw.console.debug.info( ctx.include( )[-1].PATH( ).getText( ) )
      # pfw.console.debug.info( ctx.creator( )[-1].NAMESPACE_IDENTIFIER( ).getText( ) )
      self.__components.append(
            application.Component(
                  ctx.IDENTIFIER( ).getText( ),
                  ctx.include( )[-1].PATH( ).getText( ),
                  ctx.creator( )[-1].NAMESPACE_IDENTIFIER( ).getText( )
               )
         )
      pass

   def enterThread( self, ctx: AdlParser.ThreadContext ):
      # pfw.console.debug.info( ctx.IDENTIFIER( ).getText( ) )
      # pfw.console.debug.info( ctx.watchdog( )[-1].NUMBER( ).getText( ) )
      str_components = [ component.IDENTIFIER( ).getText( ) for component in ctx.components( )[-1].arguments( ).argument( ) ]
      # for str_component in str_components: pfw.console.debug.info( str_component )

      self.__threads.append(
            application.Thread(
                  ctx.IDENTIFIER( ).getText( ),
                  int( ctx.watchdog( )[-1].NUMBER( ).getText( ) )
               )
         )
      for str_component in str_components:
         for component in self.__components:
            if str_component == component.name( ):
               self.__threads[-1].add_component( component )

   def enterApplication( self, ctx: AdlParser.ApplicationContext ):
      # pfw.console.debug.info( ctx.IDENTIFIER( ).getText( ) )
      str_threads = [ thread.IDENTIFIER( ).getText( ) for thread in ctx.threads( ).arguments( ).argument( ) ]
      # for str_thread in str_threads: pfw.console.debug.info( str_thread )

      for str_thread in str_threads:
         for thread in self.__threads:
            if str_thread == thread.name( ):
               self.__application.add_thread( thread )



   def set_application( self, _application: application.Application ):
      self.__application = _application


   __application: application.Application = None
   __threads: list = [ ]
   __components: list = [ ]

# class AdlListener



class AdlErrorListener( ErrorListener ):
   def __init__( self, output ):
      self.output = output        
      self._symbol = ''

   def syntaxError( self, recognizer, offendingSymbol, line, column, msg, e ):
      print( "syntaxError -->" )
      print( "   ", line, ":", column, "->" )
      print( "   ", "msg: ", msg )
      print( "   ", "recognizer: ", recognizer )
      print( "   ", "offendingSymbol: ", offendingSymbol )
      print( "   ", "e: ", e )
      self.output.write( msg )
      self._symbol = offendingSymbol.text
      print( "syntaxError <--" )

   @property        
   def symbol( self ):
      print( "symbol" )
      return self._symbol

# class AdlErrorListener



def parse( source_file: str ):
   input = FileStream( source_file )


   listener = AdlListener( io.StringIO( ) )
   error_listener = AdlErrorListener( io.StringIO( ) )

   lexer = AdlLexer( input )
   stream = CommonTokenStream( lexer )
   parser = AdlParser( stream )
   parser.removeErrorListeners( )        
   parser.addErrorListener( error_listener )
   tree = parser.content( )

   _application: application.Application = application.Application( )
   listener.set_application( _application )

   walker = ParseTreeWalker( )
   walker.walk( listener, tree )

   return _application
# def parse

def generate( source_file: str, gen_outdir: str, api_outdir: str, imp_outdir: str ):
   _application = parse( source_file )

   _builder: builder.Builder = builder.Builder( _application, gen_outdir )
   _builder.build( )
# def generate
