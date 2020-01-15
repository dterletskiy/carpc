#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <malloc.h>
#include <cstddef>

#include "memory.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "MEMORY"



MemoryMap s_memory_map __attribute__ (( section ("MEMORY"), init_priority (101) )) = { };

namespace memory {
   void dump( ) { s_memory_map.dump( ); }
}



size_t s_header_size = sizeof( size_t );

extern "C" void* __libc_malloc( size_t size );
extern "C" void* malloc( size_t size )
{
   void* address = __libc_malloc( size + s_header_size );
   SYS_SIMPLE_TRC( "address = %p / size = %zu", address, size );
   *((size_t*)address) = size;

   return address + s_header_size;
}

extern "C" void* __libc_memalign( size_t alignment, size_t size );
extern "C" void* memalign( size_t alignment, size_t size )
{
   void* address = __libc_memalign( alignment, size );
   SYS_SIMPLE_TRC( "address = %p / size = %zu / alignment = %zu", address, size, alignment );
   *((size_t*)address) = size;

   return address + s_header_size;
}

extern "C" void* __libc_realloc( void* address, size_t size );
extern "C" void* realloc( void* address, size_t size )
{
   void* new_address = __libc_realloc( address - s_header_size, size );
   SYS_SIMPLE_TRC( "address = %p / size = %zu / new address = %p", (address - s_header_size), size, new_address );
  *((size_t*)address) = size;

   return new_address + s_header_size;
}

extern "C" void* __libc_calloc( size_t number, size_t size );
extern "C" void* calloc( size_t number, size_t size )
{
   void* address = __libc_calloc( number, size );
   SYS_SIMPLE_TRC( "address = %p / size = %zu / number = %zu", address, size, number );
  *((size_t*)address) = number * size;

   return address + s_header_size;
}

extern "C" void* __libc_free( void* address );
extern "C" void free( void* address )
{
   SYS_SIMPLE_TRC( "address = %p / size = %zu", (address - s_header_size), *((size_t*)( address - s_header_size )) );
   __libc_free( address - s_header_size );
}











MemoryMap::MemoryMap( )
{
   // SYS_SIMPLE_TRC( );
}

MemoryMap::~MemoryMap( )
{
   // SYS_SIMPLE_TRC( );
}

