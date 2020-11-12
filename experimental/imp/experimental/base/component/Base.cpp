#include "imp/experimental/base/component/Base.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "BaseComponent"



using namespace fw::component;



Base::Base( const std::string& name )
   : m_name( name )
{
   DBG_MSG( "%s: created", m_name.c_str( ) );
}

Base::~Base( )
{
   DBG_MSG( "%s: destroyed", m_name.c_str( ) );
}

const std::string& Base::name( ) const
{
   return m_name;
}

