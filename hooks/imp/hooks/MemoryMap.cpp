#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <cstddef>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "api/sys/helpers/functions/print.hpp"
#include "api/sys/oswrappers/linux/backtrace.hpp"
#include "api/sys/oswrappers/linux/time.hpp"
#include "imp/hooks/MemoryMap.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "MEMORY"



// #define DEBUG
#ifdef DEBUG
   #define MESSAGE( FORMAT, ... ) carpc::write( FORMAT, ##__VA_ARGS__ );
#else
   #define MESSAGE( FORMAT, ... )
#endif



extern "C" void* malloc_u( size_t size );
extern "C" void free_u( void* address );



const mode_t AccessPerms = (mode_t)0600;



using namespace hook::memory;



MemoryHeader* MemoryHeader::cast( void* p )
{
   return reinterpret_cast< MemoryHeader* >( p );
}

void MemoryHeader::dump( void* _address, const int fd, const char* const message )
{
   MemoryHeader* p_header = cast( _address );

   if( 0 < fd )
   {
      carpc::write(
           fd
         , "%s: address (user address) = %p (%p) / caller = %p / size = %zu / time = %lu / thread = %#lx\n"
         , message
         , _address, static_cast< char* >( _address ) + sizeof( MemoryHeader )
         , p_header->caller
         , p_header->size
         , p_header->time
         , p_header->thread
      );
   }
   else
   {
      SYS_INF(
         "%s: address (user address) = %p (%p) / caller = %p / size = %zu / time = %lu / thread = %#lx"
         , message
         , _address, static_cast< char* >( _address ) + sizeof( MemoryHeader )
         , p_header->caller
         , p_header->size
         , p_header->time
         , p_header->thread
      );
   }
}

void MemoryHeader::write( void* _address, void* _caller, const std::size_t _size, const std::uint64_t _time, const pthread_t _thread )
{
   *cast( _address ) = { _caller, _size, _time, _thread };
}



MemoryMap::MemoryMap( const char* _path, const std::size_t _size )
   : m_min_track_size( _size )
   , m_path( _path )
{
   // m_mem_map = (tEntry*) malloc_u( s_memory_map_size * sizeof( tEntry ) );
   init( );
}

MemoryMap::~MemoryMap( )
{
   // free_u( m_mem_map );
   deinit( );
}

void MemoryMap::init( )
{
   if( nullptr == strchr( m_path, '/' ) )
      m_fd = shm_open( m_path, O_RDWR | O_CREAT, AccessPerms );
   else
      m_fd = open( m_path, O_RDWR | O_CREAT, AccessPerms );

   if( 0 > m_fd )
   {
      m_last_errno = errno;
      SYS_ERR( "open file error: %d", m_last_errno );
   }
   else
   {
      SYS_VRB( "log file opened: %s(%d)", m_path, m_fd );
      int result = lseek( m_fd, 0, SEEK_END );
      if( -1 == result )
      {
         m_last_errno = errno;
         SYS_ERR( "lseek file error: %d", m_last_errno );
      }
      else
      {
         SYS_VRB( "descriptor file positioned: %d(%d)", m_fd, result );
      }
   }
}

void MemoryMap::deinit( )
{
   SYS_VRB( );
   close( m_fd );
}

void MemoryMap::set_track_file( const char* const _path )
{
   deinit( );
   m_path = _path;
   init( );
}

void MemoryMap::set_track_size( const std::size_t _size )
{
   m_min_track_size = _size;
}

bool MemoryMap::insert( void* _address, void* _caller, std::size_t _size )
{
   SYS_VRB( "address = %p", _address );

   if( nullptr == _address )
      return false;

   std::uint64_t time_stamp = carpc::os::os_linux::time( carpc::os::os_linux::eGranularity::milliseconds );
   tHeaderType::write( _address, _caller, _size, time_stamp, pthread_self( ) );
   m_common_size += _size;

   if( 0 != m_min_track_size && m_min_track_size < _size )
   {
      tHeaderType::dump( _address, m_fd, "\n" );
      carpc::os::os_linux::back_trace( m_fd );
   }

   std::size_t index = find_free( );
   if( s_invalid_entry == index )
      return false;

   ++m_records;
   m_mem_map[ index ] = { _address };
   return true;
}

bool MemoryMap::remove( const void* _address )
{
   SYS_VRB( "address = %p", _address );

   if( nullptr == _address )
      return false;

   for( std::size_t i = 0; i < s_memory_map_size; ++i )
   {
      if( _address == m_mem_map[i].address )
      {
         m_common_size -= tHeaderType::cast( m_mem_map[i].address )->size;
         --m_records;
         m_mem_map[i] = { };
         return true;
      }
   }

   SYS_WRN( "error address = %p", _address );
   return false;
}

std::size_t MemoryMap::find_free( ) const
{
   if( m_records == s_memory_map_size )
      return s_invalid_entry;

   for( std::size_t i = 0; i < s_memory_map_size; ++i )
      if( nullptr == m_mem_map[i].address )
         return i;

   return s_invalid_entry;
}

void MemoryMap::dump( ) const
{
   SYS_DUMP_START( "HEAP" );

   SYS_INF( "Allocated heap size: %zu bytes", m_common_size );
   SYS_INF( "Unreleased allocations: %zu", m_records );
   for( std::size_t i = 0; i < s_memory_map_size; ++i )
   {
      if( nullptr != m_mem_map[i].address )
         tHeaderType::dump( m_mem_map[i].address );
   }

   SYS_DUMP_END( "HEAP" );
}

const std::size_t MemoryMap::records( ) const
{
   return m_records;
}
