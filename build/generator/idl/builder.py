# https://github.com/python/mypy/issues/3661
from __future__ import annotations
from enum import Enum
import os
import sys

import pfw.base.struct
import pfw.console

import idl.interface



DEBUG: bool = False



class Builder:
   def __init__( self, interface_: interface.Interface, gen_dir: str, api_dir: str, imp_dir: str ):
      self.__interface = interface_
      self.__gen_dir = gen_dir
      self.__api_dir = api_dir
      self.__imp_dir = imp_dir
      # self.__api_dir = os.path.join( api_dir, self.__interface.name( ), self.__interface.version( ).string( "." ) )
      # self.__imp_dir = os.path.join( imp_dir, self.__interface.name( ), self.__interface.version( ).string( "." ) )
   # def __init__

   def __del__( self ):
      pass
   # def __del__

   def __setattr__( self, attr, value ):
      attr_list = [ i for i in Builder.__dict__.keys( ) ]
      if attr in attr_list:
         self.__dict__[ attr ] = value
         return
      raise AttributeError
   # def __setattr__

   def __str__( self ):
      attr_list = [ i for i in Builder.__dict__.keys( ) if i[:2] != pfw.base.struct.ignore_field
 ]
      vector = [ ]
      for attr in attr_list:
         vector.append( str( attr ) + " = " + str( self.__dict__.get( attr ) ) )
      name = "Builder { " + ", ".join( vector ) + " }"
      return name
   # def __str__

   def info( self, **kwargs ):
      tabulations: int = kwargs.get( "tabulations", 0 )
      pfw.console.debug.info( self.__class__.__name__, ":", tabs = ( tabulations + 0 ) )
      pfw.console.debug.info( "interface:    \'", self.__interface, "\'", tabs = ( tabulations + 1 ) )
      pfw.console.debug.info( "gen_dir:      \'", self.__gen_dir, "\'", tabs = ( tabulations + 1 ) )
      pfw.console.debug.info( "api_dir:      \'", self.__api_dir, "\'", tabs = ( tabulations + 1 ) )
      pfw.console.debug.info( "imp_dir:      \'", self.__imp_dir, "\'", tabs = ( tabulations + 1 ) )
   # def info

   def generate( self, code: str, file_name: str, subdirectory: str ):
      file = None
      if False == DEBUG:
         file_directory = os.path.join( self.__gen_dir, subdirectory ) if None != subdirectory else self.__gen_dir
         os.makedirs( file_directory, exist_ok = True )
         file = open( os.path.join( file_directory, file_name ), "w" )
         original_stdout = sys.stdout
         sys.stdout = file

      print( code )

      if None != file:
         sys.stdout = original_stdout
         file.close( )
   # def generate

   def build( self, **kwargs ):
      self.build_version_hpp( )
      self.build_data_hpp( )
      self.build_data_cpp( )
      self.build_iserver_hpp( )
      self.build_iserver_cpp( )
      self.build_iclient_hpp( )
      self.build_iclient_cpp( )
      self.build_server_impl_hpp( )
      self.build_server_impl_cpp( )
      self.build_client_impl_hpp( )
      self.build_client_impl_cpp( )
      self.build_server_hpp( )
      self.build_server_cpp( )
      self.build_client_hpp( )
      self.build_client_cpp( )
   # def build

   def build_version_hpp( self ):
      code: str = "";

      code += "#pragma once\n";
      code += "\n"
      code += "#include \"carpc/comm/service/Types.hpp\"\n";
      for include in self.__interface.includes( ):
         code += "#include \"" + include + "\"\n";

      code += "\n"

      code += "namespace " + self.__interface.namespace( ) + " {\n"
      code += "\n"
      code += "   const carpc::comm::service::Version version{ " + self.__interface.version( ).string( ", " ) + " };\n"
      code += "\n"

      code += "} // namespace " + self.__interface.namespace( ) + "\n"
      code += "\n"

      self.generate( code, "Version.hpp", self.__api_dir )
      return code;
   # def build_version_hpp

   def build_data_hpp( self ):
      code: str = "";

      code += "#pragma once\n";
      code += "\n"
      code += "#include <memory>\n";
      code += "#include \""
      if None != self.__api_dir: code += self.__api_dir + "/"
      code += "Version.hpp\"\n";
      code += "\n"


      code += "namespace " + self.__interface.namespace( ) + " {\n"
      code += "\n"


      def build_base_data( ):
         code: str = "";
         code += "      class BaseData\n"
         code += "      {\n"
         code += "         public:\n"
         code += "            using tSptr = std::shared_ptr< BaseData >;\n"
         code += "\n"
         code += "            BaseData( ) = default;\n"
         code += "            virtual ~BaseData( ) = default;\n"
         # Only ipc methods
         if True == self.__interface.is_ipc( ):
            code += "\n"
            code += "         public:\n"
            code += "            static tSptr create( carpc::ipc::tStream& );\n"
            code += "            bool serrialize( carpc::ipc::tStream& );\n"
            code += "\n"
            code += "         private:\n"
            code += "            virtual bool to_stream( carpc::ipc::tStream& ) = 0;\n"
            code += "            virtual bool from_stream( carpc::ipc::tStream& ) = 0;\n"
            code += "            virtual eID id( ) const = 0;\n"
            code += "            virtual carpc::service::eType type( ) const = 0;\n"
         code += "      };\n"
         code += "\n"

         return code
      # def build_base_data


      # namespace method {
      code += "   namespace method {\n"
      code += "\n"


      code += "      DEFINE_ENUMERATION( eID, std::uint8_t\n"
      for connection in self.__interface.connections( ):
         code += "         , " + connection.name( ) + "\n"
      code += "         , Undefined\n"
      code += "      );\n"
      code += "\n"

      code += "      inline bool has_response( const eID& request_id )\n"
      code += "      {\n"
      code += "         return false\n"
      for connection in self.__interface.connections( ):
         if None != connection.response( ):
            code += "            || eID::" + connection.name( ) + " == request_id\n"
      code += "         ;\n"
      code += "      }\n"
      code += "\n"

      code += build_base_data( )

      def build_function_class( function: interface.Function, event: str ):
         code: str = "";
         if None == function:
            return code

         function_name = function.name( )
         function_arguments = function.arguments( )
         function_type = str( function.type( ) )

         code += "      namespace " + function_type.lower( ) + " {\n"
         code += "\n"
         code += "         class " + function_name + "Data : public BaseData\n"
         code += "         {\n"
         code += "            public:\n"
         code += "               static constexpr eID ID = eID::" + event + ";\n"
         code += "\n"
         code += "            public:\n"
         # Default constructor
         code += "               " + function_name + "Data( ) = default;\n"
         # Constructor with parameter
         if 0 < len( function_arguments ):
            code += "               " + function_name + "Data( "
            for count in range( len( function_arguments ) ):
               argument = function_arguments[count]
               code += "const " + argument.type( ) + "& _" + argument.name( )
               code += ", " if len( function_arguments ) > ( count + 1 ) else " "

            code += ");\n"
         # Destructor
         code += "               ~" + function_name + "Data( ) override = default;\n"
         code += "\n"
         # Fields
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "               " + argument.type( ) + " " + argument.name( )
            if None != argument.value( ):
               code += " = " + argument.value( )
            code += ";\n"
         # Only ipc methods
         if True == self.__interface.is_ipc( ):
            code += "\n"
            code += "            private:\n"
            code += "               bool to_stream( carpc::ipc::tStream& ) override;\n"
            code += "               bool from_stream( carpc::ipc::tStream& ) override;\n"
            code += "               eID id( ) const override { return ID; }\n"
            code += "               carpc::service::eType type( ) const override { return carpc::service::eType::" + function_type.upper( ) + "; }\n"
            code += "\n"
         code += "         };\n"
         code += "\n"
         code += "      } // namespace " + function_type.lower( ) + "\n"
         code += "\n"

         return code
      # def build_function_class

      for connection in self.__interface.connections( ):
         code += build_function_class( connection.request( ), connection.name( ) )
         code += build_function_class( connection.response( ), connection.name( ) )


      # } // namespace method
      code += "   } // namespace method\n"
      code += "\n"



      # namespace attribute {
      code += "   namespace attribute {\n"
      code += "\n"


      code += "      DEFINE_ENUMERATION( eID, std::uint8_t\n"
      for attribute in self.__interface.attributes( ):
         code += "         , " + attribute.name( ) + "\n"
      code += "         , Undefined\n"
      code += "      );\n"
      code += "\n"

      code += build_base_data( )

      def build_attribute_class( attribute: interface.Parameter ):
         code: str = "";
         if None == attribute:
            return code

         attribute_type = attribute.type( )
         attribute_name = attribute.name( )
         attribute_value = attribute.value( )
         event_type = "notification"

         code += "      namespace " + event_type.lower( ) + " {\n"
         code += "\n"
         code += "         class " + attribute_name + "Data : public BaseData\n"
         code += "         {\n"
         code += "            public:\n"
         code += "               static constexpr eID ID = eID::" + attribute_name + ";\n"
         code += "\n"
         code += "            public:\n"
         # Default constructor
         code += "               " + attribute_name + "Data( ) = default;\n"
         # Constructor with parameter
         code += "               " + attribute_name + "Data( const " + attribute_type + "& _value );\n"
         # Destructor
         code += "               ~" + attribute_name + "Data( ) override = default;\n"
         code += "\n"
         # Fields
         code += "               " + attribute_type + " value"
         if None != attribute_value:
            code += " = " + attribute_value
         code += ";\n"
         # Only ipc methods
         if True == self.__interface.is_ipc( ):
            code += "\n"
            code += "            private:\n"
            code += "               bool to_stream( carpc::ipc::tStream& ) override;\n"
            code += "               bool from_stream( carpc::ipc::tStream& ) override;\n"
            code += "               eID id( ) const override { return ID; }\n"
            code += "               carpc::service::eType type( ) const override { return carpc::service::eType::" + event_type.upper( ) + "; }\n"
            code += "\n"
         code += "         };\n"
         code += "\n"
         code += "      } // namespace " + event_type.lower( ) + "\n"
         code += "\n"

         return code
      # def build_attribute_class

      for attribute in self.__interface.attributes( ):
         code += build_attribute_class( attribute )


      # } // namespace attribute
      code += "   } // namespace attribute\n"
      code += "\n"


      version_method = "V" + self.__interface.version( ).string( "_" ) + "::method"
      version_attribute = "V" + self.__interface.version( ).string( "_" ) + "::attribute"

      code += "   struct Types\n"
      code += "   {\n"
      code += "      using tIPC           = carpc::"
      code += "IPC" if True == self.__interface.is_ipc( ) else "NO_IPC"
      code += ";\n"
      code += "\n"
      code += "      struct method\n"
      code += "      {\n"
      code += "         using tID         = " + version_method + "::eID;\n"
      code += "         using tBaseData   = " + version_method + "::BaseData;\n"
      code += "\n"
      code += "         static bool has_response( const " + version_method + "::eID& request_id )\n"
      code += "         {\n"
      code += "            return " + version_method + "::has_response( request_id );\n"
      code += "         }\n"
      code += "      };\n"
      code += "\n"
      code += "      struct attribute\n"
      code += "      {\n"
      code += "         using tID         = " + version_attribute + "::eID;\n"
      code += "         using tBaseData   = " + version_attribute + "::BaseData;\n"
      code += "      };\n"
      code += "   };\n"
      code += "\n"


      code += "} // namespace " + self.__interface.namespace( ) + "\n"
      code += "\n"

      self.generate( code, "Data.hpp", self.__imp_dir )
      return code;
   # def build_data_hpp

   def build_data_cpp( self ):
      code: str = "";

      code += "#include \"Data.hpp\"\n";
      code += "\n"


      code += "namespace " + self.__interface.namespace( ) + " {\n"
      code += "\n"


      # namespace method {
      code += "   namespace method {\n"
      code += "\n"

      # Only ipc methods
      if True == self.__interface.is_ipc( ):
         code += "      BaseData::tSptr BaseData::create( carpc::ipc::tStream& stream )\n"
         code += "      {\n"
         code += "         eID id = eID::UNDEFINED;\n"
         code += "         carpc::service::eType type = carpc::service::eType::UNDEFINED;\n"
         code += "         if( false == carpc::ipc::deserialize( stream, id, type ) )\n"
         code += "            return nullptr;\n"
         code += "\n"
         code += "         tSptr ptr = nullptr;\n"
         code += "         switch( id )\n"
         code += "         {\n"
         for connection in self.__interface.connections( ):
            if None == connection.request( ):
               continue
            code += "            case eID::" + connection.name( ) + ":\n"
            code += "            {\n"
            code += "               switch( type )\n"
            code += "               {\n"
            code += "                  case carpc::service::eType::REQUEST:\n"
            code += "                  {\n"
            code += "                     ptr = std::make_shared< request::" + connection.request( ).name( ) + "Data >( );\n"
            code += "                     break;\n"
            code += "                  }\n"
            if None != connection.response( ):
               code += "                  case carpc::service::eType::RESPONSE:\n"
               code += "                  {\n"
               code += "                     ptr = std::make_shared< response::" + connection.response( ).name( ) + "Data >( );\n"
               code += "                     break;\n"
               code += "                  }\n"
            code += "                  default: break;\n"
            code += "               }\n"
            code += "               break;\n"
            code += "            }\n"
         code += "            default: break;\n"
         code += "         }\n"
         code += "\n"
         code += "         if( nullptr != ptr )\n"
         code += "            ptr->from_stream( stream );\n"
         code += "\n"
         code += "         return  ptr;\n"
         code += "      }\n"
         code += "\n"

         code += "      bool BaseData::serrialize( carpc::ipc::tStream& stream )\n"
         code += "      {\n"
         code += "         if( false == carpc::ipc::serialize( stream, id( ), type( ) ) )\n"
         code += "            return false;\n"
         code += "         return to_stream( stream );\n"
         code += "      }\n"
         code += "\n"

      def build_function_class( function: interface.Function ):
         code: str = "";
         if None == function:
            return code

         function_name = function.name( )
         function_arguments = function.arguments( )
         function_type = str( function.type( ) )

         code += "      namespace " + function_type.lower( ) + " {\n"
         code += "\n"
         if 0 < len( function_arguments ):
            code += "         " + function_name + "Data::" + function_name + "Data( "
            for count in range( len( function_arguments ) ):
               argument = function_arguments[count]
               code += "const " + argument.type( ) + "& _" + argument.name( )
               code += ", " if len( function_arguments ) > ( count + 1 ) else " "
            code += ")\n"
            code += "            : BaseData( )\n"
            for argument in function_arguments:
               code += "            , " + argument.name( ) + "( _" + argument.name( ) + ")\n"
            code += "         {\n"
            code += "         }\n"
            code += "\n"

         # Only ipc methods
         if True == self.__interface.is_ipc( ):
            code += "         bool " + function_name + "Data::to_stream( carpc::ipc::tStream& stream )\n"
            code += "         {\n"
            code += "            return carpc::ipc::serialize( stream"
            for argument in function_arguments:
               code += ", " + argument.name( )
            code += " );\n"
            code += "         }\n"
            code += "\n"

            code += "         bool " + function_name + "Data::from_stream( carpc::ipc::tStream& stream )\n"
            code += "         {\n"
            code += "            return carpc::ipc::deserialize( stream"
            for argument in function_arguments:
               code += ", " + argument.name( )
            code += " );\n"
            code += "         }\n"
            code += "\n"

         code += "      } // namespace " + function_type.lower( ) + "\n"
         code += "\n"

         return code
      # def build_function_class

      for connection in self.__interface.connections( ):
         code += build_function_class( connection.request( ) )
         code += build_function_class( connection.response( ) )

      # } // namespace method
      code += "   } // namespace method\n"
      code += "\n"



      # namespace attribute {
      code += "   namespace attribute {\n"
      code += "\n"

      # Only ipc methods
      if True == self.__interface.is_ipc( ):
         code += "      BaseData::tSptr BaseData::create( carpc::ipc::tStream& stream )\n"
         code += "      {\n"
         code += "         eID id = eID::UNDEFINED;\n"
         code += "         carpc::service::eType type = carpc::service::eType::UNDEFINED;\n"
         code += "         if( false == carpc::ipc::deserialize( stream, id, type ) )\n"
         code += "            return nullptr;\n"
         code += "\n"
         code += "         tSptr ptr = nullptr;\n"
         code += "         switch( id )\n"
         code += "         {\n"
         for attribute in self.__interface.attributes( ):
            code += "            case eID::" + attribute.name( ) + ":\n"
            code += "            {\n"
            code += "               switch( type )\n"
            code += "               {\n"
            code += "                  case carpc::service::eType::NOTIFICATION:\n"
            code += "                  {\n"
            code += "                     ptr = std::make_shared< notification::" + attribute.name( ) + "Data >( );\n"
            code += "                     break;\n"
            code += "                  }\n"
            code += "                  default: break;\n"
            code += "               }\n"
            code += "            }\n"
         code += "            default: break;\n"
         code += "         }\n"
         code += "\n"
         code += "         if( nullptr != ptr )\n"
         code += "            ptr->from_stream( stream );\n"
         code += "\n"
         code += "         return  ptr;\n"
         code += "      }\n"
         code += "\n"

         code += "      bool BaseData::serrialize( carpc::ipc::tStream& stream )\n"
         code += "      {\n"
         code += "         if( false == carpc::ipc::serialize( stream, id( ), type( ) ) )\n"
         code += "            return false;\n"
         code += "         return to_stream( stream );\n"
         code += "      }\n"
         code += "\n"

      def build_attribute_class ( attribute: interface.Parameter, namespace: str ):
         code: str = "";
         if None == attribute:
            return code

         attribute_type = attribute.type( )
         attribute_name = attribute.name( )

         code += "      namespace " + namespace + " {\n"
         code += "\n"
         code += "         " + attribute_name + "Data::" + attribute_name + "Data( " \
            + "const " + attribute_type + "& _value )\n"
         code += "            : BaseData( )\n"
         code += "            , value( _value )\n"
         code += "         {\n"
         code += "         }\n"
         code += "\n"

         # Only ipc methods
         if True == self.__interface.is_ipc( ):
            code += "         bool " + attribute_name + "Data::to_stream( carpc::ipc::tStream& stream )\n"
            code += "         {\n"
            code += "            return carpc::ipc::serialize( stream, value );\n"
            code += "         }\n"
            code += "\n"

            code += "         bool " + attribute_name + "Data::from_stream( carpc::ipc::tStream& stream )\n"
            code += "         {\n"
            code += "            return carpc::ipc::deserialize( stream, value );\n"
            code += "         }\n"
            code += "\n"

         code += "      } // namespace " + namespace + "\n"
         code += "\n"

         return code
      # def build_attribute_class

      for attribute in self.__interface.attributes( ):
         code += build_attribute_class( attribute, "notification" )

      # } // namespace attribute
      code += "   } // namespace attribute\n"
      code += "\n"



      code += "} // namespace " + self.__interface.namespace( ) + "\n"
      code += "\n"

      self.generate( code, "Data.cpp", self.__imp_dir )
      return code;
   # def build_data_cpp

   def build_iserver_hpp( self ):
      code: str = "";

      code += "#pragma once\n";
      code += "\n"
      code += "#include \"carpc/comm/service/experimental/TServer.hpp\"\n";
      code += "#include \"Data.hpp\"\n";
      code += "\n"

      code += "namespace " + self.__interface.namespace( ) + " {\n"
      code += "\n"

      code += "   class IServer\n"
      code += "      : public carpc::service::experimental::TServer< Types >\n"
      code += "   {\n"
      code += "      public:\n"
      code += "         IServer( const std::string& role_name );\n"
      code += "         ~IServer( ) override = default;\n"
      code += "\n"
      code += "      private:\n"
      code += "         void process_request_event( const tMethod::Event& event ) override;\n"
      code += "\n"

      for connection in self.__interface.connections( ):
         function = connection.request( )
         function_name = function.name( )
         function_arguments = function.arguments( )
         code += "      private:\n"
         code += "         virtual void request_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "const " + argument.type( ) + "& _" + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ") = 0;\n"

         function = connection.response( )
         if None == function:
            code += "\n"
            continue
         function_name = function.name( )
         function_arguments = function.arguments( )
         code += "      public:\n"
         code += "         void response_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "const " + argument.type( ) + "& _" + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ");\n"

         code += "\n"
      code += "\n"

      code += "      public:\n"
      # attributes setters and getters
      for attribute in self.__interface.attributes( ):
         attribute_type = attribute.type( )
         attribute_name = attribute.name( )
         code += "         void " + attribute_name + "( const " + attribute_type + "& );\n"
         code += "         const " + attribute_type + "& " + attribute_name + "( ) const;\n"
      code += "   };\n"

      code += "\n"
      code += "} // namespace " + self.__interface.namespace( ) + "\n"
      code += "\n"

      self.generate( code, "IServer.hpp", self.__imp_dir )
      return code;
   # def build_iserver_hpp

   def build_iserver_cpp( self ):
      code: str = "";

      code += "#include \"IServer.hpp\"\n";
      code += "\n"

      code += "using namespace " + self.__interface.namespace( ) + ";\n"
      code += "\n"

      code += "IServer::IServer( const std::string& role_name )\n"
      code += "   : carpc::service::experimental::TServer< Types >( role_name, CARPC_IS_IPC_TYPE( Types::tIPC ) )\n"
      code += "{\n"
      code += "}\n"
      code += "\n"

      for connection in self.__interface.connections( ):
         function = connection.response( )
         if None == function: continue
         function_name = function.name( )
         function_arguments = function.arguments( )
         code += "void IServer::response_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "const " + argument.type( ) + "& " + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ")\n"
         code += "{\n"
         code += "   response< method::response::" + function_name + "Data >( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ");\n"
         code += "}\n"
         code += "\n"

      code += "void IServer::process_request_event( const tMethod::Event& event )\n"
      code += "{\n"
      code += "   method::eID event_id = event.info( ).id( );\n"
      code += "   carpc::service::eType event_type = event.info( ).type( );\n"
      code += "\n"
      code += "   if( carpc::service::eType::REQUEST != event_type )\n"
      code += "      return;\n"
      code += "\n"
      code += "   switch( event_id )\n"
      code += "   {\n"
      for connection in self.__interface.connections( ):
         function = connection.request( )
         if None == function: continue
         function_name = function.name( )
         function_arguments = function.arguments( )
         code += "      case method::eID::" + connection.name( ) + ":\n"
         code += "      {\n"
         code += "         using tMethodData = method::request::" + function_name + "Data;\n"
         code += "         if( const tMethodData* p_event_data = get_event_data< tMethodData >( event ) )\n"
         code += "            request_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "p_event_data->" + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ");\n"
         code += "         break;\n"
         code += "      }\n"
      code += "      default: break;\n"
      code += "   }\n"
      code += "}\n"
      code += "\n"

      for attribute in self.__interface.attributes( ):
         attribute_type = attribute.type( )
         attribute_name = attribute.name( )

         code += "void IServer::" + attribute_name + "( const " + attribute_type + "& value )\n"
         code += "{\n"
         code += "   notify< attribute::notification::" + attribute_name + "Data >( value );\n"
         code += "}\n"
         code += "\n"

         code += "const " + attribute_type + "& IServer::" + attribute_name + "( ) const\n"
         code += "{\n"
         code += "   if( auto p_data = attribute< attribute::notification::" + attribute_name + "Data >( ) )\n"
         code += "      return p_data->value;\n"
         code += "\n"
         code += "   static const " + attribute_type + " dummy{ };\n"
         code += "   return dummy;\n"
         code += "}\n"
         code += "\n"

      self.generate( code, "IServer.cpp", self.__imp_dir )
      return code;
   # def build_iserver_cpp

   def build_iclient_hpp( self ):
      code: str = "";

      code += "#pragma once\n";
      code += "\n"
      code += "#include \"carpc/comm/service/experimental/TClient.hpp\"\n";
      code += "#include \"Data.hpp\"\n";
      code += "\n"

      code += "namespace " + self.__interface.namespace( ) + " {\n"
      code += "\n"

      code += "   class IClient\n"
      code += "      : public carpc::service::experimental::TClient< Types >\n"
      code += "   {\n"
      code += "      public:\n"
      code += "         IClient( const std::string& role_name );\n"
      code += "         ~IClient( ) override = default;\n"
      code += "\n"
      code += "      private:\n"
      code += "         void process_event( const tMethod::Event& event ) override;\n"
      code += "         void process_event( const tAttribute::Event& event ) override;\n"
      code += "\n"

      for connection in self.__interface.connections( ):
         code += "      public:\n"
         function = connection.request( )
         function_name = function.name( )
         function_arguments = function.arguments( )
         return_type: str = "void" if None == connection.response( ) else "carpc::comm::sequence::ID"
         code += "         " + return_type + " request_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "const " + argument.type( ) + "& " + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ");\n"

         function = connection.response( )
         if None == function:
            code += "\n"
            continue
         code += "      private:\n"
         function_name = function.name( )
         function_arguments = function.arguments( )
         code += "         virtual void response_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "const " + argument.type( ) + "& _" + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ") = 0;\n"

         # request_failed
         # It will be generated only for requests what have responses => only when response was generated on current iteration
         function = connection.request( )
         if None == function: continue
         function_name = function.name( )
         function_arguments = function.arguments( )
         code += "         virtual void request_" + function_name + "_failed( const carpc::service::eError& error ) = 0;\n"

         code += "\n"
      code += "\n"

      for attribute in self.__interface.attributes( ):
         attribute_type = attribute.type( )
         attribute_name = attribute.name( )
         code += "      public:\n"
         code += "         void subscribe_" + attribute_name + "( );\n"
         code += "         void unsubscribe_" + attribute_name + "( );\n"
         code += "      private:\n"
         code += "         virtual void on_" + attribute_name + "( const " + attribute_type + "& ) = 0;\n"
      code += "   };\n"

      code += "\n"
      code += "} // namespace " + self.__interface.namespace( ) + "\n"
      code += "\n"

      self.generate( code, "IClient.hpp", self.__imp_dir )
      return code;
   # def build_iclient_hpp

   def build_iclient_cpp( self ):
      code: str = "";

      code += "#include \"IClient.hpp\"\n";
      code += "\n"

      code += "using namespace " + self.__interface.namespace( ) + ";\n"
      code += "\n"

      code += "IClient::IClient( const std::string& role_name )\n"
      code += "   : carpc::service::experimental::TClient< Types >( role_name, CARPC_IS_IPC_TYPE( Types::tIPC ) )\n"
      code += "{\n"
      code += "}\n"
      code += "\n"

      for connection in self.__interface.connections( ):
         function = connection.request( )
         if None == function: continue
         function_name = function.name( )
         function_arguments = function.arguments( )
         return_type: str = "void" if None == connection.response( ) else "carpc::comm::sequence::ID"
         code += return_type + " IClient::request_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "const " + argument.type( ) + "& " + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ")\n"
         code += "{\n"
         is_return: str = "" if None == connection.response( ) else "return "
         code += "   " + is_return + "request< method::request::" + function_name + "Data >( this"
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += ", " + argument.name( )
         code += " );\n"
         code += "}\n"
         code += "\n"

      for attribute in self.__interface.attributes( ):
         attribute_type = attribute.type( )
         attribute_name = attribute.name( )

         code += "void IClient::subscribe_" + attribute_name + "( )\n"
         code += "{\n"
         code += "   subscribe< attribute::notification::" + attribute_name + "Data >( this );\n"
         code += "}\n"
         code += "\n"

         code += "void IClient::unsubscribe_" + attribute_name + "( )\n"
         code += "{\n"
         code += "   unsubscribe< attribute::notification::" + attribute_name + "Data >( this );\n"
         code += "}\n"
         code += "\n"

      code += "void IClient::process_event( const tMethod::Event& event )\n"
      code += "{\n"
      code += "   method::eID event_id = event.info( ).id( );\n"
      code += "   carpc::service::eType event_type = event.info( ).type( );\n"
      code += "\n"
      code += "   switch( event_id )\n"
      code += "   {\n"
      for connection in self.__interface.connections( ):
         function = connection.response( )
         if None == function: continue
         function_name = function.name( )
         function_arguments = function.arguments( )
         code += "      case method::eID::" + connection.name( ) + ":\n"
         code += "      {\n"
         code += "         if( carpc::service::eType::RESPONSE == event_type )\n"
         code += "         {\n"
         code += "            using tMethodData = method::response::" + function_name + "Data;\n"
         code += "            if( const tMethodData* p_event_data = get_event_data< tMethodData >( event ) )\n"
         code += "               response_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "p_event_data->" + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ");\n"
         code += "            else\n"
         code += "               request_" + connection.request( ).name( ) + "_failed( carpc::service::eError::EmptyData );\n"
         code += "         }\n"
         code += "         else if( carpc::service::eType::BUSY == event_type )\n"
         code += "         {\n"
         code += "            request_" + connection.request( ).name( ) + "_failed( carpc::service::eError::RequestBusy );\n"
         code += "         }\n"
         code += "         break;\n"
         code += "      }\n"
      code += "      default: break;\n"
      code += "   }\n"
      code += "}\n"
      code += "\n"

      code += "void IClient::process_event( const tAttribute::Event& event )\n"
      code += "{\n"
      code += "   attribute::eID event_id = event.info( ).id( );\n"
      code += "   carpc::service::eType event_type = event.info( ).type( );\n"
      code += "\n"
      code += "   switch( event_id )\n"
      code += "   {\n"
      for attribute in self.__interface.attributes( ):
         attribute_name = attribute.name( )
         code += "      case attribute::eID::" + attribute_name + ":\n"
         code += "      {\n"
         code += "         if( carpc::service::eType::NOTIFICATION == event_type )\n"
         code += "         {\n"
         code += "            using tAttributeData = attribute::notification::" + attribute_name + "Data;\n"
         code += "            if( const tAttributeData* p_event_data = get_event_data< tAttributeData >( event ) )\n"
         code += "               on_" + attribute_name + "( p_event_data->value );\n"
         code += "         }\n"
         code += "         break;\n"
         code += "      }\n"
      code += "      default: break;\n"
      code += "   }\n"
      code += "}\n"
      code += "\n"

      self.generate( code, "IClient.cpp", self.__imp_dir )
      return code;
   # def build_iclient_cpp

   def build_server_impl_hpp( self ):
      code: str = "";

      code += "#pragma once\n";
      code += "\n"
      code += "#include \"IServer.hpp\"\n";
      code += "\n"

      code += "namespace " + self.__interface.namespace( ) + " {\n"
      code += "\n"
      code += "   class Server;\n"
      code += "\n"

      code += "   class ServerImpl\n"
      code += "      : public IServer\n"
      code += "   {\n"
      code += "         friend class Server;\n"
      code += "\n"
      code += "      public:\n"
      code += "         ServerImpl( Server& server, const std::string& role_name );\n"
      code += "         ~ServerImpl( ) override = default;\n"
      code += "\n"
      code += "      private:\n"
      code += "         void connected( ) override;\n"
      code += "         void disconnected( ) override;\n"
      code += "\n"

      code += "      public:\n"
      for connection in self.__interface.connections( ):
         function = connection.request( )
         function_name = function.name( )
         function_arguments = function.arguments( )
         code += "         void request_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "const " + argument.type( ) + "& _" + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ") override;\n"
      code += "\n"

      code += "      private:\n"
      code += "         Server& m_server;\n"
      code += "   };\n"

      code += "\n"
      code += "} // namespace " + self.__interface.namespace( ) + "\n"
      code += "\n"

      self.generate( code, "ServerImpl.hpp", self.__imp_dir )
      return code;
   # def build_server_impl_hpp

   def build_server_impl_cpp( self ):
      code: str = "";

      code += "#include \""
      if None != self.__api_dir: code += self.__api_dir + "/"
      code += "Server.hpp\"\n";
      code += "#include \"ServerImpl.hpp\"\n";
      code += "\n"

      code += "using namespace " + self.__interface.namespace( ) + ";\n"
      code += "\n"

      code += "ServerImpl::ServerImpl( Server& server, const std::string& role_name )\n"
      code += "   : IServer( role_name )\n"
      code += "   , m_server( server )\n"
      code += "{\n"
      code += "}\n"
      code += "\n"

      code += "void ServerImpl::connected( )\n"
      code += "{\n"
      code += "   m_server.connected( );\n"
      code += "}\n"
      code += "\n"

      code += "void ServerImpl::disconnected( )\n"
      code += "{\n"
      code += "   m_server.disconnected( );\n"
      code += "}\n"
      code += "\n"

      for connection in self.__interface.connections( ):
         function = connection.request( )
         if None == function: continue
         function_name = function.name( )
         function_arguments = function.arguments( )
         code += "void ServerImpl::request_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "const " + argument.type( ) + "& " + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ")\n"
         code += "{\n"
         code += "   m_server.request_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ");\n"
         code += "}\n"
         code += "\n"

      self.generate( code, "ServerImpl.cpp", self.__imp_dir )
      return code;
   # def build_server_impl_cpp

   def build_client_impl_hpp( self ):
      code: str = "";

      code += "#pragma once\n";
      code += "\n"
      code += "#include \"IClient.hpp\"\n";
      code += "\n"

      code += "namespace " + self.__interface.namespace( ) + " {\n"
      code += "\n"
      code += "   class Client;\n"
      code += "\n"

      code += "   class ClientImpl\n"
      code += "      : public IClient\n"
      code += "   {\n"
      code += "         friend class Client;\n"
      code += "\n"
      code += "      public:\n"
      code += "         ClientImpl( Client& client, const std::string& role_name );\n"
      code += "         ~ClientImpl( ) override = default;\n"
      code += "\n"
      code += "      private:\n"
      code += "         void connected( ) override;\n"
      code += "         void disconnected( ) override;\n"
      code += "\n"

      code += "      public:\n"
      for connection in self.__interface.connections( ):
         function = connection.response( )
         if None == function: continue
         function_name = function.name( )
         function_arguments = function.arguments( )
         code += "         void response_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "const " + argument.type( ) + "& _" + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ") override;\n"

         # request_failed
         # It will be generated only for requests what have responses => only when response was generated on current iteration
         function = connection.request( )
         if None == function: continue
         function_name = function.name( )
         function_arguments = function.arguments( )
         code += "         virtual void request_" + function_name + "_failed( const carpc::service::eError& error ) override;\n"

         code += "\n"

      code += "      public:\n"
      for attribute in self.__interface.attributes( ):
         attribute_type = attribute.type( )
         attribute_name = attribute.name( )
         code += "         void on_" + attribute_name + "( const " + attribute_type + "& _value ) override;\n"
      code += "\n"

      code += "      private:\n"
      code += "         Client& m_client;\n"
      code += "   };\n"

      code += "\n"
      code += "} // namespace " + self.__interface.namespace( ) + "\n"
      code += "\n"

      self.generate( code, "ClientImpl.hpp", self.__imp_dir )
      return code;
   # def build_client_impl_hpp

   def build_client_impl_cpp( self ):
      code: str = "";

      code += "#include \""
      if None != self.__api_dir: code += self.__api_dir + "/"
      code += "Client.hpp\"\n";
      code += "#include \"ClientImpl.hpp\"\n";
      code += "\n"

      code += "using namespace " + self.__interface.namespace( ) + ";\n"
      code += "\n"

      code += "ClientImpl::ClientImpl( Client& client, const std::string& role_name )\n"
      code += "   : IClient( role_name )\n"
      code += "   , m_client( client )\n"
      code += "{\n"
      code += "}\n"
      code += "\n"

      code += "void ClientImpl::connected( )\n"
      code += "{\n"
      code += "   m_client.connected( );\n"
      code += "}\n"
      code += "\n"

      code += "void ClientImpl::disconnected( )\n"
      code += "{\n"
      code += "   m_client.disconnected( );\n"
      code += "}\n"
      code += "\n"

      for connection in self.__interface.connections( ):
         function = connection.response( )
         if None == function: continue
         function_name = function.name( )
         function_arguments = function.arguments( )
         code += "void ClientImpl::response_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "const " + argument.type( ) + "& " + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ")\n"
         code += "{\n"
         code += "   m_client.response_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ");\n"
         code += "}\n"
         code += "\n"

         function = connection.request( )
         function_name = function.name( )
         code += "void ClientImpl::request_" + function_name + "_failed( const carpc::service::eError& error )\n"
         code += "{\n"
         code += "   m_client.request_" + function_name + "_failed( error );\n"
         code += "}\n"
         code += "\n"

      for attribute in self.__interface.attributes( ):
         attribute_type = attribute.type( )
         attribute_name = attribute.name( )
         code += "void ClientImpl::on_" + attribute_name + "( const " + attribute_type + "& value )\n"
         code += "{\n"
         code += "   m_client.on_" + attribute_name + "( value );\n"
         code += "}\n"
         code += "\n"

      self.generate( code, "ClientImpl.cpp", self.__imp_dir )
      return code;
   # def build_client_impl_cpp

   def build_server_hpp( self ):
      code: str = "";

      code += "#pragma once\n";
      code += "\n"
      code += "#include \"Version.hpp\"\n";
      code += "\n"

      code += "namespace " + self.__interface.namespace( ) + " {\n"
      code += "\n"
      code += "   class ServerImpl;\n"
      code += "\n"

      code += "   class Server\n"
      code += "   {\n"
      code += "         friend class ServerImpl;\n"
      code += "\n"
      code += "      public:\n"
      code += "         using tSptr = std::shared_ptr< Server >;\n"
      code += "\n"
      code += "      public:\n"
      code += "         Server( const std::string& role_name );\n"
      code += "         virtual ~Server( );\n"
      code += "\n"
      code += "      private:\n"
      code += "         virtual void connected( ) = 0;\n"
      code += "         virtual void disconnected( ) = 0;\n"
      code += "\n"

      code += "      public:\n"
      for connection in self.__interface.connections( ):
         function = connection.request( )
         function_name = function.name( )
         function_arguments = function.arguments( )
         code += "         virtual void request_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "const " + argument.type( ) + "& _" + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ") = 0;\n"

         function = connection.response( )
         if None == function:
            code += "\n"
            continue
         function_name = function.name( )
         function_arguments = function.arguments( )
         code += "         void response_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "const " + argument.type( ) + "& _" + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ");\n"

         code += "\n"
      code += "\n"

      code += "      public:\n"
      # attributes setters and getters
      for attribute in self.__interface.attributes( ):
         attribute_type = attribute.type( )
         attribute_name = attribute.name( )
         code += "         void " + attribute_name + "( const " + attribute_type + "& );\n"
         code += "         const " + attribute_type + "& " + attribute_name + "( ) const;\n"
      code += "\n"

      code += "      public:\n"
      code += "         const carpc::comm::sequence::ID unblock_request( );\n"
      code += "         void prepare_response( const carpc::comm::sequence::ID& );\n"
      code += "\n"
      code += "      private:\n"
      code += "         ServerImpl* mp_impl = nullptr;\n"

      code += "   };\n"

      code += "\n"
      code += "} // namespace " + self.__interface.namespace( ) + "\n"
      code += "\n"

      self.generate( code, "Server.hpp", self.__api_dir )
      return code;
   # def build_server_hpp

   def build_server_cpp( self ):
      code: str = "";

      code += "#include \"ServerImpl.hpp\"\n";
      code += "#include \""
      if None != self.__api_dir: code += self.__api_dir + "/"
      code += "Server.hpp\"\n";
      code += "\n"

      code += "using namespace " + self.__interface.namespace( ) + ";\n"
      code += "\n"

      code += "Server::Server( const std::string& role_name )\n"
      code += "{\n"
      code += "   mp_impl = new ServerImpl( *this, role_name );\n"
      code += "}\n"
      code += "\n"

      code += "Server::~Server( )\n"
      code += "{\n"
      code += "   delete mp_impl;\n"
      code += "}\n"
      code += "\n"

      for connection in self.__interface.connections( ):
         function = connection.response( )
         if None == function: continue
         function_name = function.name( )
         function_arguments = function.arguments( )
         code += "void Server::response_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "const " + argument.type( ) + "& " + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ")\n"
         code += "{\n"
         code += "   mp_impl->response_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ");\n"
         code += "}\n"
         code += "\n"

      for attribute in self.__interface.attributes( ):
         attribute_type = attribute.type( )
         attribute_name = attribute.name( )

         code += "void Server::" + attribute_name + "( const " + attribute_type + "& value )\n"
         code += "{\n"
         code += "   mp_impl->" + attribute_name + "( value );\n"
         code += "}\n"
         code += "\n"

         code += "const " + attribute_type + "& Server::" + attribute_name + "( ) const\n"
         code += "{\n"
         code += "   return mp_impl->" + attribute_name + "( );\n"
         code += "}\n"
         code += "\n"

         code += "const carpc::comm::sequence::ID Server::unblock_request( )\n"
         code += "{\n"
         code += "   return mp_impl->unblock_request( );\n"
         code += "}\n"
         code += "\n"
         code += "void Server::prepare_response( const carpc::comm::sequence::ID& seq_id )\n"
         code += "{\n"
         code += "   mp_impl->prepare_response( seq_id );\n"
         code += "}\n"
         code += "\n"

      self.generate( code, "Server.cpp", self.__imp_dir )
      return code;
   # def build_server_cpp

   def build_client_hpp( self ):
      code: str = "";

      code += "#pragma once\n";
      code += "\n"
      code += "#include \"Version.hpp\"\n";
      code += "\n"

      code += "namespace " + self.__interface.namespace( ) + " {\n"
      code += "\n"
      code += "   class ClientImpl;\n"
      code += "\n"

      code += "   class Client\n"
      code += "   {\n"
      code += "         friend class ClientImpl;\n"
      code += "\n"
      code += "      public:\n"
      code += "         using tSptr = std::shared_ptr< Client >;\n"
      code += "\n"
      code += "      public:\n"
      code += "         Client( const std::string& role_name );\n"
      code += "         virtual ~Client( );\n"
      code += "\n"
      code += "      private:\n"
      code += "         virtual void connected( ) = 0;\n"
      code += "         virtual void disconnected( ) = 0;\n"
      code += "\n"

      code += "      public:\n"
      for connection in self.__interface.connections( ):
         function = connection.request( )
         if None == function:
            continue
         function_name = function.name( )
         function_arguments = function.arguments( )
         return_type: str = "void" if None == connection.response( ) else "carpc::comm::sequence::ID"
         code += "         " + return_type + " request_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "const " + argument.type( ) + "& " + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ");\n"

         function = connection.response( )
         if None == function:
            code += "\n"
            continue
         function_name = function.name( )
         function_arguments = function.arguments( )
         code += "         virtual void response_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "const " + argument.type( ) + "& " + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ") = 0;\n"

         function = connection.request( )
         function_name = function.name( )
         code += "         virtual void request_" + function_name + "_failed( const carpc::service::eError& ) = 0;\n"

         code += "\n"
      code += "\n"

      code += "      public:\n"
      # attributes setters and getters
      for attribute in self.__interface.attributes( ):
         attribute_type = attribute.type( )
         attribute_name = attribute.name( )
         code += "         virtual void on_" + attribute_name + "( const " + attribute_type + "& ) = 0;\n"
         code += "         void subscribe_" + attribute_name + "( );\n"
         code += "         void unsubscribe_" + attribute_name + "( );\n"

      code += "\n"

      code += "      private:\n"
      code += "         ClientImpl* mp_impl = nullptr;\n"

      code += "   };\n"

      code += "\n"
      code += "} // namespace " + self.__interface.namespace( ) + "\n"
      code += "\n"

      self.generate( code, "Client.hpp", self.__api_dir )
      return code;
   # def build_client_hpp

   def build_client_cpp( self ):
      code: str = "";

      code += "#include \"ClientImpl.hpp\"\n";
      code += "#include \""
      if None != self.__api_dir: code += self.__api_dir + "/"
      code += "Client.hpp\"\n";
      code += "\n"

      code += "using namespace " + self.__interface.namespace( ) + ";\n"
      code += "\n"

      code += "Client::Client( const std::string& role_name )\n"
      code += "{\n"
      code += "   mp_impl = new ClientImpl( *this, role_name );\n"
      code += "}\n"
      code += "\n"

      code += "Client::~Client( )\n"
      code += "{\n"
      code += "   delete mp_impl;\n"
      code += "}\n"
      code += "\n"

      for connection in self.__interface.connections( ):
         function = connection.request( )
         if None == function: continue
         function_name = function.name( )
         function_arguments = function.arguments( )
         return_type: str = "void" if None == connection.response( ) else "carpc::comm::sequence::ID"
         code += return_type + " Client::request_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += "const " + argument.type( ) + "& " + argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ")\n"
         code += "{\n"
         code += "   return mp_impl->request_" + function_name + "( "
         for count in range( len( function_arguments ) ):
            argument = function_arguments[count]
            code += argument.name( )
            code += ", " if len( function_arguments ) > ( count + 1 ) else " "
         code += ");\n"
         code += "}\n"
         code += "\n"

      for attribute in self.__interface.attributes( ):
         attribute_type = attribute.type( )
         attribute_name = attribute.name( )
         code += "void Client::subscribe_" + attribute_name + "( )\n"
         code += "{\n"
         code += "   return mp_impl->subscribe_" + attribute_name + "( );\n"
         code += "}\n"
         code += "\n"

         code += "void Client::unsubscribe_" + attribute_name + "( )\n"
         code += "{\n"
         code += "   return mp_impl->unsubscribe_" + attribute_name + "( );\n"
         code += "}\n"
         code += "\n"

      self.generate( code, "Client.cpp", self.__imp_dir )
      return code;
   # def build_client_cpp



   __interface: Interface = None
   __gen_dir: str = None
   __api_dir: str = None
   __imp_dir: str = None
# class Builder
