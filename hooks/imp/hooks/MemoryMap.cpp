#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <malloc.h>
#include <cstddef>

#include "MemoryMap.hpp"

#include "api/sys/oswrappers/linux/time.hpp"
#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "MEMORY"



MemoryMap::MemoryMap( )
{
   SYS_SIMPLE_TRC( );
}

MemoryMap::~MemoryMap( )
{
   SYS_SIMPLE_TRC( );
}

bool MemoryMap::insert( void* _address )
{
   SYS_SIMPLE_TRC( "address = %p", _address );

   if( nullptr == _address )
      return false;

   size_t index = find_free( );
   if( s_invalid_entry == index )
      return false;

   m_mem_map[ index ] = { _address };
   SYS_SIMPLE_TRC( "records: %ld", records( ) );
   return true;
}

bool MemoryMap::remove( const void* _address )
{
   SYS_SIMPLE_TRC( "address = %p", _address );

   if( nullptr == _address )
      return false;

   for( size_t i = 0; i < s_memory_map_size; ++i )
      if( _address == m_mem_map[i].address )
      {
         m_mem_map[i] = { };
         SYS_SIMPLE_TRC( "records: %ld", records( ) );
         return true;
      }

   SYS_SIMPLE_WRN( "error address = %p", _address );
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
      if( nullptr != m_mem_map[i].address )
      {
         SYS_SIMPLE_INF( "caddress = %p", m_mem_map[i].address );
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
