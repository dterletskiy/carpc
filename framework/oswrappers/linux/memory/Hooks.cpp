#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <cstddef>

#include "Hooks.hpp"

#include "Trace.hpp"
#define CLASS_ABBR "MEMORY"



#ifdef HOOK_MEMORY_ALLOCATOR



MemoryMap s_memory_map __attribute__ (( section ("MEMORY"), init_priority (101) )) = { };



namespace memory {

   void dump( ) { s_memory_map.dump( ); }

}



typedef void* ( *tp_linux_malloc )( size_t );
static tp_linux_malloc __malloc = nullptr;
void* malloc( size_t size )
{
   if( nullptr == __malloc )
   {
      SYS_SIMPLE_WRN( "__malloc = nullptr" );
      __malloc = ( tp_linux_malloc ) dlsym(RTLD_NEXT, "malloc");
   }
   void* p = __malloc(size);
   SYS_SIMPLE_TRC( "address = %p / size = %zu", p, size );

   s_memory_map.insert( p, size );
   return p;
}

typedef void ( *tp_linux_free )( void* );
static tp_linux_free __free = nullptr;
void free( void* p )
{
   if( nullptr == __free )
   {
      SYS_SIMPLE_WRN( "__free = nullptr" );
      __free = ( tp_linux_free ) dlsym(RTLD_NEXT, "free");
   }
   SYS_SIMPLE_TRC( "address = %p", p );
   s_memory_map.remove( p );

   __free( p );
}

void* operator new( size_t size )
{
   void* p = malloc( size );
   if( nullptr == p )
   {
      SYS_SIMPLE_WRN( "bad_alloc" );
      throw std::bad_alloc();
   }

   SYS_SIMPLE_TRC( "address = %p / size = %zu", p, size );
   return p;
}

void operator delete( void* p )
{
   free( p );
   SYS_SIMPLE_TRC( "address = %p", p );
}



MemoryMap::MemoryMap( )
{
   // SYS_SIMPLE_TRC( );
}

MemoryMap::~MemoryMap( )
{
   // SYS_SIMPLE_TRC( );
}

bool MemoryMap::insert( void* _address, const size_t _size )
{
   // SYS_SIMPLE_TRC( "address = %p / size = %zu", _address, _size );

   if( nullptr == _address )
      return false;

   size_t index = find_free( );
   if( s_invalid_entry == index )
      return false;

   m_mem_map[ index ] = { _address, _size };
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
         SYS_SIMPLE_INF( "address = %p / size = %zu", m_mem_map[i].address, m_mem_map[i].size );
      }

   SYS_SIMPLE_INF( "---------------- End Dump ----------------" );
}



#else



namespace memory {

   void dump( ) { }

}



#endif
