#include "api/sys/tools/Tools.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TOOLS"



#include <filesystem>
namespace base::tools {

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
      // std::string file = std::filesystem::path( *argv ).filename( );
      // file += std::string( ".cfg" );

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

         if( true == line.empty( ) )
            continue;

         size_t position = line.find( delimiter );
         if( std::string::npos == position )
            m_map.emplace( line, std::string("") );
         else
            m_map.emplace( line.substr( 0, position ), line.substr( position + 1 ) );
      }

      file_stream.close( );
   }



   PCE::PCE( int argc, char** argv, char** envp, const std::string& delimiter )
      : Parameters( argc, argv, delimiter )
      , Environment( envp, delimiter )
      , Configuration( )
   {
      std::string file_name = std::filesystem::path( *argv ).filename( );
      file_name += std::string( ".cfg" );
      file_name = Parameters::value( "config" ).value_or( file_name );

      Configuration::init( file_name, delimiter );
   }

   void PCE::init( int argc, char** argv, char** envp, const std::string& delimiter )
   {
      Parameters::init( argc, argv, delimiter );
      Environment::init( envp, delimiter );

      std::string file_name = std::filesystem::path( *argv ).filename( );
      file_name += std::string( ".cfg" );
      file_name = Parameters::value( "config" ).value_or( file_name );

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

} // namespace base::tools



namespace base::tools::cmd {

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

} // namespace base::tools::cmd



#include <random>
#include <sstream>
// #include "uuid/uuid.h"

// link: https://stackoverflow.com/a/60198074/12978480
namespace base::tools::uuid {

   static std::random_device                 rd;
   static std::mt19937                       gen( rd( ) );
   static std::uniform_int_distribution< >   dis( 0, 15 );
   static std::uniform_int_distribution< >   dis2( 8, 11 );

   void generate_random( std::string& _uuid )
   {
      std::stringstream ss;
      ss << std::hex;
      for( size_t i = 0; i < 8; i++ ) ss << dis( gen );
      ss << "-";
      for( size_t i = 0; i < 4; i++ ) ss << dis( gen );
      ss << "-4";
      for( size_t i = 0; i < 3; i++ ) ss << dis( gen );
      ss << "-";
      ss << dis2( gen );
      for( size_t i = 0; i < 3; i++ ) ss << dis( gen );
      ss << "-";
      for( size_t i = 0; i < 12; i++ ) ss << dis( gen );

      _uuid = ss.str( );
   }

} // base::tools::uuid
