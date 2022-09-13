#include "carpc/application/IThread.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "ISrv"



using namespace carpc::application;



IThread::IThread( const std::string& name, const std::size_t wd_timeout )
   : m_name( name )
   , m_wd_timeout( wd_timeout )
{
}
