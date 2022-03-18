#include <fstream>
#include <algorithm>

#include "api/sys/helpers/functions/format.hpp"
#include "api/sys/tools/Tools.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TOOLS"



#include <filesystem>
namespace carpc::tools {

   BasePCE::BasePCE( const std::string& dump_message )
      : m_dump_message( dump_message )
   {
   }

   BasePCE::tValueOpt BasePCE::value( const tParameter& parameter ) const
   {
      const auto iterator = m_map.find( parameter );
      if( m_map.end( ) == iterator )
         return std::nullopt;

      return iterator->second;
   }

   BasePCE::tValue BasePCE::value_or( const tParameter& parameter, const tValue& default_value ) const
   {
      const auto iterator = m_map.find( parameter );
      if( m_map.end( ) == iterator )
         return default_value;

      return iterator->second;
   }

   void BasePCE::print( ) const
   {
      const char* line = "----------------------------------------------";
      MSG_WRN( "%s BEGIN: %s %s", line, m_dump_message.c_str( ), line );
      for( auto pair : m_map )
      {
         MSG_VRB( "   '%s' = '%s'", pair.first.c_str( ), pair.second.c_str( ) );
      }
      MSG_WRN( "%s  END:  %s %s", line, m_dump_message.c_str( ), line );
   }



   Parameters::Parameters( )
      : BasePCE( "Parameters" )
   {
   }

   Parameters::Parameters( int argc, char** argv, const std::string& delimiter )
      : BasePCE( "Parameters" )
   {
      init( argc, argv, delimiter );
   }

   void Parameters::init( int argc, char** argv, const std::string& delimiter )
   {
      if( nullptr == argv ) return;

      for( int count = 0; count < argc; ++count )
      {
         tArgument argument( argv[ count ] );
         std::size_t position = argument.find( delimiter );
         if( std::string::npos == position )
            m_map.emplace( argument, std::string( "" ) );
         else
            m_map.emplace( argument.substr( 0, position ), argument.substr( position + 1 ) );
      }
   }



   Environment::Environment( )
      : BasePCE( "Environment" )
   {
   }

   Environment::Environment( char** envp, const std::string& delimiter )
      : BasePCE( "Environment" )
   {
      init( envp, delimiter );
   }

   void Environment::init( char** envp, const std::string& delimiter )
   {
      if( nullptr == envp ) return;

      for( int count = 0; nullptr != envp[ count ]; ++count )
      {
         tArgument argument( envp[ count ] );
         std::size_t position = argument.find( delimiter );
         if( std::string::npos == position )
            m_map.emplace( argument, std::string( "" ) );
         else
            m_map.emplace( argument.substr( 0, position ), argument.substr( position + 1 ) );
      }
   }



   Configuration::Configuration( )
      : BasePCE( "Configuration" )
   {
   }

   Configuration::Configuration( const std::string& file, const std::string& delimiter )
      : BasePCE( "Configuration" )
   {
      init( file, delimiter );
   }

   void Configuration::init( const std::string& file, const std::string& delimiter )
   {
      std::ifstream file_stream;
      file_stream.open( file.c_str( ) );
      // std::cout << file_stream.rdbuf( ) << std::endl;

      if( false == file_stream.is_open( ) )
      {
         MSG_WRN( "configuration file '%s' can't be found", file.c_str( ) );
         return;
      }

      while( file_stream )
      {
         std::string line;
         std::getline( file_stream, line, '\n' );

         line = carpc::trim( line );
         if( true == line.empty( ) )
            continue;
         if( '#' == line[0] )
            continue;

         std::size_t position = line.find( delimiter );
         if( std::string::npos == position )
            m_map.emplace( line, std::string("") );
         else
            m_map.emplace( line.substr( 0, position ), line.substr( position + 1 ) );
      }

      file_stream.close( );
   }



