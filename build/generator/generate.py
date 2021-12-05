#!/usr/bin/python3

from enum import Enum
import re
import xml.etree.ElementTree as ET
import pfw.base
import pfw.console



project_path: str = "/home/dmytro_terletskyi/Sources/TDA/python/generate/"

api_path: str = project_path + "api/"
configuration_file_name: str = api_path + "System.xml"

gen_path: str = project_path + "gen/"
hpp_file_name: str = gen_path + "System.hpp"
cpp_file_name: str = gen_path + "System.cpp"

template_path: str = project_path + "templates/"





class Template:
   def __init__( self, hpp: str, cpp: str ):
      self.__hpp = hpp
      self.__cpp = cpp
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Template.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in Template.__dict__.keys( ) if i[:2] != pfw.base.class_ignore_field ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      name = "Element { " + ", ".join( vector ) + " }"
      return name
   # def __str__

   def hpp( self ):
      return self.__hpp
   # def hpp

   def cpp( self ):
      return self.__cpp
   # def hpp

   __hpp: str = None
   __cpp: str = None
# class Template
g_templates: dict = {
   'class': Template( template_path + "TemplateClass.hpp", template_path + "TemplateClass.cpp" ),
   'enum': Template( template_path + "TemplateEnum.hpp", template_path + "TemplateEnum.cpp" ),
   'event': Template( template_path + "TemplateEvent.hpp", template_path + "TemplateEvent.cpp" )
}



class Pattern:
   def __init__( self ):
      self.reset( )
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Pattern.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in Pattern.__dict__.keys( ) if i[:2] != pfw.base.class_ignore_field ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      name = "Pattern { " + ", ".join( vector ) + " }"
      return name
   # def __str__

   def info( self, tabulations: int = 0 ):
      pfw.console.debug.info( tabulations * "\t", "prefix:      \'", self.__prefix, "\'" )
      pfw.console.debug.info( tabulations * "\t", "type:        \'", self.__type, "\'" )
      pfw.console.debug.info( tabulations * "\t", "pattern:     \'", self.__pattern, "\'" )
      pfw.console.debug.info( tabulations * "\t", "separator:   \'", self.__separator, "\'" )
      pfw.console.debug.info( tabulations * "\t", "suffix:      \'", self.__suffix, "\'" )
   # def print

   def reset( self ):
      self.__prefix: str = None
      self.__type: str = None
      self.__pattern: str = None
      self.__separator: str = None
      self.__suffix: str = None
   # def reset


   def parse( self, line: str ):
      match = re.match( self.__reg_exp, line )
      if match:
         self.__prefix: str = match.group( 1 )
         self.__type: str = match.group( 2 )
         self.__pattern: str = match.group( 3 )
         self.__separator: str = match.group( 4 ).replace( "NL", "\n" )
         self.__suffix: str = match.group( 5 )
         return True
      else:
         self.reset( )
         return False
   # def parse

   def prefix( self ):
      return self.__prefix

   def type( self ):
      return self.__type

   def pattern( self ):
      return self.__pattern

   def separator( self ):
      return self.__separator

   def suffix( self ):
      return self.__suffix

   __reg_exp: str = r'^(.*)ITER_BEGIN[{](\S+)[|](.+)[|](.*)[}]ITER_END(.*)$'
   __prefix: str = None
   __type: str = None
   __pattern: str = None
   __separator: str = None
   __suffix: str = None
# class Pattern
g_parser: Pattern = Pattern( )



class IElement:
   def element_type( ):
      pass

class Element:
   def __init__( self, type: str, name: str, value: str = None, descripion: str = None ):
      self.__type = type
      self.__name = name
      self.__value = value if None != value else "{ }"
      self.__description = descripion if None != descripion else self.__name
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Element.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in Element.__dict__.keys( ) if i[:2] != pfw.base.class_ignore_field ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      name = "Element { " + ", ".join( vector ) + " }"
      return name
   # def __str__

   def info( self, tabulations: int = 0 ):
      pfw.console.debug.info( tabulations * "\t", self.__type, " ", self.__name, " = ", self.__value )
   # def info

   def generate( self ):
      pfw.console.debug.info( "generate variable" )
      self.__generate( template_variable_hpp_file_name, generated_hpp )
      self.__generate( template_variable_cpp_file_name, generated_cpp )
   # def generate

   def __generate( self, template_file_name, output_file ):
      template_file = open( template_file_name, "r" )

      lines = template_file.readlines( )
      for line in lines:
         pfw.console.debug.trace( "processing:\n", line )

         line = line.replace( 'NAMESPACE_NAME', self.__namespace )

         pfw.console.debug.info( "processed:\n", line )

         output_file.write( line )

      template_file.close( )
   # def __generate

   def type( self ):
      return self.__type
   # def type

   def name( self ):
      return self.__name
   # def type

   def value( self ):
      return self.__value
   # def type

   __namespace: str = None
   __type: str = None
   __name: str = None
   __value: str = None
   __description: str = None
