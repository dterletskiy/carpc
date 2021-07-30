#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <dlfcn.h>
#include <malloc.h>
#include <cstddef>

#include "api/sys/helpers/functions/print.hpp"
#include "api/sys/oswrappers/Mutex.hpp"
#include "imp/hooks/MemoryMap.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "MEMORY"



#define DEBUG
#ifdef DEBUG
   #define MESSAGE( FORMAT, ... ) base::write( FORMAT, ##__VA_ARGS__ );
#else
   #define MESSAGE( FORMAT, ... )
#endif



namespace {

   hook::memory::MemoryMap s_memory_map __attribute__ (( section ("MEMORY"), init_priority (101) )) = { "/tmp/backtrace", 1024 * 1024 };
   base::os::Mutex s_mutex; 

}

namespace memory {

   void dump( )                              { s_memory_map.dump( ); }
   void set_track_size( const size_t size )  { s_memory_map.set_track_size( size ); }
   void set_track_file( const char* path )   { s_memory_map.set_track_file( path ); }

}



extern "C" void* __libc_malloc( size_t size );
extern "C" void* malloc( size_t size )
{
   if( 0 == size )
      return nullptr;

   base::os::Mutex::AutoLocker locker( s_mutex );
   void* address = __libc_malloc( size + hook::memory::s_header_size );
   s_memory_map.insert( address, __builtin_return_address(0), size );

   SYS_VRB( "address = %p / size = %zu", (static_cast< char* >( address ) + hook::memory::s_header_size), size );
   return static_cast< void* >( static_cast< char* >( address ) + hook::memory::s_header_size );
}
extern "C" void* malloc_u( size_t size )
{
   return __libc_malloc( size );
}

extern "C" void* __libc_memalign( size_t alignment, size_t size );
extern "C" void* memalign( size_t alignment, size_t size )
{
   if( 0 == size )
      return nullptr;

   base::os::Mutex::AutoLocker locker( s_mutex );
   void* address = __libc_memalign( alignment, size );
   s_memory_map.insert( address, __builtin_return_address(0), size );

   SYS_VRB( "address = %p / size = %zu / alignment = %zu", (static_cast< char* >( address ) + hook::memory::s_header_size), size, alignment );
   return static_cast< void* >( static_cast< char* >( address ) + hook::memory::s_header_size );
}
extern "C" void* memalign_u( size_t alignment, size_t size )
{
   return __libc_memalign( alignment, size );   
}

extern "C" void* __libc_realloc( void* address, size_t size );
extern "C" void* realloc( void* address, size_t size )
{
   if( 0 == size )
      return nullptr;

   base::os::Mutex::AutoLocker locker( s_mutex );
   void* new_address = __libc_realloc(
        address ? static_cast< void* >(static_cast< char* >( address ) - hook::memory::s_header_size) : address
      , (size + hook::memory::s_header_size)
   );
   s_memory_map.remove( static_cast< char* >( address ) - hook::memory::s_header_size );
   s_memory_map.insert( new_address, __builtin_return_address(0), size );

   SYS_VRB( "address = %p / size = %zu / new address = %p", address, size, (static_cast< char* >( new_address ) + hook::memory::s_header_size) );
   return static_cast< void* >( static_cast< char* >( new_address ) + hook::memory::s_header_size );
}
extern "C" void* realloc_u( void* address, size_t size )
{
   return __libc_realloc( address, size );
}

extern "C" void* __libc_calloc( size_t number, size_t size );
extern "C" void* calloc( size_t number, size_t size )
{
   if( 0 == size || 0 == number )
      return nullptr;

   base::os::Mutex::AutoLocker locker( s_mutex );
   // void* address = __libc_calloc( number, size );
   void* address = __libc_malloc( number * size + hook::memory::s_header_size );
   memset( address, 0, number * size + hook::memory::s_header_size );
   s_memory_map.insert( address, __builtin_return_address(0), number * size );

   SYS_VRB( "address = %p / size = %zu / number = %zu", (static_cast< char* >( address ) + hook::memory::s_header_size), size, number );
   return static_cast< void* >( static_cast< char* >( address ) + hook::memory::s_header_size );
}
extern "C" void* calloc_u( size_t number, size_t size )
{
   return __libc_calloc( number, size );
}

