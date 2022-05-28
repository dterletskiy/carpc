#!/usr/bin/python3

import io

from antlr4 import *
from antlr4.error.ErrorListener import *

from DdlLexer import DdlLexer
from DdlParser import DdlParser
from DdlParserListener import DdlParserListener

import pfw.base
import pfw.console
import pfw.shell

from ddl import data
from ddl import builder



class DdlListener( DdlParserListener ):
   def __init__( self, output ):
      self.output = output

   def enterContent( self, ctx: DdlParser.ContentContext ):
      # pfw.console.debug.trace( "enterContent" )
      pass

   def exitContent( self, ctx: DdlParser.ContentContext ):
      # pfw.console.debug.trace( "exitContent" )
      pass


   def enterElement( self, ctx: DdlParser.ElementContext ):
      # pfw.console.debug.trace( "enterElement" )
      pass

   def exitElement( self, ctx: DdlParser.ElementContext ):
      # pfw.console.debug.trace( "exitElement" )
      pass


   def enterAuthor( self, ctx: DdlParser.AuthorContext ):
      # pfw.console.debug.trace( "enterAuthor" )
      # pfw.console.debug.info( ctx.IDENTIFIER( ).getText( ) )
      pass

   def exitAuthor( self, ctx: DdlParser.AuthorContext ):
      # pfw.console.debug.trace( "exitAuthor" )
      pass


   def enterVersion( self, ctx: DdlParser.VersionContext ):
      pfw.console.debug.trace( "enterVersion" )
      self.__data.set_version(
            pfw.code.Version(
                  ctx.DIGIT( 0 ).getText( ),
                  ctx.DIGIT( 1 ).getText( ),
                  ctx.DIGIT( 2 ).getText( )
               )
         )

   def exitVersion( self, ctx: DdlParser.VersionContext ):
      pfw.console.debug.trace( "exitVersion" )
      pass


   def enterData( self, ctx: DdlParser.DataContext ):
      pfw.console.debug.trace( "enterData" )
      self.__data.set_name( ctx.IDENTIFIER( ).getText( ) )

   def exitData( self, ctx: DdlParser.DataContext ):
      pfw.console.debug.trace( "exitData" )
      self.__data = None


   def enterStruct( self, ctx: DdlParser.StructContext ):
      # pfw.console.debug.trace( "enterStruct" )
      self.__struct = pfw.code.Struct( ctx.IDENTIFIER( ).getText( ) )
      pass

   def exitStruct( self, ctx: DdlParser.StructContext ):
      # pfw.console.debug.trace( "exitStruct" )
      self.__data.add_struct( self.__struct )
      self.__struct = None
      pass


   def enterMethod( self, ctx: DdlParser.MethodContext ):
      # pfw.console.debug.trace( "enterMethod" )
      method = pfw.code.Function(
            ctx.function( ).IDENTIFIER( 0 ).getText( ),
            ctx.function( ).IDENTIFIER( 1 ).getText( )
         )

      for argument in ctx.function( ).arguments_list( ).arguments( ).argument( ):
         method.add_argument(
               pfw.code.Parameter(
                     argument.type_( ).getText( ),
                     argument.name( ).getText( )
                  )
            )

      if None != self.__struct:
         self.__struct.add_method( method )
      elif None != self.__data:
         self.__data.add_method( method )

   def exitMethod( self, ctx: DdlParser.MethodContext ):
      # pfw.console.debug.trace( "exitMethod" )
      pass


   def enterField( self, ctx: DdlParser.FieldContext ):
      # pfw.console.debug.trace( "enterField" )
      field = pfw.code.Parameter(
            ctx.argument( ).type_( ).getText( ),
            ctx.argument( ).name( ).getText( )
         )

      if None != self.__struct:
         self.__struct.add_field( field )
      elif None != self.__data:
         self.__data.add_field( field )

   def exitField( self, ctx: DdlParser.FieldContext ):
      # pfw.console.debug.trace( "exitField" )
      pass



   def set_data( self, _data: data.Data ):
      self.__data = _data



   __data: data.Data = None
   __struct: pfw.code.Struct = None

# class DdlListener



class DdlErrorListener( ErrorListener ):
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

# class DdlErrorListener



def parse( source_file: str ):
   input = FileStream( source_file )


   listener = DdlListener( io.StringIO( ) )
   error_listener = DdlErrorListener( io.StringIO( ) )

   lexer = DdlLexer( input )
   stream = CommonTokenStream( lexer )
   parser = DdlParser( stream )
   parser.removeErrorListeners( )        
   parser.addErrorListener( error_listener )
   tree = parser.content( )

   _data: data.Data = data.Data( )
   listener.set_data( _data )

   walker = ParseTreeWalker( )
   walker.walk( listener, tree )

   return _data
# def parse

def generate( source_file: str, gen_outdir: str, api_outdir: str, imp_outdir: str ):
   _data = parse( source_file )
   _data.info( )

   _builder: builder.Builder = builder.Builder( _data, gen_outdir )
   _builder.build( )
# def generate
