#include "api/sys/service/IServiceThread.hpp"

#include "api/sys/tools/Tools.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "ISrv"



using namespace base;



IServiceThread::IServiceThread( const std::string& name, const size_t wd_timeout )
   : m_id( tools::id::generate( "srv_thread" ) )
   , m_name( name )
   , m_wd_timeout( wd_timeout )
{
}
