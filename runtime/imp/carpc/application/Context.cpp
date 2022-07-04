#include "carpc/helpers/functions/format.hpp"
#include "carpc/application/Process.hpp"
#include "carpc/application/IThread.hpp"
#include "carpc/application/Context.hpp"



using namespace carpc::application;



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
         m_pid = process::current_id( );
         m_tid = thread::current_id( );
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

Context::Context( const thread::ID& tid, const process::ID& pid )
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

bool Context::is_internal( ) const
{
   return ( process::current_id( ) == m_pid ) || ( process::local == m_pid );
}

bool Context::is_internal_local( ) const
{
   if( false == is_internal( ) )
      return false;

   return ( thread::current_id( ) == m_tid ) || ( thread::local == m_tid );
}

bool Context::is_internal_broadcast( ) const
{
   return !is_internal_local( );
}

bool Context::is_external( ) const
{
   return !is_internal( );
}

bool Context::is_valid( ) const
{
   return m_pid.is_valid( ) && m_tid.is_valid( );
}

bool Context::to_stream( carpc::ipc::tStream& stream ) const
{
   return ipc::serialize( stream, m_pid, m_tid );
}

bool Context::from_stream( carpc::ipc::tStream& stream )
{
   return ipc::deserialize( stream, m_pid, m_tid );
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

const std::string Context::dbg_name( ) const
{
   return carpc::format_string( m_pid.dbg_name( ), ".", m_tid.dbg_name( ) );
}
