#pragma once

#include <cstdlib>
#include <cstdint>
#include <string>
#include <map>
#include <optional>
#include <sstream>



namespace carpc::tools {

   class BasePCE
   {
      public:
         using tArgument = std::string;
         using tParameter = std::string;
         using tValue = std::string;
         using tValueOpt = std::optional< std::string >;
         using tMap = std::map< tParameter, tValue >;

      public:
         BasePCE( const std::string& dump_message );
         ~BasePCE( ) = default;

      public:
         tValueOpt value( const tParameter& parameter ) const;
         tValue value_or( const tParameter& parameter, const tValue& default_value ) const;
         template< typename T >
            T value_or( const tParameter& parameter, const T& default_value ) const
            {
               const auto iterator = m_map.find( parameter );
               if( m_map.end( ) == iterator )
                  return default_value;

               std::stringstream ss( iterator->second );
               T value;
               ss >> value;

               return value;
            }

         void print( ) const;

      protected:
         tMap        m_map;
         std::string m_dump_message;
   };

   class Parameters : public BasePCE
   {
      public:
         Parameters( );
         Parameters( int argc, char** argv, const std::string& delimiter = "=" );
         ~Parameters( ) = default;

         void init( int argc, char** argv, const std::string& delimiter = "=" );
   };

   class Environment : public BasePCE
   {
      public:
         Environment( );
         Environment( char** envp, const std::string& delimiter = "=" );
         ~Environment( ) = default;

         void init( char** envp, const std::string& delimiter = "=" );
   };

   class Configuration : public BasePCE
   {
      public:
         Configuration( );
         Configuration( const std::string&, const std::string& delimiter = "=" );
         ~Configuration( ) = default;

         void init( const std::string&, const std::string& delimiter = "=" );
   };

   class PCE : public Parameters, public Environment, public Configuration
   {
      public:
         enum class eType : std::uint8_t { CMD, ENV, CFG, DEFAULT };

      public:
         PCE( ) = default;
         PCE( int argc, char** argv, char** envp, const std::string& delimiter = "=" );

         void init( int argc, char** argv, char** envp, const std::string& delimiter = "=" );

         tValueOpt value( const tParameter& parameter, const eType& type = eType::DEFAULT ) const;
         tValue value_or( const tParameter& parameter, const tValue& default_value, const eType& type = eType::DEFAULT ) const;
         void print( const eType& type = eType::DEFAULT ) const;
   };

} // namespace carpc::tools



namespace carpc::tools::cmd {

   char* get_option( int, char**, const std::string& );
   bool is_option_exists( int , char**, const std::string&  );

} // namespace carpc::tools::cmd
