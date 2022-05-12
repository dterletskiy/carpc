#!/usr/bin/python3

import sys
import io
import getopt
import argparse

from antlr4 import *
from antlr4.error.ErrorListener import *

import pfw.console

import interface
import builder



SOURCE_FILE: str = None
GEN_DIR: str = None
API_DIR: str = "api/service/"
IMP_DIR: str = "imp/service/"



def cmd( argv ):
   pfw.console.debug.info( "Number of arguments:", len(sys.argv) )
   pfw.console.debug.info( "Argument List:", str(sys.argv) )

   try:
      opts, args = getopt.getopt(
            argv, "hs:i:g:", [
               "help",
               "source=",
               "import=",
               "gen="
            ]
         )
      pfw.console.debug.info( "opts: ", opts )
      pfw.console.debug.info( "args: ", args )
   except getopt.GetoptError:
      pfw.console.debug.info( "No options" )

   for opt, arg in opts:
      pfw.console.debug.info( "processing: ", opt, " = ", arg )

      if opt in ( "-s", "--source" ):
         pfw.console.debug.header( "Source file: ", arg )
         global SOURCE_FILE
         SOURCE_FILE = arg
      elif opt in ( "-i", "--import" ):
         pfw.console.debug.header( "Import directory: ", arg )
         sys.path.insert( 1, arg )
      elif opt in ( "-g", "--gen" ):
         pfw.console.debug.header( "Output directory: ", arg )
         global GEN_DIR
         GEN_DIR = arg
# def cmd

cmd( sys.argv[1:] )



from IdlLexer import IdlLexer
from IdlParser import IdlParser
from IdlParserListener import IdlParserListener



