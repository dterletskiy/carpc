#!/usr/bin/python

import os
import sys

import base
import archive
import console
import settings
import project
import time



def choose_projects( _projects: list ):
   console.debug.info( "Available projects:" )
   for index, project_name in enumerate( _projects ):
      console.debug.trace( "   %d: %s" %(index, project_name) )
   console.debug.trace( "   %d: all projects" %( len( _projects ) ) )

   _project_names: list = [ ]
   result_project: str = console.debug.promt( "Choose project [default {0}]: ".format( len( _projects ) ) )
   result: list = result_project.split( )
   if 0 == len( result ):
      _project_names = _projects
   else:  
      for item in result:
         number: int = base.string_to_int( item, 4 )
         if number == len( _projects ):
            _project_names = _projects
            break
         if number > len( _projects ):
            console.debug.error( "wrong choise: ", number )
            exit( 3 )
         _project_names.append( _projects[ number ] )
   # console.debug.info( "Projects: ", _project_names )
   return _project_names
# def choose_projects

def choose_action( _actions: list ) -> str:
   console.debug.info( "Available actions:" )
   for index, action in enumerate( _actions ):
      console.debug.trace( "   %d: %s" %(index, action) )

   result: str = console.debug.promt( "Choose action [default {0}]: ".format( len( _actions ) - 1 ) )
   number: int = len( _actions ) - 1
   if 0 != len( result ):
      number: int = base.string_to_int( result, 4 )
      if number >= len( _actions ):
         console.debug.error( "wrong choise: ", number )
         exit( 3 )
   # console.debug.info( "Action: ", _action_name )
   return _actions[ number ]
# def choose_action



os.system( "reset" )
COMPILER=os.environ.get( 'CXX', "g++" )
SESSION=os.environ.get( 'SESSION', "default" )

ROOT = "/mnt/p4v/Source/RPC"
# projects: list = [ "framework", "hooks", "servicebrocker", "application", "core", "controller", "hmi", "experimental" ]
projects: list = [ "framework", "hooks", "servicebrocker", "application", "core", "controller", "hmi" ]
# projects: list = [ "framework", "servicebrocker", "application", "core", "controller", "hmi" ]
# projects: list = [ "framework", "experimental" ]
actions: dict = {
   'archive':        [ project.clean_all, archive.archive                        ],
   'clean':          [ project.clean_all                                         ],
   'info':           [ project.info_all                                          ],
   'preprocessing':  [ project.preprocessing_all                                 ],
   'compile':        [ project.compile_all                                       ],
   'link':           [ project.link_all                                          ],
   'build':          [ project.compile_all, project.link_all                     ],
   'clean build':    [ project.clean_all, project.compile_all, project.link_all  ]
}
PROJECTS: dict = { }



project_names: list = choose_projects( projects )
action_name: str = choose_action( list( actions.keys( ) ) )

start_action: int = int( round(time.time() * 1000) )
project.init( project_names, PROJECTS, ROOT, SESSION, COMPILER )
for processor in actions.get( action_name ):
   if archive.archive == processor:
      archive.archive( ROOT )
   else:
      processor( PROJECTS )
finish_action: int = int( round(time.time() * 1000) )
console.debug.question( "Total action time: %d ms" %(finish_action - start_action) )