   PCE::PCE( int argc, char** argv, char** envp, const std::string& delimiter )
      : Parameters( )
      , Environment( )
      , Configuration( )
   {
      PCE::init( argc, argv, envp, delimiter );
   }

   void PCE::init( int argc, char** argv, char** envp, const std::string& delimiter )
   {
      Parameters::init( argc, argv, delimiter );
      Environment::init( envp, delimiter );

      std::string file_name = std::filesystem::path( *argv ).filename( );
      file_name += std::string( ".cfg" );
      file_name = Parameters::value_or( "config", file_name );

      Configuration::init( file_name, delimiter );
   }

   void PCE::print( const eType& type ) const
   {
      switch( type )
      {
         case eType::CMD:
         {
            Parameters::print( );
            break;
         }
         case eType::ENV:
         {
            Environment::print( );
            break;
         }
         case eType::CFG:
         {
            Configuration::print( );
            break;
         }
         default:
         {
            Parameters::print( );
            Environment::print( );
            Configuration::print( );
            break;
         }
      }
   }

   PCE::tValueOpt PCE::value( const tParameter& parameter, const eType& type ) const
   {
      switch( type )
      {
         case eType::CMD: return Parameters::value( parameter );
         case eType::ENV: return Environment::value( parameter );
         case eType::CFG: return Configuration::value( parameter );
         default: break;
      }

      tValueOpt value_opt = Parameters::value( parameter );
      if( std::nullopt != value_opt )
         return value_opt;

      value_opt = Environment::value( parameter );
      if( std::nullopt != value_opt )
         return value_opt;

      value_opt = Configuration::value( parameter );
      return value_opt;
   }

   PCE::tValue PCE::value_or( const tParameter& parameter, const tValue& default_value, const eType& type ) const
   {
      switch( type )
      {
         case eType::CMD: return Parameters::value_or( parameter, default_value );
         case eType::ENV: return Environment::value_or( parameter, default_value );
         case eType::CFG: return Configuration::value_or( parameter, default_value );
         default: break;
      }

      tValueOpt value_opt = Parameters::value( parameter );
      if( std::nullopt != value_opt )
         return value_opt.value( );

      value_opt = Environment::value( parameter );
      if( std::nullopt != value_opt )
         return value_opt.value( );

      value_opt = Configuration::value( parameter );
      return value_opt.value_or( default_value );
   }

} // namespace carpc::tools



namespace carpc::tools::cmd {

   char* get_option( int argc, char** begin, const std::string & option )
   {
      char** end = begin + argc;
      char** itr = std::find( begin, end, option );
      if( itr != end && ++itr != end )
         return *itr;

      return nullptr;
   }

   bool is_option_exists( int argc, char** begin, const std::string& option )
   {
      char** end = begin + argc;
      return std::find( begin, end, option ) != end;
   }

} // namespace carpc::tools::cmd



#include <random>
#include <sstream>
// #include "uuid/uuid.h"

// link: https://stackoverflow.com/a/60198074/12978480
namespace carpc::tools::uuid {

   static std::random_device                 rd;
   static std::mt19937                       gen( rd( ) );
   static std::uniform_int_distribution< >   dis( 0, 15 );
   static std::uniform_int_distribution< >   dis2( 8, 11 );

   void generate_random( std::string& _uuid )
   {
      std::stringstream ss;
      ss << std::hex;
      for( std::size_t i = 0; i < 8; i++ ) ss << dis( gen );
      ss << "-";
      for( std::size_t i = 0; i < 4; i++ ) ss << dis( gen );
      ss << "-4";
      for( std::size_t i = 0; i < 3; i++ ) ss << dis( gen );
      ss << "-";
      ss << dis2( gen );
      for( std::size_t i = 0; i < 3; i++ ) ss << dis( gen );
      ss << "-";
      for( std::size_t i = 0; i < 12; i++ ) ss << dis( gen );

      _uuid = ss.str( );
   }

} // carpc::tools::uuid
