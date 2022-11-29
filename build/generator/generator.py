#!/usr/bin/python3

import os
import sys
import argparse



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

g_app_data = cmdline_argparse( sys.argv[1:] )



def configure( app_data ):
   include_count: int = 0
   for path in app_data.include_dirs:
      include_count += 1
      sys.path.insert( include_count, path )
# def configure

configure( g_app_data )

filename, file_extension = os.path.splitext( g_app_data.source_file )



import pfw.base
import pfw.console
import pfw.shell



def antlr_gen( app_data ):
   if None == app_data.antlr_jar:
      return

   os.makedirs( app_data.antlr_outdir, exist_ok = True )

   for g4_file in app_data.antlr_lexer:
      pfw.shell.execute(
            "java", "-jar", app_data.antlr_jar, "-Dlanguage=Python3",
            g4_file,
            "-o", app_data.antlr_outdir,
            "-listener", "-visitor"
         )

   for g4_file in app_data.antlr_parser:
      pfw.shell.execute(
            "java", "-jar", app_data.antlr_jar, "-Dlanguage=Python3",
            g4_file,
            "-o", app_data.antlr_outdir,
            "-listener", "-visitor"
         )
# def antlr_gen

antlr_gen( g_app_data )



if ".idl" == file_extension:
   pfw.console.debug.header( "IDL generation" )
   import idl.parser
elif ".ddl" == file_extension:
   pfw.console.debug.header( "DDL generation" )
   import ddl.parser
elif ".adl" == file_extension:
   pfw.console.debug.header( "ADL generation" )
   import adl.parser



def main( ):
   if ".idl" == file_extension:
      idl.parser.generate( g_app_data.source_file, g_app_data.gen_outdir, g_app_data.api_outdir, g_app_data.imp_outdir )
   elif ".ddl" == file_extension:
      ddl.parser.generate( g_app_data.source_file, g_app_data.gen_outdir, g_app_data.api_outdir, g_app_data.imp_outdir )
   elif ".adl" == file_extension:
      adl.parser.generate( g_app_data.source_file, g_app_data.gen_outdir, g_app_data.api_outdir, g_app_data.imp_outdir )
# def main



if __name__ == '__main__':
   main( )
