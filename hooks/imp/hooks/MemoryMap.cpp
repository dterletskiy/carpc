#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <cstddef>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "MemoryMap.hpp"

#include "api/sys/oswrappers/linux/kernel.hpp"
#include "api/sys/oswrappers/linux/time.hpp"
#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "MEMORY"



const mode_t AccessPerms = (mode_t)0600;



namespace hook::memory {



MemoryHeader* MemoryHeader::cast( void* p )
{
   return reinterpret_cast< MemoryHeader* >( p );
}

void MemoryHeader::dump( void* _address )
{
   MemoryHeader* p_header = cast( _address );
   SYS_SIMPLE_INF( "address (user address) = %p (%p) / caller = %p / size = %zu / time = %lu", _address, _address + sizeof( MemoryHeader ), p_header->caller, p_header->size, p_header->time );
}

void MemoryHeader::write( void* _address, void* _caller, const size_t _size, const uint64_t _time )
{
   *cast( _address ) = { _caller, _size, _time };
}



MemoryMap::MemoryMap( const char* _path, const size_t _size )
   : m_path( _path )
   , m_min_track_size( _size )
{
   if( nullptr == strchr( m_path, '/' ) )
      m_fd = shm_open( m_path, O_RDWR | O_CREAT, AccessPerms );
   else
      m_fd = open( m_path, O_RDWR | O_CREAT, AccessPerms );

   if( 0 > m_fd )
   {
      m_last_errno = errno;
      SYS_SIMPLE_ERR( "open file error: %d", m_last_errno );
   }
   else
   {
      SYS_SIMPLE_TRC( "log file opened: %s(%d)", m_path, m_fd );
      int result = lseek( m_fd, 0, SEEK_END );
      if( -1 == result )
      {
         m_last_errno = errno;
         SYS_SIMPLE_ERR( "lseek file error: %d", m_last_errno );
      }
      else
      {
         SYS_SIMPLE_TRC( "descriptor file positioned: %d(%d)", m_fd, result );
      }
   }
}

MemoryMap::~MemoryMap( )
{
   SYS_SIMPLE_TRC( );
   close( m_fd );
}

void MemoryMap::set_track_size( const size_t _size )
{
   m_min_track_size = _size;
}

bool MemoryMap::insert( void* _address, void* _caller, size_t _size )
{
   // SYS_SIMPLE_TRC( "address = %p", _address );

   if( nullptr == _address )
      return false;

   uint64_t time_stamp = base::os::linux::time( base::os::linux::eGranularity::miliseconds );
   if( 0 != m_min_track_size && m_min_track_size < _size )
   {
      char buffer[ 256 ];
      ::write( m_fd, buffer, sprintf( buffer, "\ntime: %lu / caller: %p / address: %p / size: %zu\n", time_stamp, _caller, _address, _size ) );
      base::os::linux::back_trace( m_fd );
   }

   tHeaderType::write( _address, _caller, _size, time_stamp );

   size_t index = find_free( );
   if( s_invalid_entry == index )
      return false;

   m_mem_map[ index ] = { _address };
   return true;
}

bool MemoryMap::remove( const void* _address )
{
   // SYS_SIMPLE_TRC( "address = %p", _address );

   if( nullptr == _address )
      return false;

   for( size_t i = 0; i < s_memory_map_size; ++i )
      if( _address == m_mem_map[i].address )
      {
         m_mem_map[i] = { };
         return true;
      }

   // SYS_SIMPLE_WRN( "error address = %p", _address );
   return false;
}

size_t MemoryMap::find_free( ) const
{
   // SYS_SIMPLE_TRC( );

   for( size_t i = 0; i < s_memory_map_size; ++i )
      if( nullptr == m_mem_map[i].address )
         return i;

   return s_invalid_entry;
}

void MemoryMap::dump( ) const
{
   SYS_SIMPLE_INF( "--------------- Start Dump ---------------" );

   for( size_t i = 0; i < s_memory_map_size; ++i )
   {
      if( nullptr != m_mem_map[i].address )
         tHeaderType::dump( m_mem_map[i].address );
   }

   SYS_SIMPLE_INF( "---------------- End Dump ----------------" );
}

const size_t MemoryMap::records( ) const
{
   size_t records = 0;
   for( size_t i = 0; i < s_memory_map_size; ++i )
      if( nullptr != m_mem_map[i].address )
         ++records;

   return records;
}



} // namespace hook::memory