# class Element


class ElementEnum:
   def __init__( self, namespace: str, name: str, type: str ):
      self.__namespace = namespace
      self.__type = type
      self.__name = name
      self.__fields = [ ]
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in ElementEnum.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in ElementEnum.__dict__.keys( ) if i[:2] != pfw.base.class_ignore_field ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      name = "ElementEnum { " + ", ".join( vector ) + " }"
      return name
   # def __str__

   def add_field( self, name: str, value: str = None, descripion: str = None ):
      self.__fields.append( Element( self.__type, name, value, descripion ) )
   #def add_field

   def info( self, tabulations: int = 0 ):
      pfw.console.debug.info( tabulations * "\t", "type: ", self.__type )
      pfw.console.debug.info( tabulations * "\t", "name: ", self.__name )
      pfw.console.debug.info( tabulations * "\t", "fields: " )
      for field in self.__fields:
         field.info( tabulations + 1 )
   # def info

   def generate( self ):
      pfw.console.debug.info( "generate enum" )
      self.__generate( g_templates[ 'enum' ].hpp( ), generated_hpp )
      self.__generate( g_templates[ 'enum' ].cpp( ), generated_cpp )
   # def generate

   def __generate( self, template_file_name, output_file ):
      template_file = open( template_file_name, "r" )

      lines = template_file.readlines( )
      for line in lines:
         pfw.console.debug.trace( "processing:\n", line )

         line = line.replace( 'NAMESPACE_NAME', self.__namespace )
         line = line.replace( 'ENUM_NAME', self.__name )
         line = line.replace( 'ENUM_TYPE', self.__type )

         if True == g_parser.parse( line ):
            line = g_parser.prefix( )
            if 'FIELD' == g_parser.type( ):
               field_count = -1
               for field in self.__fields:
                  field_count += 1
                  line += g_parser.pattern( )
                  line = line.replace( 'FIELD_TYPE', field.type( ) )
                  line = line.replace( 'FIELD_NAME', field.name( ) )
                  line = line.replace( 'FIELD_VALUE', field.value( ) )
                  if field_count != len( self.__fields ) - 1:
                     line += g_parser.separator( )
            line += g_parser.suffix( ) + "\n"

         pfw.console.debug.info( "processed:\n", line )

         output_file.write( line )

      template_file.close( )
   # def __generate

   __namespace: str = None
   __type: str = None
   __name: str = None
   __fields: list = [ ]
# class ElementEnum


class ElementClass:
   def __init__( self, namespace: str, name: str ):
      self.__namespace = namespace
      self.__name = name
      self.__fields = [ ]
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in ElementClass.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def add_field( self, type: str, name: str, value: str = None, descripion: str = None ):
      self.__fields.append( Element( type, name, value, descripion ) )
   # def add_field

   def info( self, tabulations: int = 0 ):
      pfw.console.debug.info( tabulations * "\t", "name: ", self.__name )
      pfw.console.debug.info( tabulations * "\t", "fields: " )
      for field in self.__fields:
         field.info( tabulations + 1 )
   # def info

   def generate( self ):
      pfw.console.debug.info( "generate class" )
      self.__generate( g_templates[ 'class' ].hpp( ), generated_hpp )
      self.__generate( g_templates[ 'class' ].cpp( ), generated_cpp )
   # def generate

   def __generate( self, template_file_name, output_file ):
      template_file = open( template_file_name, "r" )

      lines = template_file.readlines( )
      for line in lines:
         pfw.console.debug.trace( "processing:\n", line )

         line = line.replace( 'NAMESPACE_NAME', self.__namespace )
         line = line.replace( 'CLASS_NAME', self.__name )

         if True == g_parser.parse( line ):
            line = g_parser.prefix( )
            if 'FIELD' == g_parser.type( ):
               field_count = -1
               for field in self.__fields:
                  field_count += 1
                  line += g_parser.pattern( )
                  line = line.replace( 'FIELD_TYPE', field.type( ) )
                  line = line.replace( 'FIELD_NAME', field.name( ) )
                  line = line.replace( 'FIELD_VALUE', field.value( ) )
                  if field_count != len( self.__fields ) - 1:
                     line += g_parser.separator( )
            line += g_parser.suffix( ) + "\n"

         pfw.console.debug.info( "processed:\n", line )

         output_file.write( line )

      template_file.close( )
   # def __generate

   __namespace: str = None
   __name: str = None
   __fields: list = [ ]
