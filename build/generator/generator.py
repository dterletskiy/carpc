#!/usr/bin/python3

import os
import sys
import io
import getopt
import argparse

from antlr4 import *
from antlr4.error.ErrorListener import *



# ./generator.py \
#    --include=/mnt/dev/TDA \
#    --antlr_jar=/mnt/dev/TDA/rpc/thirdparty/antlr/antlr-4.10.1-complete.jar \
#    --antlr_lexer=/mnt/dev/TDA/rpc/build/generator/source/grammar/IdlLexer.g4  \
#    --antlr_parser=/mnt/dev/TDA/rpc/build/generator/source/grammar/IdlParser.g4 \
#    --antlr_outdir=/mnt/dev/TDA/rpc/build/generator/source/build \
#    --source=/mnt/dev/TDA/rpc/build/generator/source/idl/onoff.idl \
#    --gen_outdir=/mnt/dev/TDA/rpc/service \
#    --api_outdir=api/service \
#    --imp_outdir=imp/service




class ApplicationData:
   antlr_jar: str = None
   include_dirs: list = [ ]
   antlr_lexer: list = [ ]
   antlr_parser: list = [ ]
   antlr_outdir: str = None
   source_file: str = None
   gen_outdir: str = None
   api_outdir: str = None
   imp_outdir: str = None
# class ApplicationData




class Description:
   help = "Show this help menu."
   include = "Additional directory to search import packages"
   antlr_jar = "Path to antlr jar file"
   antlr_lexer = "Path to lexer grammer files"
   antlr_parser = "Path to parser grammer files"
   antlr_outdir = "Output directory for generated lexer, parser, listener and visitor files."
   source = "Path to source file for data generation"
   gen_outdir = "Output directory for generated c++ api files."
   api_outdir = "Output subdirectory for generated c++ api files."
   imp_outdir = "Output subdirectory for generated c++ imp files."
# class Description
g_description = Description( )



def cmdline_argparse( argv ):
   application_data = ApplicationData( )

   parser = argparse.ArgumentParser( description = 'App description' )

   parser.add_argument( "--include", dest = "include", type = str, action = "append",
      help = g_description.include )

   parser.add_argument( "--antlr_jar", dest = "antlr_jar", type = str, action = "store",
      help = g_description.antlr_jar )

   parser.add_argument( "--antlr_lexer", dest = "antlr_lexer", type = str, action = "append",
      help = g_description.antlr_lexer )

   parser.add_argument( "--antlr_parser", dest = "antlr_parser", type = str, action = "append",
      help = g_description.antlr_parser )

   parser.add_argument( "--antlr_outdir", dest = "antlr_outdir", type = str, action = "store",
      help = g_description.antlr_outdir )

   parser.add_argument( "--source", dest = "source", type = str, action = "store",
      help = g_description.source )

   parser.add_argument( "--gen_outdir", dest = "gen_outdir", type = str, action = "store",
      help = g_description.gen_outdir )

   parser.add_argument( "--api_outdir", dest = "api_outdir", type = str, action = "store",
      help = g_description.api_outdir )

   parser.add_argument( "--imp_outdir", dest = "imp_outdir", type = str, action = "store",
      help = g_description.imp_outdir )

   # parser.print_help( )
   try:
      argument = parser.parse_args( )
   except argparse.ArgumentError:
      print( 'Catching an ArgumentError' )

   if argument.include:
      print( "include: ", argument.include )
      application_data.include_dirs.extend( argument.include )

   if argument.antlr_jar:
      print( "antlr_jar: ", argument.antlr_jar )
      application_data.antlr_jar = argument.antlr_jar

   if argument.antlr_lexer:
      print( "antlr_lexer: ", argument.antlr_lexer )
      application_data.antlr_lexer.extend( argument.antlr_lexer )

   if argument.antlr_parser:
      print( "antlr_parser: ", argument.antlr_parser )
      application_data.antlr_parser.extend( argument.antlr_parser )

   if argument.antlr_outdir:
      print( "antlr_outdir: ", argument.antlr_outdir )
      application_data.include_dirs.append( argument.antlr_outdir )
      application_data.antlr_outdir = argument.antlr_outdir

   if argument.source:
      print( "source: ", argument.source )
      application_data.source_file = argument.source

   if argument.gen_outdir:
      print( "gen_outdir: ", argument.gen_outdir )
      application_data.gen_outdir = argument.gen_outdir

   if argument.api_outdir:
      print( "api_outdir: ", argument.api_outdir )
      application_data.api_outdir = argument.api_outdir

   if argument.imp_outdir:
      print( "imp_outdir: ", argument.imp_outdir )
      application_data.imp_outdir = argument.imp_outdir

   return application_data
# def cmdline_argparse

g_application_data = cmdline_argparse( sys.argv[1:] )



def configure( app_data ):
   include_count: int = 0
   for path in app_data.include_dirs:
      include_count += 1
      sys.path.insert( include_count, path )
# def configure

configure( g_application_data )



import pfw.base
import pfw.console
import pfw.shell



