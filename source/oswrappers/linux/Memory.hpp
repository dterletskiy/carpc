#pragma once



#ifdef HOOK_MEMORY_ALLOCATOR

void* malloc( size_t size );
void free( void* p );

void* operator new( size_t size );
void operator delete( void* p );

#endif