bool MemoryMap::insert( void* _caller, void* _address, const size_t _size )
{
   SYS_SIMPLE_TRC( "address = %p / size = %zu", _address, _size );

   if( nullptr == _address )
      return false;

   size_t index = find_free( );
   if( s_invalid_entry == index )
      return false;

   m_mem_map[ index ] = { _caller, _address, _size };
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
         SYS_SIMPLE_INF( "caller: %p / address = %p / size = %zu", m_mem_map[i].caller, m_mem_map[i].address, m_mem_map[i].size );
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





#if 0

typedef void* ( *tp_linux_malloc )( size_t );
static tp_linux_malloc __malloc = nullptr;
void* allocate( const char* function, void* caller, size_t size )
{
   if( nullptr == __malloc )
   {
      SYS_SIMPLE_WRN( "[%s] -> __malloc = nullptr", function );
      __malloc = ( tp_linux_malloc ) dlsym(RTLD_NEXT, "malloc");
   }
   void* address = __malloc( size );
   SYS_SIMPLE_TRC( "[%s] -> caller: %p / address = %p / size = %zu", function, caller, address, size );

   s_memory_map.insert( caller, address, size );
   return address;

}

typedef void ( *tp_linux_free )( void* );
static tp_linux_free __free = nullptr;
void deallocate( const char* function, void* caller, void* address )
{
   if( nullptr == __free )
   {
      SYS_SIMPLE_WRN( "[%s] -> __free = nullptr", function );
      __free = ( tp_linux_free ) dlsym(RTLD_NEXT, "free");
   }
   SYS_SIMPLE_TRC( "[%s] -> caller: %p / address = %p", function, caller, address );
   s_memory_map.remove( address );

   __free( address );
}



void* malloc( size_t size )
{
   // int caller = -1;
   // asm("movl %%esp,%0" : "=r"(caller));
   return allocate( __FUNCTION__, __builtin_return_address(0), size );
}

void free( void* address )
{
   deallocate( __FUNCTION__, __builtin_return_address(0), address );
}

void* operator new( size_t size )
{
   void* address = allocate( __FUNCTION__, __builtin_return_address(0), size );
   if( nullptr == address )
   {
      SYS_SIMPLE_WRN( "bad_alloc" );
      throw std::bad_alloc();
   }

   return address;
}

void operator delete( void* address )
{
   deallocate( __FUNCTION__, __builtin_return_address(0), address );
}

#endif



#if 0

typedef void* ( *tp_linux_malloc )( size_t );
static tp_linux_malloc __malloc = nullptr;
void* malloc( size_t size )
{
   if( nullptr == __malloc )
   {
      SYS_SIMPLE_WRN( " __malloc = nullptr" );
      __malloc = ( tp_linux_malloc ) dlsym(RTLD_NEXT, "malloc");
      if( nullptr == __malloc )
      {
         SYS_SIMPLE_ERR( " __malloc = nullptr" );
         return nullptr;
      }
   }
   void* address = __malloc( size ) ;
   SYS_SIMPLE_TRC( "address = %p / size = %zu", address, size );

   return address;
}

typedef void* ( *tp_linux_calloc )( size_t, size_t );
static tp_linux_calloc __calloc = nullptr;
void* calloc( size_t number, size_t size )
{
   if( nullptr == __calloc )
   {
      SYS_SIMPLE_WRN( " __calloc = nullptr" );
      __calloc = ( tp_linux_calloc ) dlsym(RTLD_NEXT, "calloc");
      char* error = dlerror( );
      if( nullptr == __calloc )
      {
         SYS_SIMPLE_ERR( " __calloc = nullptr / error %s", error );
         return nullptr;
      }
   }
   void* address = __calloc( number, size ) ;
   SYS_SIMPLE_TRC( "address = %p / size = %zu / number = %zu", address, size, number );

   return address;
}

typedef void* ( *tp_linux_realloc )( void*, size_t );
static tp_linux_realloc __realloc = nullptr;
void* realloc( void* address, size_t size )
{
   if( nullptr == __realloc )
   {
      SYS_SIMPLE_WRN( " __realloc = nullptr" );
      __realloc = ( tp_linux_realloc ) dlsym(RTLD_NEXT, "realloc");
      if( nullptr == __realloc )
      {
         SYS_SIMPLE_ERR( " __realloc = nullptr" );
         return nullptr;
      }
   }
   void* new_address = __realloc( address, size );
   SYS_SIMPLE_TRC( "new address = %p / address = %p / size = %zu", new_address, address, size );

   return new_address;
}

typedef void ( *tp_linux_free )( void* );
static tp_linux_free __free = nullptr;
void free( void* address )
{
   if( nullptr == __free )
   {
      SYS_SIMPLE_WRN( "__free = nullptr" );
      __free = ( tp_linux_free ) dlsym(RTLD_NEXT, "free");
      if( nullptr == __free )
      {
         SYS_SIMPLE_ERR( " __free = nullptr" );
         return;
      }
   }
   SYS_SIMPLE_TRC( "address = %p", address );
   __free( address );
}

void* operator new( size_t size )
{
   void* address = malloc( size );
   if( nullptr == address )
   {
      SYS_SIMPLE_WRN( "bad_alloc" );
      throw std::bad_alloc();
   }

   return address;
}

void operator delete( void* address )
{
   free( address );
}

#endif



#if 0

using tp_malloc_hook = void*(*)( size_t, const void* );
using tp_realloc_hook = void*(*)( void*, size_t, const void* );
using tp_memalign_hook = void*(*)( size_t, size_t, const void* );
using tp_free_hook = void(*)( void*, const void* );

namespace glibc {
   tp_malloc_hook    malloc   = nullptr;
   tp_realloc_hook   realloc  = nullptr;
   tp_memalign_hook  memalign = nullptr;
   tp_free_hook      free     = nullptr;
}

namespace memory {
   void* malloc( size_t size, const void* );
   void* memalign( size_t alignment, size_t size, const void* );
   void* realloc( void* address, size_t size, const void* );
   void free( void* address, const void* );
}

void __init__( ) __attribute__(( constructor(1) ));
void __init__( )
{
   glibc::malloc        = __malloc_hook;
   glibc::realloc       = __realloc_hook;
   glibc::memalign      = __memalign_hook;
   glibc::free          = __free_hook;

   __malloc_hook        = memory::malloc;
   __realloc_hook       = memory::realloc;
   __memalign_hook      = memory::memalign;
   __free_hook          = memory::free;
}

namespace memory {

void* malloc( size_t size, const void* )
{
   __malloc_hook = glibc::malloc;
   void* address = ::malloc( size );
   __malloc_hook = memory::malloc;
   SYS_SIMPLE_TRC( "address = %p / size = %zu", address, size );

   return address;
}

void* memalign( size_t alignment, size_t size, const void* )
{
   __memalign_hook = glibc::memalign;
   void* address = ::memalign( alignment, size );
   __memalign_hook = memory::memalign;
   SYS_SIMPLE_TRC( "address = %p / size = %zu / alignment = %zu", address, size, alignment );

   return address;
}

void* realloc( void* address, size_t size, const void* )
{
   __realloc_hook = glibc::realloc;
   void* new_address = ::realloc( address, size );
   __realloc_hook = memory::realloc;
   SYS_SIMPLE_TRC( "address = %p / size = %zu / new address = %p", address, size, new_address );

   return new_address;
}

void free( void* address, const void* )
{
   SYS_SIMPLE_TRC( "address = %p", address );
   __free_hook = glibc::free;
   ::free( address );
   __free_hook = memory::free;
}

}

void* calloc( size_t number, size_t size )
{
   void* address = ::malloc( number * size );
   SYS_SIMPLE_TRC( "address = %p / size = %zu / number = %zu", address, size, number );
   return address;
}

#endif