def antlr_gen( app_data ):
   if None == app_data.antlr_jar:
      return

   os.makedirs( app_data.antlr_outdir, exist_ok = True )

   for g4_file in app_data.antlr_lexer:
      pfw.shell.run_and_wait_with_status(
            "java", "-jar", app_data.antlr_jar, "-Dlanguage=Python3",
            g4_file,
            "-o", app_data.antlr_outdir,
            "-listener", "-visitor"
         )

   for g4_file in app_data.antlr_parser:
      pfw.shell.run_and_wait_with_status(
            "java", "-jar", app_data.antlr_jar, "-Dlanguage=Python3",
            g4_file,
            "-o", app_data.antlr_outdir,
            "-listener", "-visitor"
         )
# def antlr_gen

antlr_gen( g_application_data )



from IdlLexer import IdlLexer
from IdlParser import IdlParser
from IdlParserListener import IdlParserListener

import interface
import builder



class IdlListener( IdlParserListener ):
   def __init__( self, output ):
      self.output = output

   def enterContent( self, ctx: IdlParser.ContentContext ):
      # pfw.console.debug.trace( "enterContent" )
      pass

   def exitContent( self, ctx: IdlParser.ContentContext ):
      # pfw.console.debug.trace( "exitContent" )
      pass


   def enterElement( self, ctx: IdlParser.ElementContext ):
      # pfw.console.debug.trace( "enterElement" )
      pass

   def exitElement( self, ctx: IdlParser.ElementContext ):
      # pfw.console.debug.trace( "exitElement" )
      pass


   def enterInterface( self, ctx: IdlParser.InterfaceContext ):
      # pfw.console.debug.trace( "enterInterface" )
      # pfw.console.debug.info( ctx.IDENTIFIER( ).getText( ) )
      self.__interface.set_name( ctx.IDENTIFIER( ).getText( ) )

   def exitInterface( self, ctx: IdlParser.InterfaceContext ):
      # pfw.console.debug.trace( "exitInterface" )
      pass


   def enterVersion( self, ctx: IdlParser.VersionContext ):
      # pfw.console.debug.trace( "enterVersion" )
      # pfw.console.debug.info( ctx.DIGIT( )[0].getText( ) )
      # pfw.console.debug.info( ctx.DIGIT( )[1].getText( ) )
      # pfw.console.debug.info( ctx.DIGIT( )[2].getText( ) )
      self.__interface.set_version(
            interface.Version(
                  ctx.DIGIT( )[0].getText( ),
                  ctx.DIGIT( )[1].getText( ),
                  ctx.DIGIT( )[2].getText( )
               )
         )

   def exitVersion( self, ctx: IdlParser.VersionContext ):
      # pfw.console.debug.trace( "exitVersion" )
      pass


   def enterIpc( self, ctx: IdlParser.IpcContext ):
      # pfw.console.debug.trace( "enterIpc" )
      # pfw.console.debug.trace( ctx.TRUE( ) )
      # pfw.console.debug.trace( ctx.FALSE( ) )
      if None != ctx.TRUE( ):
         self.__interface.set_is_ipc( True )
      elif None != ctx.FALSE( ):
         self.__interface.set_is_ipc( False )

   def exitIpc( self, ctx: IdlParser.IpcContext ):
      # pfw.console.debug.trace( "exitIpc" )
      pass


   def enterAuthor( self, ctx: IdlParser.AuthorContext ):
      # pfw.console.debug.trace( "enterAuthor" )
      # pfw.console.debug.info( ctx.IDENTIFIER( ).getText( ) )
      pass

   def exitAuthor( self, ctx: IdlParser.AuthorContext ):
      # pfw.console.debug.trace( "exitAuthor" )
      pass


   def enterRequest( self, ctx: IdlParser.RequestContext ):
      # pfw.console.debug.trace( "enterRequest" )
      pass

   def exitRequest( self, ctx: IdlParser.RequestContext ):
      # pfw.console.debug.trace( "exitRequest" )
      self.__function.set_type( interface.Function.eType.REQUEST )
      self.__interface.add_request( self.__function )
      self.__function = None


   def enterResponse( self, ctx: IdlParser.ResponseContext ):
      # pfw.console.debug.trace( "enterResponse" )
      pass

   def exitResponse( self, ctx: IdlParser.ResponseContext ):
      # pfw.console.debug.trace( "exitResponse" )
      self.__function.set_type( interface.Function.eType.RESPONSE )
      self.__interface.add_response( self.__function, self.__to_request_name )
      self.__function = None
      self.__to_request_name = None


   def enterConnection( self, ctx: IdlParser.ConnectionContext ):
      # pfw.console.debug.trace( "enterConnection" )
      # pfw.console.debug.info( ctx.IDENTIFIER( ).getText( ) )
      self.__to_request_name = ctx.IDENTIFIER( ).getText( )

   def exitConnection( self, ctx: IdlParser.ConnectionContext ):
      # pfw.console.debug.trace( "exitConnection" )
      pass


   def enterMethod( self, ctx: IdlParser.MethodContext ):
      # pfw.console.debug.trace( "enterMethod" )
      pass

   def exitMethod( self, ctx: IdlParser.MethodContext ):
      # pfw.console.debug.trace( "exitMethod" )
      request = self.__function
      request.set_type( interface.Function.eType.REQUEST )
      response = None
      if None != self.__arguments:
         response = interface.Function( None, request.name( ), interface.Function.eType.RESPONSE )
         response.set_arguments( self.__arguments )
      connection = interface.Connection( request, response )
      self.__interface.add_connection( connection )

      self.__function = None
      self.__arguments = None


   def enterCallback( self, ctx: IdlParser.CallbackContext ):
      # pfw.console.debug.trace( "enterCallback" )
      pass

   def exitCallback( self, ctx: IdlParser.CallbackContext ):
      # pfw.console.debug.trace( "exitCallback" )
      pass


   def enterAttribute( self, ctx: IdlParser.AttributeContext ):
      # pfw.console.debug.trace( "enterAttribute" )
      # pfw.console.debug.info( ctx.IDENTIFIER( )[0].getText( ) )
      # pfw.console.debug.info( ctx.IDENTIFIER( )[1].getText( ) )
      self.__interface.add_attribute(
            interface.Parameter( ctx.IDENTIFIER( )[0].getText( ),
            ctx.IDENTIFIER( )[1].getText( ) )
         )

   def exitAttribute( self, ctx: IdlParser.AttributeContext ):
      # pfw.console.debug.trace( "exitAttribute" )
      pass


   def enterProcedure( self, ctx: IdlParser.ProcedureContext ):
      # pfw.console.debug.trace( "enterProcedure" )
      # pfw.console.debug.info( ctx.IDENTIFIER( ).getText( ) )
      self.__function = interface.Function(
            None,
            ctx.IDENTIFIER( ).getText( )
         )

   def exitProcedure( self, ctx: IdlParser.ProcedureContext ):
      # pfw.console.debug.trace( "exitProcedure" )
      self.__function.set_arguments( self.__arguments )
      self.__arguments = None


   def enterFunction( self, ctx: IdlParser.FunctionContext ):
      # pfw.console.debug.trace( "enterFunction" )
      # pfw.console.debug.info( ctx.IDENTIFIER( )[0].getText( ) )
      # pfw.console.debug.info( ctx.IDENTIFIER( )[1].getText( ) )
      self.__function = interface.Function(
            ctx.IDENTIFIER( )[0].getText( ),
            ctx.IDENTIFIER( )[1].getText( )
         )

   def exitFunction( self, ctx: IdlParser.FunctionContext ):
      # pfw.console.debug.trace( "exitFunction" )
      self.__function.set_arguments( self.__arguments )
      self.__arguments = None


   def enterArguments_list( self, ctx: IdlParser.Arguments_listContext ):
      # pfw.console.debug.trace( "enterArguments_list" )
      pass

   def exitArguments_list( self, ctx: IdlParser.Arguments_listContext ):
      # pfw.console.debug.trace( "exitArguments_list" )
      pass


   def enterArguments( self, ctx: IdlParser.ArgumentsContext ):
      # pfw.console.debug.trace( "enterArguments" )
      self.__arguments = [ ]

   def exitArguments( self, ctx: IdlParser.ArgumentsContext ):
      # pfw.console.debug.trace( "exitArguments" )
      pass


   def enterArgument( self, ctx: IdlParser.ArgumentContext ):
      # pfw.console.debug.trace( "enterArgument" )
      # pfw.console.debug.info( ctx.IDENTIFIER( )[0].getText( ) )
      # pfw.console.debug.info( ctx.IDENTIFIER( )[1].getText( ) )
      self.__arguments.append(
            interface.Parameter(
                  ctx.IDENTIFIER( )[0].getText( ),
                  ctx.IDENTIFIER( )[1].getText( )
               )
         )

   def exitArgument( self, ctx: IdlParser.ArgumentContext ):
      # pfw.console.debug.trace( "exitArgument" )
      pass



   def set_interface( self, interface: interface.Interface ):
      self.__interface = interface
   # def set_interface



   __interface: interface.Interface = None
   __function: interface.Function = None
   __arguments: list = [ ]
   __to_request_name: str = None

# class IdlListener



class IdlErrorListener( ErrorListener ):
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



def main( ):
   input = FileStream( g_application_data.source_file )


   listener = IdlListener( io.StringIO( ) )
   error_listener = IdlErrorListener( io.StringIO( ) )

   lexer = IdlLexer( input )
   stream = CommonTokenStream( lexer )
   parser = IdlParser( stream )
   parser.removeErrorListeners( )        
   parser.addErrorListener( error_listener )
   tree = parser.content( )

   onoff_interface: interface.Interface = interface.Interface( )
   listener.set_interface( onoff_interface )

   walker = ParseTreeWalker( )
   walker.walk( listener, tree )

   onoff_builder: builder.Builder = \
      builder.Builder( onoff_interface, g_application_data.gen_outdir, g_application_data.api_outdir, g_application_data.imp_outdir )
   onoff_builder.build( )

# def main



if __name__ == '__main__':
   main( )
