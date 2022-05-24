#include "carpc/tools/Performance.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "Performance"



namespace carpc::tools {


Performance::Performance( const std::string& name )
   : m_name( name )
{
}

Performance::~Performance( )
{
}

void Performance::start( const std::string& message )
{
   if( false == message.empty( ) )
   {
      MSG_VRB( "%s", message.c_str( ) );
   }

   m_start.reset( );
   m_finish.reset( );
   m_delta.reset( );

   m_start = std::chrono::system_clock::now( );
}

void Performance::stop( const std::string& message )
{
   m_finish = std::chrono::system_clock::now( );

   if( std::nullopt == m_start )
   {
      SYS_ERR( "%s: calculation was not started", m_name.c_str( ) );
      m_finish.reset( );
      return;
   }

   m_delta = std::chrono::duration_cast< std::chrono::microseconds >( m_finish.value( ) - m_start.value( ) );
   m_start.reset( );
   m_finish.reset( );
   if( false == message.empty( ) )
   {
      MSG_VRB( "%s", message.c_str( ) );
   }
   static_cast< void >( info( ) );
}

long int Performance::info( ) const
{
   if( std::nullopt == m_delta )
      return 0;

   MSG_VRB( "%s: time delta: %ld microseconds", m_name.c_str( ), m_delta.value( ).count( ) );
   return m_delta.value( ).count( );
}



} // namespace carpc::tools