# class ElementClass


class ElementEvent:
   def __init__( self, namespace: str, type: str, name: str, data: str, signature: str ):
      self.__namespace = namespace
      self.__type = type
      self.__name = name
      self.__data = data
      self.__signature = signature
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in ElementEvent.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def info( self, tabulations: int = 0 ):
      pfw.console.debug.info( tabulations * "\t", "type: ", self.__type )
      pfw.console.debug.info( tabulations * "\t", "name: ", self.__name )
      pfw.console.debug.info( tabulations * "\t", "data: ", self.__data )
      pfw.console.debug.info( tabulations * "\t", "signature: ", self.__signature )
   # def info

   def generate( self ):
      pfw.console.debug.info( "generate event" )
      self.__generate( g_templates[ 'event' ].hpp( ), generated_hpp )
      self.__generate( g_templates[ 'event' ].cpp( ), generated_cpp )
   # def generate

   def __generate( self, template_file_name, output_file ):
      template_file = open( template_file_name, "r" )

      lines = template_file.readlines( )
      for line in lines:
         pfw.console.debug.trace( "processing:\n", line )

         line = line.replace( 'NAMESPACE_NAME', self.__namespace )
         line = line.replace( 'EVENT_NAME', self.__name )
         line = line.replace( 'DATA_NAME', self.__data )
         line = line.replace( 'SIGNATURE_NAME', self.__signature )

         pfw.console.debug.info( "processed:\n", line )

         output_file.write( line )

      template_file.close( )
   # def __generate

   __namespace: str = None
   __type: str = None
   __name: str = None
   __data: str = None
   __signature: str = None
# class ElementEvent


class ElementResponse:
   def __init__( self, namespace: str, name: str ):
      self.__namespace = namespace
      self.__name = name
      self.__parameters = [ ]
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in ElementResponse.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def add_parameter( self, type: str, name: str, value: str = None, descripion: str = None ):
      self.__parameters.append( Element( type, name, value, descripion ) )
   # def add_parameter

   def info( self, tabulations: int = 0 ):
      pfw.console.debug.info( tabulations * "\t", "name: ", self.__name )
      pfw.console.debug.info( tabulations * "\t", "parameters: " )
      for parameter in self.__parameters:
         parameter.info( tabulations + 1 )
   # def info

   def generate( self ):
      pfw.console.debug.info( "generate response" )
      self.__generate( g_templates[ 'class' ].hpp( ), generated_hpp )
      self.__generate( g_templates[ 'class' ].cpp( ), generated_cpp )
   # def generate

   def __generate( self, template_file_name, output_file ):
      template_file = open( template_file_name, "r" )

      lines = template_file.readlines( )
      for line in lines:
         pfw.console.debug.trace( "processing:\n", line )



         pfw.console.debug.info( "processed:\n", line )

         output_file.write( line )

      template_file.close( )
   # def __generate

   __namespace: str = None
   __name: str = None
   __parameters: list = [ ]
# class ElementClass


