#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

// #include "Trace.hpp"
// #define CLASS_ABBR "Memory"



#ifdef HOOK_MEMORY_ALLOCATOR

static const uint8_t s_buffer_size = 64;

typedef void* ( *tp_linux_malloc )( size_t );
static tp_linux_malloc __malloc = nullptr;
void* malloc( size_t size )
{
   char p_buffer[s_buffer_size];
   if( nullptr == __malloc )
   {
      // write( STDOUT_FILENO, p_buffer, sprintf( p_buffer, "malloc: __malloc = nullptr\n" ) );
      __malloc = ( tp_linux_malloc ) dlsym(RTLD_NEXT, "malloc");
   }
   void* p = __malloc(size);
   write( STDOUT_FILENO, p_buffer, sprintf( p_buffer, "malloc: size=%zu, addr=%p\n", size, p ) );
   return p;
}

typedef void ( *tp_linux_free )( void* );
static tp_linux_free __free = nullptr;
void free( void* p )
{
   char p_buffer[s_buffer_size];
   if( nullptr == __free )
   {
      // write( STDOUT_FILENO, p_buffer, sprintf( p_buffer, "free: __free = nullptr\n" ) );
      __free = ( tp_linux_free ) dlsym(RTLD_NEXT, "free");
   }
   write( STDOUT_FILENO, p_buffer, sprintf( p_buffer, "free: addr=%p\n", p ) );
   __free( p );
}

void* operator new( size_t size )
{
   char p_buffer[s_buffer_size];
   void* p = malloc( size );
   if( nullptr == p )
   {
      // write( STDOUT_FILENO, p_buffer, sprintf( p_buffer, "new: bad_alloc\n" ) );
      throw std::bad_alloc();
   }

   write( STDOUT_FILENO, p_buffer, sprintf( p_buffer, "new: size=%zu, addr=%p\n", size, p ) );
   return p;
}

void operator delete( void* p )
{
   char p_buffer[s_buffer_size];
   free( p );
   write( STDOUT_FILENO, p_buffer, sprintf( p_buffer, "delete: addr=%p\n", p ) );
}

#endif
