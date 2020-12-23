#include "api/sys/application/IThread.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "ISrv"



using namespace base::application;



IThread::IThread( const std::string& name, const size_t wd_timeout )
   : m_name( name )
   , m_wd_timeout( wd_timeout )
{
}
