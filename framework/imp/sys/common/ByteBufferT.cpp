#include "api/sys/common/ByteBufferT.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "B_BUFFER_T"



namespace base {




ByteBufferT::Transaction::Transaction( size_t& _size )
   : m_size( _size )
{ }

bool ByteBufferT::Transaction::start( const eType _type )
{
   if( eType::undefined == _type )
   {
      SYS_ERR( "Undefined transaction type" );
      return false;
   }
   if( eType::undefined != m_type && _type != m_type )
   {
      SYS_ERR( "Another transaction type was already started" );
      return false;
   }

   if( 0 == m_recursive_count )
   {
      m_type = _type;
      m_size_backup = m_size;
   }

   ++m_recursive_count;
   // SYS_TRC( "m_size = %zu / m_recursive_count = %zu", m_size, m_recursive_count );
   return true;
}

bool ByteBufferT::Transaction::finish( )
{
   --m_recursive_count;
   if( 0 == m_recursive_count )
   {
      // SYS_TRC( "Transaction finished" );
      m_type = eType::undefined;
      m_size_backup = 0;
   }

   return true;
}

bool ByteBufferT::Transaction::error( )
{
   --m_recursive_count;
   if( 0 == m_recursive_count )
   {
      SYS_ERR( "Transaction error" );
      m_size = m_size_backup;
      m_size_backup = 0;
      m_type = eType::undefined;
   }

   return false;
}



ByteBufferT::ByteBufferT( )
   : ByteBuffer( )
   , m_transaction( m_size )
{
}

ByteBufferT::ByteBufferT( const void* p_buffer, const size_t size )
   : ByteBuffer( p_buffer, size )
   , m_transaction( m_size )
{
}

ByteBufferT::~ByteBufferT( )
{
   reset( );
}



/*****************************************
 *
 * Push buffer methods
 *
 ****************************************/
bool ByteBufferT::push( const void* p_buffer, const size_t size, const bool is_reallocate )
{
   if( false == m_transaction.start( Transaction::eType::push ) )
      return false;

   if( false == write( p_buffer, size, is_reallocate ) )
      return m_transaction.error( );

   m_size += size;
   return m_transaction.finish( );
}

bool ByteBufferT::push( const void* const p_buffer, const bool is_reallocate )
{
   if( false == m_transaction.start( Transaction::eType::push ) )
      return false;

   if( false == write( &p_buffer, sizeof( void* ), is_reallocate ) )
      return m_transaction.error( );

   m_size += sizeof( void* );
   return m_transaction.finish( );
}

bool ByteBufferT::push( const std::string& string, const bool is_reallocate )
{
   if( false == m_transaction.start( Transaction::eType::push ) )
      return false;

   const char* p_buffer = string.c_str( );
   const size_t size = string.size( );

   // Backup buffer state.
   size_t size_backup = m_size;

   // It should be possible to store next amount of bytes: content size + size of content.
   if( ( m_capacity - m_size ) < ( size + sizeof( size ) ) && false == is_reallocate )
      return m_transaction.error( );

   // Storing string content
   if( false == push( static_cast< const void* >( p_buffer ), size, is_reallocate ) )
      return m_transaction.error( );

   // Storing size of string. In case of error prevoius buffer state will be restored.
   // In this case stored string content will be deleted.
   if( false == push( size, is_reallocate ) )
   {
      m_size = size_backup;
      return m_transaction.error( );
   }

   return m_transaction.finish( );
}



/*****************************************
 *
 * Pop buffer methods
 *
 ****************************************/
bool ByteBufferT::pop( const void* p_buffer, const size_t size )
{
   if( false == m_transaction.start( Transaction::eType::pop ) )
      return false;

   if( false == read( p_buffer, size ) )
      return m_transaction.error( );

   m_size -= size;
   return m_transaction.finish( );
}

bool ByteBufferT::pop( const void*& p_buffer )
{
   if( false == m_transaction.start( Transaction::eType::pop ) )
      return false;

   if( false == read( &p_buffer, sizeof( void* ) ) )
      return m_transaction.error( );

   m_size -= sizeof( void* );
   return m_transaction.finish( );
}

bool ByteBufferT::pop( std::string& string )
{
   if( false == m_transaction.start( Transaction::eType::pop ) )
      return false;

   size_t size_string = 0;
   // Reading size of string content
   if( false == pop( size_string ) )
      return m_transaction.error( );

   // Error in case of rest of data in buffer less then size to be read.
   // push previously poped data to restore previous buffer state.
   if( size_string > m_size )
      return m_transaction.error( );

   char p_buffer[ size_string + 1 ]; // +1 for termitating '\0'
   if( false == pop( static_cast< void* >( p_buffer ), size_string ) )
      return m_transaction.error( );

   // Adding termitating '\0'
   p_buffer[ size_string + 1 - 1 ] = 0;

   string = p_buffer;
   return m_transaction.finish( );
}




} // namespace base