extern "C" void* __libc_free( void* address );
extern "C" void free( void* address )
{
   if( nullptr == address )
      return;

   base::os::Mutex::AutoLocker locker( s_mutex );
   SYS_VRB( "address = %p", address );
   s_memory_map.remove( static_cast< char* >( address ) - hook::memory::s_header_size );
   __libc_free( static_cast< char* >( address ) - hook::memory::s_header_size );
}
extern "C" void free_u( void* address )
{
   __libc_free( address );
}











#if 0 // hook using dlsym(...)

typedef void* ( *tp_linux_malloc )( size_t );
static tp_linux_malloc __malloc = nullptr;
void* malloc( size_t size )
{
   if( nullptr == __malloc )
   {
      SYS_WRN( " __malloc = nullptr" );
      __malloc = ( tp_linux_malloc ) dlsym(RTLD_NEXT, "malloc");
      if( nullptr == __malloc )
      {
         SYS_ERR( " __malloc = nullptr" );
         return nullptr;
      }
   }
   void* address = __malloc( size ) ;
   SYS_VRB( "address = %p / size = %zu", address, size );

   return address;
}

typedef void* ( *tp_linux_calloc )( size_t, size_t );
static tp_linux_calloc __calloc = nullptr;
void* calloc( size_t number, size_t size )
{
   if( nullptr == __calloc )
   {
      SYS_WRN( " __calloc = nullptr" );
      __calloc = ( tp_linux_calloc ) dlsym(RTLD_NEXT, "calloc");
      char* error = dlerror( );
      if( nullptr == __calloc )
      {
         SYS_ERR( " __calloc = nullptr / error %s", error );
         return nullptr;
      }
   }
   void* address = __calloc( number, size ) ;
   SYS_VRB( "address = %p / size = %zu / number = %zu", address, size, number );

   return address;
}

typedef void* ( *tp_linux_realloc )( void*, size_t );
static tp_linux_realloc __realloc = nullptr;
void* realloc( void* address, size_t size )
{
   if( nullptr == __realloc )
   {
      SYS_WRN( " __realloc = nullptr" );
      __realloc = ( tp_linux_realloc ) dlsym(RTLD_NEXT, "realloc");
      if( nullptr == __realloc )
      {
         SYS_ERR( " __realloc = nullptr" );
         return nullptr;
      }
   }
   void* new_address = __realloc( address, size );
   SYS_VRB( "new address = %p / address = %p / size = %zu", new_address, address, size );

   return new_address;
}

typedef void ( *tp_linux_free )( void* );
static tp_linux_free __free = nullptr;
void free( void* address )
{
   if( nullptr == __free )
   {
      SYS_WRN( "__free = nullptr" );
      __free = ( tp_linux_free ) dlsym(RTLD_NEXT, "free");
      if( nullptr == __free )
      {
         SYS_ERR( " __free = nullptr" );
         return;
      }
   }
   SYS_VRB( "address = %p", address );
   __free( address );
}

void* operator new( size_t size )
{
   void* address = malloc( size );
   if( nullptr == address )
   {
      SYS_WRN( "bad_alloc" );
      throw std::bad_alloc();
   }

   return address;
}

void operator delete( void* address )
{
   free( address );
}

#endif



#if 0 // hook using depricated __malloc_hook

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
   SYS_VRB( "address = %p / size = %zu", address, size );

   return address;
}

void* memalign( size_t alignment, size_t size, const void* )
{
   __memalign_hook = glibc::memalign;
   void* address = ::memalign( alignment, size );
   __memalign_hook = memory::memalign;
   SYS_VRB( "address = %p / size = %zu / alignment = %zu", address, size, alignment );

   return address;
}

void* realloc( void* address, size_t size, const void* )
{
   __realloc_hook = glibc::realloc;
   void* new_address = ::realloc( address, size );
   __realloc_hook = memory::realloc;
   SYS_VRB( "address = %p / size = %zu / new address = %p", address, size, new_address );

   return new_address;
}

void free( void* address, const void* )
{
   SYS_VRB( "address = %p", address );
   __free_hook = glibc::free;
   ::free( address );
   __free_hook = memory::free;
}

}

void* calloc( size_t number, size_t size )
{
   void* address = ::malloc( number * size );
   SYS_VRB( "address = %p / size = %zu / number = %zu", address, size, number );
   return address;
}

#endif

