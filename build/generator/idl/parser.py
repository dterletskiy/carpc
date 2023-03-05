#!/usr/bin/python3

import io

from antlr4 import *
from antlr4.error.ErrorListener import *

from IdlLexer import IdlLexer
from IdlParser import IdlParser
from IdlParserListener import IdlParserListener

import pfw.console
import pfw.shell

from idl import interface
from idl import builder



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


   def enterAuthor( self, ctx: IdlParser.AuthorContext ):
      # pfw.console.debug.trace( "enterAuthor" )
      pass

   def exitAuthor( self, ctx: IdlParser.AuthorContext ):
      # pfw.console.debug.trace( "exitAuthor" )
      pass


   def enterVersion( self, ctx: IdlParser.VersionContext ):
      # pfw.console.debug.trace( "enterVersion" )
      self.__interface.set_version(
            pfw.code.Version(
                  ctx.version_number( ).NUMBER( 0 ).getText( ),
                  ctx.version_number( ).NUMBER( 1 ).getText( ),
                  ctx.version_number( ).NUMBER( 2 ).getText( )
               )
         )

   def exitVersion( self, ctx: IdlParser.VersionContext ):
      # pfw.console.debug.trace( "exitVersion" )
      pass


   def enterImport_xdl( self, ctx: IdlParser.Import_xdlContext ):
      # pfw.console.debug.trace( "enterImport_xdl" )
      self.__interface.add_include( ctx.MODE_IMPORT_PATH( ).getText( ) )
      pass

   def exitImport_xdl( self, ctx: IdlParser.Import_xdlContext ):
      # pfw.console.debug.trace( "exitImport_xdl" )
      pass


   def enterInterface( self, ctx: IdlParser.InterfaceContext ):
      # pfw.console.debug.trace( "enterInterface" )
      self.__interface = interface.Interface( ctx.IDENTIFIER( ).getText( ) )

   def exitInterface( self, ctx: IdlParser.InterfaceContext ):
      # pfw.console.debug.trace( "exitInterface" )
      pass

   def exitVersion( self, ctx: IdlParser.VersionContext ):
      # pfw.console.debug.trace( "exitVersion" )
      pass


   def enterIpc( self, ctx: IdlParser.IpcContext ):
      # pfw.console.debug.trace( "enterIpc" )
      if None != ctx.TRUE( ):
         self.__interface.set_is_ipc( True )
      elif None != ctx.FALSE( ):
         self.__interface.set_is_ipc( False )

   def exitIpc( self, ctx: IdlParser.IpcContext ):
      # pfw.console.debug.trace( "exitIpc" )
      pass


   def enterRequest( self, ctx: IdlParser.RequestContext ):
      # pfw.console.debug.trace( "enterRequest" )
      pass

   def exitRequest( self, ctx: IdlParser.RequestContext ):
      # pfw.console.debug.trace( "exitRequest" )
      self.__function.set_type( pfw.code.Function.eType.REQUEST )
      self.__interface.add_request( self.__function )
      self.__function = None


   def enterResponse( self, ctx: IdlParser.ResponseContext ):
      # pfw.console.debug.trace( "enterResponse" )
      pass

   def exitResponse( self, ctx: IdlParser.ResponseContext ):
      # pfw.console.debug.trace( "exitResponse" )
      self.__function.set_type( pfw.code.Function.eType.RESPONSE )
      self.__interface.add_response( self.__function, self.__to_request_name )
      self.__function = None
      self.__to_request_name = None


   def enterConnection( self, ctx: IdlParser.ConnectionContext ):
      # pfw.console.debug.trace( "enterConnection" )
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
      request.set_type( pfw.code.Function.eType.REQUEST )
      response = None
      if None != self.__arguments:
         response = pfw.code.Function( None, request.name( ), pfw.code.Function.eType.RESPONSE )
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
      self.__interface.add_attribute(
            pfw.code.Parameter( ctx.IDENTIFIER( )[0].getText( ),
            ctx.IDENTIFIER( )[1].getText( ) )
         )

   def exitAttribute( self, ctx: IdlParser.AttributeContext ):
      # pfw.console.debug.trace( "exitAttribute" )
      pass


   def enterProcedure( self, ctx: IdlParser.ProcedureContext ):
      # pfw.console.debug.trace( "enterProcedure" )
      self.__function = pfw.code.Function(
            None,
            ctx.name( ).getText( )
         )

   def exitProcedure( self, ctx: IdlParser.ProcedureContext ):
      # pfw.console.debug.trace( "exitProcedure" )
      self.__function.set_arguments( self.__arguments )
      self.__arguments = None


   def enterFunction( self, ctx: IdlParser.FunctionContext ):
      # pfw.console.debug.trace( "enterFunction" )
      self.__function = pfw.code.Function(
            ctx.type_( ).getText( ),
            ctx.name( ).getText( )
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
      self.__arguments.append(
            pfw.code.Parameter(
                  ctx.type_( ).getText( ),
                  ctx.name( ).getText( )
               )
         )

   def exitArgument( self, ctx: IdlParser.ArgumentContext ):
      # pfw.console.debug.trace( "exitArgument" )
      pass



   def get_interface( self ):
      return self.__interface
   # def get_interface



   __interface: interface.Interface = None
   __function: pfw.code.Function = None
   __arguments: list = [ ]
   __to_request_name: str = None

# class IdlListener



class IdlErrorListener( ErrorListener ):
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

# class IdlErrorListener



def parse( source_file: str ):
   input = FileStream( source_file )


   listener = IdlListener( io.StringIO( ) )
   error_listener = IdlErrorListener( io.StringIO( ) )

   lexer = IdlLexer( input )
   stream = CommonTokenStream( lexer )
   parser = IdlParser( stream )
   parser.removeErrorListeners( )        
   parser.addErrorListener( error_listener )
   tree = parser.content( )

   walker = ParseTreeWalker( )
   walker.walk( listener, tree )

   return listener.get_interface( )
# def parse

def generate( source_file: str, gen_outdir: str, api_outdir: str, imp_outdir: str ):
   _interface = parse( source_file )

   _builder: builder.Builder = builder.Builder( _interface, gen_outdir, api_outdir, imp_outdir )
   _builder.build( )
# def generate
