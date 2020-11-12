#include "imp/experimental/base/application/Context.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Context"



using namespace fw::application;



Context::Context( const std::string& name )
   : m_name( name )
   , m_service( )
   , m_work( m_service )
{
}

Context::~Context( )
{
   stop( );
}

const std::string& Context::name( ) const
{
   return m_name;
}

void Context::run( )
{
   m_service.run( );
}

void Context::stop( )
{
   m_service.stop( );
}

void Context::post( tFunction function )
{
   m_service.post( function );
}
