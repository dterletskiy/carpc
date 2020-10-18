#include "api/sys/helpers/functions/format.hpp"
#include "api/sys/application/Process.hpp"
#include "api/sys/application/IThread.hpp"
#include "api/sys/application/Context.hpp"



using namespace base::application;



const Context::process::tID Context::process::local = Context::process::tID::invalid( ) - Context::process::tID::VALUE_TYPE( 1 );

const Context::thread::tID Context::thread::broadcast = Context::thread::tID::invalid( );
const Context::thread::tID Context::thread::local = Context::thread::broadcast - Context::thread::tID::VALUE_TYPE( 1 );



Context::Context( const eInitType init_type )
{
   if( eInitType::Auto == init_type )
   {
      m_pid = Process::instance( )->id( );
      IThread::tSptr thread = Process::instance( )->current_thread( );
      if( nullptr != thread )
         m_tid = thread->id( );
   }
}

Context::Context( const process::tID& pid )
   : m_pid( pid )
{
}

Context::Context( const thread::tID& tid )
   : m_tid( tid )
{
}

Context::Context( const process::tID& pid, const thread::tID& tid )
   : m_pid( pid )
   , m_tid( tid )
{
}

Context::Context( const Context& other )
   : m_pid( other.m_pid )
   , m_tid( other.m_tid )
{
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

bool Context::to_stream( base::dsi::tByteStream& stream ) const
{
   return stream.push( m_pid, m_tid );
}

bool Context::from_stream( base::dsi::tByteStream& stream )
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
