#!/usr/bin/python

import xml.etree.ElementTree as ET
import base

def xml_print_element( element, is_print = 0 ):
   if 0 == is_print: return
   print( element, element.tag, element.attrib )
# def xml_print_element



########################################
# 
# class Condition
# 
########################################
class Condition:
   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Condition.__dict__.keys( ) ]
      for attr_name in attr_list:
         if attr == attr_name:
            self.__dict__[ attr ] = value
            return
      raise AttributeError

   def __str__( self ):
      attr_list = [ i for i in Condition.__dict__.keys( ) if i[:1] != '_' ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      name = "Condition { " + ", ".join( vector ) + " }"
      return name

   type: str = None
   name: str = None
   status: str = None
# class Condition

########################################
# 
# class Setting
# 
########################################
class Setting:
   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Setting.__dict__.keys( ) ]
      for attr_name in attr_list:
         if attr == attr_name:
            self.__dict__[ attr ] = value
            return
      raise AttributeError

   def __str__( self ):
      attr_list = [ i for i in Setting.__dict__.keys( ) if i[:1] != '_' ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      name = "Setting { " + ", ".join( vector ) + " }"
      return name

   type: str = None
   name: str = None
   value: str = None
   status: str = None
   conditions: Condition = [ ]
# class Setting



def process_configuration( element, project_configuration ):
   xml_print_element( element )

   for child in element.findall( 'config' ):
      process_config( child, project_configuration )
# def process_configuration

def process_config( element, project_configuration ):
   xml_print_element( element )

   if None == element.attrib.get( 'name' ):
      print( "Error: config name not defined" )
      return None

   project_configuration[ element.attrib.get( 'name' ) ] = element.attrib.get( 'status' )
# def process_config

def process_settings( element, project_settings ):
   xml_print_element( element )

   settings_type = element.attrib.get( 'type' )
   if None == settings_type:
      print( "Error: setting type not defined" )
      return None

   if settings_type not in project_settings:
      project_settings[ settings_type ] = [ ]

   for child in element.findall( 'setting' ):
      project_settings[ settings_type ].append( process_setting( child ) )
# def process_settings

def process_setting( element ):
   xml_print_element( element )

   setting: Setting = Setting( )
   setting.type = element.attrib.get( 'type' )
   setting.name = element.attrib.get( 'name' )
   setting.value = element.attrib.get( 'value' )
   setting.status = element.attrib.get( 'status' )

   vector: Condition = [ ]
   if "conditions" == setting.status:
      for child in element.findall( 'condition' ):
         vector.append( process_condition( child ) )
   setting.conditions = vector

   return setting
# def process_setting

def process_condition( element ):
   xml_print_element( element )

   condition: Condition = Condition( )
   condition.type = element.attrib.get( 'type' )
   condition.name = element.attrib.get( 'name' )
   condition.status = element.attrib.get( 'status' )

   return condition
# def process_condition

def process_config_file( config_file, project_info, project_configuration, project_settings ):
   tree = ET.parse( config_file )
   root = tree.getroot( )
   xml_print_element( root )
   config_type = root.attrib.get( 'type' )
   print( "Processing $config_type configuration file: ", config_file )
   if "local" == config_type:
      project_info['name'] = root.attrib.get( "name" )

   for child in root.findall( 'configuration' ):
      process_configuration( child, project_configuration )
   # print( "---------------------------- configuration ---------------------------" )
   # print( base.to_string( project_configuration ) )
   # print( "----------------------------------------------------------------------" )

   for child in root.findall( 'settings' ):
      process_settings( child, project_settings )
   # print( "------------------------------ settings ------------------------------" )
   # print( base.to_string( project_settings ) )
   # print( "----------------------------------------------------------------------" )
# def process_config_file

def apply_configuration( project_configuration, project_settings ):
   for settings in project_settings:
      for setting in project_settings[ settings ]:
         if "conditions" == setting.status:
            status: bool = True
            for condition in setting.conditions:
               status = status and ( condition.status == project_configuration[ condition.name ] )
            if True == status: setting.status = "active"
            else: setting.status = "inactive"
            setting.conditions = [ ]
# def apply_configuration




def init( config_file_list ):
   project_info: dict = { 'name': None }
   project_configuration: dict = { }
   project_settings: dict = { 'compilation': [ ], 'link': [ ] }

   for config_file in config_file_list:
      process_config_file( config_file, project_info, project_configuration, project_settings )

   apply_configuration( project_configuration, project_settings )
   print( "---------------------------- configuration ---------------------------" )
   print( base.to_string( project_configuration ) )
   print( "----------------------------------------------------------------------" )
   print( "------------------------------ settings ------------------------------" )
   print( base.to_string( project_settings ) )
   print( "----------------------------------------------------------------------" )


   compile_params: str = ""
   for item in project_settings['compilation']:
      if "active" != item.status: continue

      compile_params += " "
      if "include" == item.type:
         compile_params += "-I" + item.value
      elif "define" == item.type:
         compile_params += "-D" + item.name
         if None != item.value:
            compile_params += "=" + item.value
      elif "flag" == item.type:
         compile_params += item.value
      else: print( "Undefined parameter type", item.type )
   # print( compile_params )

   link_params: str = ""
   for item in project_settings['link']:
      if "active" != item.status: continue

      link_params += " "
      if "include" == item.type:
         link_params += "-L" + item.value
      elif "flag" == item.type:
         link_params += item.value
      else: print( "Undefined parameter type", item.type )
   # print( link_params )

   result: dict = { }
   result['type'] = project_configuration['type']
   
   if "shared" == result['type']:
      result['product'] = "lib" + project_info['name'] + ".so"
   elif "static" == result['type']:
      result['product'] = "lib" + project_info['name'] + ".a"
   else:
      result['product'] = project_info['name']
   result['cflags'] = compile_params
   result['ldflags'] = link_params

   return result
# def init
