#include "api/sys/helpers/functions/format.hpp"
#include "api/sys/application/Process.hpp"
#include "api/sys/application/IThread.hpp"
#include "api/sys/application/Context.hpp"



using namespace base::application;



const Context Context::invalid{ Context::eInitType::INVALID };
const Context Context::internal_broadcast{ Context::eInitType::INTERNAL_BROADCAST };
const Context Context::internal_local{ Context::eInitType::INTERNAL_LOCAL };



Context::Context( const eInitType init_type )
{
   switch( init_type )
   {
      case eInitType::INTERNAL_BROADCAST:
      {
         m_pid = process::local;
         m_tid = thread::broadcast;
         break;
      }
      case eInitType::INTERNAL_LOCAL:
      {
         m_pid = process::local;
         m_tid = thread::local;
         break;
      }
      case eInitType::CURRENT:
      {
         m_pid = Process::instance( )->id( );
         IThread::tSptr thread = Process::instance( )->current_thread( );
         if( nullptr != thread )
            m_tid = thread->id( );
         break;
      }
      case eInitType::INVALID:
      default:
      {
         m_pid = process::ID::invalid;
         m_tid = thread::ID::invalid;
         break;
      }
   }
}

Context::Context( const process::ID& pid, const thread::ID& tid )
   : m_pid( pid )
   , m_tid( tid )
{
}

Context::Context( const Context& other )
   : m_pid( other.m_pid )
   , m_tid( other.m_tid )
{
}

Context Context::current( )
{
   return Context{ eInitType::CURRENT };
}

bool Context::is_local( ) const
{
   return ( application::Process::instance( )->id( ) == m_pid ) || ( process::local == m_pid );
}

bool Context::is_external( ) const
{
   return !is_local( );
}

bool Context::is_valid( ) const
{
   return m_pid.is_valid( ) && m_tid.is_valid( );
}

bool Context::to_stream( base::ipc::tStream& stream ) const
{
   return stream.push( m_pid, m_tid );
}

bool Context::from_stream( base::ipc::tStream& stream )
{
   return stream.pop( m_pid, m_tid );
}

Context& Context::operator=( const Context& other )
{
   m_pid = other.m_pid;
   m_tid = other.m_tid;
   return *this;
}

bool Context::operator==( const Context& other ) const
{
   return ( m_pid == other.m_pid ) && ( m_tid == other.m_tid );
}

bool Context::operator!=( const Context& other ) const
{
   return !( *this == other );
}

bool Context::operator<( const Context& other ) const
{
   if( m_pid != other.m_pid )
      return m_pid < other.m_pid;

   return m_tid < other.m_tid;
}

const std::string Context::name( ) const
{
   return base::format_string( m_pid.name( ), ".", m_tid.name( ) );
}
