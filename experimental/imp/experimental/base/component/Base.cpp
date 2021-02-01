#include "imp/experimental/base/component/Base.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "BaseComponent"



using namespace fw::component;



Base::Base( const std::string& name )
   : m_name( name )
{
   MSG_DBG( "%s: created", m_name.c_str( ) );
}

Base::~Base( )
{
   MSG_DBG( "%s: destroyed", m_name.c_str( ) );
}

const std::string& Base::name( ) const
{
   return m_name;
}