class IdlListener( IdlParserListener ) :
   def __init__( self, output ):
      self.output = output

   def enterContent( self, ctx: IdlParser.ContentContext ):
      pfw.console.debug.trace( "enterContent" )

   def exitContent( self, ctx: IdlParser.ContentContext ):
      pfw.console.debug.trace( "exitContent" )


   def enterElement( self, ctx: IdlParser.ElementContext ):
      pfw.console.debug.trace( "enterElement" )

   def exitElement( self, ctx: IdlParser.ElementContext ):
      pfw.console.debug.trace( "exitElement" )


   def enterInterface( self, ctx: IdlParser.InterfaceContext ):
      pfw.console.debug.trace( "enterInterface" )
      pfw.console.debug.info( ctx.IDENTIFIER( ).getText( ) )
      self.__interface.set_name( ctx.IDENTIFIER( ).getText( ) )

   def exitInterface( self, ctx: IdlParser.InterfaceContext ):
      pfw.console.debug.trace( "exitInterface" )


   def enterVersion( self, ctx: IdlParser.VersionContext ):
      pfw.console.debug.trace( "enterVersion" )
      pfw.console.debug.info( ctx.DIGIT( )[0].getText( ) )
      pfw.console.debug.info( ctx.DIGIT( )[1].getText( ) )
      pfw.console.debug.info( ctx.DIGIT( )[2].getText( ) )
      self.__interface.set_version(
            interface.Version(
                  ctx.DIGIT( )[0].getText( ),
                  ctx.DIGIT( )[1].getText( ),
                  ctx.DIGIT( )[2].getText( )
               )
         )

   def exitVersion( self, ctx:IdlParser.VersionContext ):
      pfw.console.debug.trace( "exitVersion" )


   def enterAuthor( self, ctx: IdlParser.AuthorContext ):
      pfw.console.debug.trace( "enterAuthor" )
      pfw.console.debug.info( ctx.IDENTIFIER( ).getText( ) )

   def exitAuthor( self, ctx: IdlParser.AuthorContext ):
      pfw.console.debug.trace( "exitAuthor" )


   def enterRequest( self, ctx: IdlParser.RequestContext ):
      pfw.console.debug.trace( "enterRequest" )

   def exitRequest( self, ctx: IdlParser.RequestContext ):
      pfw.console.debug.trace( "exitRequest" )
      self.__function.set_type( interface.Function.eType.REQUEST )
      self.__interface.add_request( self.__function )
      self.__function = None


   def enterResponse( self, ctx: IdlParser.ResponseContext ):
      pfw.console.debug.trace( "enterResponse" )

   def exitResponse( self, ctx: IdlParser.ResponseContext ):
      pfw.console.debug.trace( "exitResponse" )
      self.__function.set_type( interface.Function.eType.RESPONSE )
      self.__interface.add_response( self.__function, self.__to_request_name )
      self.__function = None
      self.__to_request_name = None


   def enterConnection( self, ctx: IdlParser.ConnectionContext ):
      pfw.console.debug.trace( "enterConnection" )
      pfw.console.debug.info( ctx.IDENTIFIER( ).getText( ) )
      self.__to_request_name = ctx.IDENTIFIER( ).getText( )

   def exitConnection( self, ctx: IdlParser.ConnectionContext ):
      pfw.console.debug.trace( "exitConnection" )


   def enterMethod( self, ctx: IdlParser.MethodContext ):
      pfw.console.debug.trace( "enterMethod" )

   def exitMethod( self, ctx: IdlParser.MethodContext ):
      pfw.console.debug.trace( "exitMethod" )
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
      pfw.console.debug.trace( "enterCallback" )

   def exitCallback( self, ctx: IdlParser.CallbackContext ):
      pfw.console.debug.trace( "exitCallback" )


   def enterAttribute( self, ctx: IdlParser.AttributeContext ):
      pfw.console.debug.trace( "enterAttribute" )
      pfw.console.debug.info( ctx.IDENTIFIER( )[0].getText( ) )
      pfw.console.debug.info( ctx.IDENTIFIER( )[1].getText( ) )
      self.__interface.add_attribute(
            interface.Parameter( ctx.IDENTIFIER( )[0].getText( ),
            ctx.IDENTIFIER( )[1].getText( ) )
         )

   def exitAttribute( self, ctx: IdlParser.AttributeContext ):
      pfw.console.debug.trace( "exitAttribute" )


   def enterProcedure( self, ctx: IdlParser.ProcedureContext ):
      pfw.console.debug.trace( "enterProcedure" )
      pfw.console.debug.info( ctx.IDENTIFIER( ).getText( ) )
      self.__function = interface.Function(
            None,
            ctx.IDENTIFIER( ).getText( )
         )

   def exitProcedure( self, ctx: IdlParser.ProcedureContext ):
      pfw.console.debug.trace( "exitProcedure" )
      self.__function.set_arguments( self.__arguments )
      self.__arguments = None


   def enterFunction( self, ctx: IdlParser.FunctionContext ):
      pfw.console.debug.trace( "enterFunction" )
      pfw.console.debug.info( ctx.IDENTIFIER( )[0].getText( ) )
      pfw.console.debug.info( ctx.IDENTIFIER( )[1].getText( ) )
      self.__function = interface.Function(
            ctx.IDENTIFIER( )[0].getText( ),
            ctx.IDENTIFIER( )[1].getText( )
         )

   def exitFunction( self, ctx: IdlParser.FunctionContext ):
      pfw.console.debug.trace( "exitFunction" )
      self.__function.set_arguments( self.__arguments )
      self.__arguments = None


   def enterArguments_list( self, ctx: IdlParser.Arguments_listContext ):
      pfw.console.debug.trace( "enterArguments_list" )

   def exitArguments_list( self, ctx: IdlParser.Arguments_listContext ):
      pfw.console.debug.trace( "exitArguments_list" )


   def enterArguments( self, ctx: IdlParser.ArgumentsContext ):
      pfw.console.debug.trace( "enterArguments" )
      self.__arguments = [ ]

   def exitArguments( self, ctx: IdlParser.ArgumentsContext ):
      pfw.console.debug.trace( "exitArguments" )


   def enterArgument( self, ctx: IdlParser.ArgumentContext ):
      pfw.console.debug.trace( "enterArgument" )
      pfw.console.debug.info( ctx.IDENTIFIER( )[0].getText( ) )
      pfw.console.debug.info( ctx.IDENTIFIER( )[1].getText( ) )
      self.__arguments.append(
            interface.Parameter(
                  ctx.IDENTIFIER( )[0].getText( ),
                  ctx.IDENTIFIER( )[1].getText( )
               )
         )

   def exitArgument( self, ctx: IdlParser.ArgumentContext ):
      pfw.console.debug.trace( "exitArgument" )



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
   input = FileStream( SOURCE_FILE )


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

   onoff_interface.info( )
   onoff_builder: builder.Builder = builder.Builder( onoff_interface, GEN_DIR, API_DIR, IMP_DIR )
   onoff_builder.info( )
   onoff_builder.build( )


# def main



if __name__ == '__main__':
   main( )
