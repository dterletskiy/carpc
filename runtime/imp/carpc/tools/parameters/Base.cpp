#include <fstream>
#include <filesystem>
#include <algorithm>

#include "carpc/helpers/functions/format.hpp"
#include "carpc/tools/parameters/Base.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "PARAMS"



using namespace carpc::tools::parameters;



Base::Base( const tMap& map, const std::string& dump_message )
   : m_map( map )
   , m_dump_message( dump_message)
{
}

bool Base::is_exist( const tParameter& parameter ) const
{
   return m_map.end( ) != m_map.find( parameter );
}

std::pair< tValueOpt, bool > Base::value( const tParameter& parameter ) const
{
   const auto iterator = m_map.find( parameter );
   if( m_map.end( ) == iterator )
      return std::make_pair( invalid_value, false );

   return std::make_pair( iterator->second, true );
}

std::pair< tValue, bool > Base::value_or( const tParameter& parameter, const tValue& default_value ) const
{
   const auto iterator = m_map.find( parameter );
   if( m_map.end( ) == iterator )
      return std::make_pair( default_value, false );

   if( invalid_value == iterator->second )
      return std::make_pair( default_value, true );

   return std::make_pair( iterator->second.value( ), true );
}

void Base::print( ) const
{
   const char* line = "----------------------------------------------";
   MSG_WRN( "%s BEGIN: %s %s", line, m_dump_message.c_str( ), line );
   for( auto pair : m_map )
   {
      if( invalid_value == pair.second )
         MSG_VRB( "   '%s'", pair.first.c_str( ) );
      else
         MSG_VRB( "   '%s' = '%s'", pair.first.c_str( ), pair.second.value( ).c_str( ) );
   }
   MSG_WRN( "%s  END:  %s %s", line, m_dump_message.c_str( ), line );
}
