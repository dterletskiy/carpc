#include "api/sys/tools/Performance.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Performance"



namespace base::tools {


Performance::Performance( const std::string& name )
   : m_name( name )
{
}

Performance::~Performance( )
{
}

void Performance::start( )
{
   m_start.reset( );
   m_finish.reset( );
   m_delta.reset( );

   m_start = std::chrono::system_clock::now( );
}

void Performance::stop( )
{
   m_finish = std::chrono::system_clock::now( );

   if( std::nullopt == m_start )
   {
      DBG_ERR( "%s: calculation was not started", m_name.c_str( ) );
      m_finish.reset( );
      return;
   }

   m_delta = std::chrono::duration_cast< std::chrono::microseconds >( m_finish.value( ) - m_start.value( ) );
   DBG_INF( "%s: time delta: %ld", m_name.c_str( ), m_delta.value( ).count( ) );
}



} // namespace base::tools