class ElementRequest:
   def __init__( self, namespace: str, name: str ):
      self.__namespace = namespace
      self.__name = name
      self.__parameters = [ ]
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in ElementRequest.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def add_parameter( self, type: str, name: str, value: str = None, descripion: str = None ):
      self.__parameters.append( Element( type, name, value, descripion ) )
   # def add_parameter

   def set_response( self, response: ElementResponse ):
      self.__response = response
   # def set_response

   def info( self, tabulations: int = 0 ):
      pfw.console.debug.info( tabulations * "\t", "name: ", self.__name )
      pfw.console.debug.info( tabulations * "\t", "parameters: " )
      for parameter in self.__parameters:
         parameter.info( tabulations + 1 )
   # def info

   def generate( self ):
      pfw.console.debug.info( "generate request" )
      self.__generate( g_templates[ 'class' ].hpp( ), generated_hpp )
      self.__generate( g_templates[ 'class' ].cpp( ), generated_cpp )
   # def generate

   def __generate( self, template_file_name, output_file ):
      template_file = open( template_file_name, "r" )

      lines = template_file.readlines( )
      for line in lines:
         pfw.console.debug.trace( "processing:\n", line )



         pfw.console.debug.info( "processed:\n", line )

         output_file.write( line )

      template_file.close( )
   # def __generate

   __namespace: str = None
   __name: str = None
   __parameters: list = [ ]
   __response: ElementResponse = [ ]
# class ElementClass



class Elements:
   def __init__( self, namespace: str ):
      self.__namespace = namespace
      self.__elements = [ ]
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def add_element( self, element: IElement ):
      self.__elements.append( element )
   # def add_element

   def generate( self ):
      for element in self.__elements:
         element.generate( )
         # if isinstance( element, ElementEnum ):
         #    pfw.console.debug.info( "enum" )
   # def generate

   def info( self ):
      tabulations: int = 0
      pfw.console.debug.header( tabulations * "\t", "namespace: ", self.__namespace )

      tabulations += 1
      for element in self.__elements:
         element.info( tabulations )
   # def info

   def namespace( self ):
      return self.__namespace
   # def namespace

   __namespace: str = None
   __elements: list = [ ]
# class Elements



class ElementsBuilder:
   def __init__( self, xml_file: str ):
      tree = ET.parse( xml_file )
      root = tree.getroot( )
      self.xml_print_element( root )
      self.__elements = Elements( root.attrib.get( 'name' ) )

      for root_child in root:
         self.xml_process_element( root_child )

      self.__elements.info( )
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def xml_print_element( self, element, is_print: bool = True ):
      if False == is_print: return
      pfw.console.debug.trace( element, element.tag, element.attrib )
   # def xml_print_element

   def xml_process_element( self, element ):
      if "class" == element.tag:
         self.xml_process_element_class( element )
      elif "enum" == element.tag:
         self.xml_process_element_enum( element )
      elif "event" == element.tag:
         self.xml_process_element_event( element )
      else:
         pfw.console.debug.info( "processing undefined:", element.attrib )
   # def xml_process_element

   def xml_process_element_class( self, element ):
      pfw.console.debug.info( "processing class:", element.attrib )

      element_class: ElementClass = ElementClass(
         self.__elements.namespace( ),
         element.attrib.get( 'name' )
      )

      for element_filed in element.findall( 'field' ):
         element_class.add_field(
            element_filed.attrib.get( 'type' ),
            element_filed.attrib.get( 'name' ),
            element_filed.attrib.get( 'value' )
         )

      self.__elements.add_element( element_class )
   # def xml_process_element_class

   def xml_process_element_enum( self, element ):
      pfw.console.debug.info( "processing enum:", element.attrib )

      element_enum: ElementEnum = ElementEnum(
         self.__elements.namespace( ),
         element.attrib.get( 'name' ),
         element.attrib.get( 'type' )
      )

      for element_filed in element.findall( 'field' ):
         element_enum.add_field(
            element_filed.attrib.get( 'name' ),
            element_filed.attrib.get( 'value' ),
            element_filed.attrib.get( 'description' )
         )

      self.__elements.add_element( element_enum )
   # def xml_process_element_enum

   def xml_process_element_event( self, element ):
      pfw.console.debug.info( "processing event:", element.attrib )

      element_event: ElementEvent = ElementEvent(
         self.__elements.namespace( ),
         element.attrib.get( 'type' ),
         element.attrib.get( 'name' ),
         element.attrib.get( 'data' ),
         element.attrib.get( 'signature' )
      )

      self.__elements.add_element( element_event )
   # def xml_process_element_event

   def generate( self ):
      self.__elements.generate( )
   # def generate

   __elements = None
# class ElementsBuilder











builder: ElementsBuilder = ElementsBuilder( configuration_file_name )
generated_hpp = open( hpp_file_name, "w+" )
generated_cpp = open( cpp_file_name, "w+" )
builder.generate( )
generated_hpp.close( )
generated_cpp.close( )
