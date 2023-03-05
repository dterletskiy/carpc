#!/usr/bin/python3

import io

from antlr4 import *
from antlr4.error.ErrorListener import *

from AdlLexer import AdlLexer
from AdlParser import AdlParser
from AdlParserListener import AdlParserListener

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


   def enterAuthor( self, ctx: AdlParser.AuthorContext ):
      # pfw.console.debug.trace( "enterAuthor" )
      pass

   def exitAuthor( self, ctx: AdlParser.AuthorContext ):
      # pfw.console.debug.trace( "exitAuthor" )
      pass


   def enterVersion( self, ctx: AdlParser.VersionContext ):
      # pfw.console.debug.trace( "enterVersion" )
      # pfw.console.debug.info( ctx.version_number( ).NUMBER( 0 ).getText( ) )
      # pfw.console.debug.info( ctx.version_number( ).NUMBER( 1 ).getText( ) )
      # pfw.console.debug.info( ctx.version_number( ).NUMBER( 2 ).getText( ) )
      pass

   def exitVersion( self, ctx: AdlParser.VersionContext ):
      # pfw.console.debug.trace( "exitVersion" )
      pass


   def enterComponent( self, ctx: AdlParser.ComponentContext ):
      # pfw.console.debug.info( ctx.IDENTIFIER( ).getText( ) )
      # pfw.console.debug.info( ctx.include( )[-1].MODE_INCLUDE_PATH( ).getText( ) )
      # pfw.console.debug.info( ctx.creator( )[-1].type( ).getText( ) )
      self.__components.append(
            application.Component(
                  ctx.IDENTIFIER( ).getText( ),
                  ctx.include( )[-1].MODE_INCLUDE_PATH( ).getText( ),
                  ctx.creator( )[-1].type_( ).getText( )
               )
         )
      pass

   def enterThread( self, ctx: AdlParser.ThreadContext ):
      # pfw.console.debug.info( ctx.IDENTIFIER( ).getText( ) )
      # pfw.console.debug.info( ctx.watchdog( )[-1].NUMBER( ).getText( ) )
      str_components = [ component.IDENTIFIER( ).getText( ) for component in ctx.components( )[-1].names( ).name( ) ]
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
      self.__application = application.Application( ctx.IDENTIFIER( ).getText( ) )
      if None != ctx.threads( ):
         str_threads = [ thread.IDENTIFIER( ).getText( ) for thread in ctx.threads( ).names( ).name( ) ]
         # for str_thread in str_threads: pfw.console.debug.info( str_thread )

         for str_thread in str_threads:
            for thread in self.__threads:
               if str_thread == thread.name( ):
                  self.__application.add_thread( thread )



   def get_application( self ):
      return self.__application
   # def get_application


   __application: application.Application = None
   __threads: list = [ ]
   __components: list = [ ]

# class AdlListener



class AdlErrorListener( ErrorListener ):
   def __init__( self, output ):
      self.output = output        
      self._symbol = ''

   def syntaxError( self, recognizer, offendingSymbol, line, column, msg, e ):
      pfw.console.debug.error( "syntaxError -->" )
      pfw.console.debug.error( "   ", line, ":", column, "->" )
      pfw.console.debug.error( "   ", "msg: ", msg )
      pfw.console.debug.error( "   ", "recognizer: ", recognizer )
      pfw.console.debug.error( "   ", "offendingSymbol: ", offendingSymbol )
      pfw.console.debug.error( "   ", "e: ", e )
      self.output.write( msg )
      self._symbol = offendingSymbol.text
      pfw.console.debug.error( "syntaxError <--" )

   @property        
   def symbol( self ):
      pfw.console.debug.error( "symbol" )
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

   walker = ParseTreeWalker( )
   walker.walk( listener, tree )

   return listener.get_application( )
# def parse

def generate( source_file: str, gen_outdir: str, api_outdir: str, imp_outdir: str ):
   _application = parse( source_file )

   _builder: builder.Builder = builder.Builder( _application, gen_outdir, api_outdir, imp_outdir )
   _builder.build( )
# def generate
