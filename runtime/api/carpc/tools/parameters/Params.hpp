#pragma once

#include <vector>

#include "carpc/tools/parameters/CmdLine.hpp"
#include "carpc/tools/parameters/Env.hpp"
#include "carpc/tools/parameters/Config.hpp"



namespace carpc::tools::parameters {

   class Params
   {
      public:
         using tSptr = std::shared_ptr< Base >;

      public:
         Params(
               int argc, char** argv, char** envp,
               const tMap& default_parameters = { }
            );
         ~Params( ) = default;

      public:
         bool is_exist( const tParameter& parameter ) const;

         std::pair< tValueOpt, bool > value( const tParameter& parameter ) const;
         template< typename T >
            std::pair< std::optional< T >, bool > value( const tParameter& parameter ) const;

         // Returns a pair:
         //    - if parameter exists will return found value and true.
         //    - if parameters does not exist will return 'default_value' and true.
         std::pair< tValue, bool > value_or( const tParameter& parameter, const tValue& default_value ) const;
         template< typename T >
            std::pair< T, bool > value_or( const tParameter& parameter, const T& default_value ) const;

         void print( ) const;

      protected:
         CmdLine::tSptr    mp_cmdline = nullptr;
         Env::tSptr        mp_env = nullptr;
         Config::tSptr     mp_config = nullptr;
         std::vector< Base::tSptr > m_params;
   };



   template< typename T >
   std::pair< std::optional< T >, bool > Params::value( const tParameter& parameter ) const
   {
      for( auto item: m_params )
      {
         if( nullptr == item )
            continue;

         auto value = item->value< T >( parameter );
         if( value.second )
            return value;
      }

      return std::make_pair( invalid_value, false );
   }

   template< typename T >
   std::pair< T, bool > Params::value_or( const tParameter& parameter, const T& default_value ) const
   {
      for( auto item: m_params )
      {
         if( nullptr == item )
            continue;

         auto value = item->value_or< T >( parameter, default_value );
         if( value.second )
            return value;
      }

      return std::make_pair( default_value, false );
   }

} // namespace carpc::tools::parameters
