#include "api/sys/application/IThread.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "ISrv"



using namespace carpc::application;



IThread::IThread( const std::string& name, const size_t wd_timeout )
   : m_name( name )
   , m_wd_timeout( wd_timeout )
{
}
