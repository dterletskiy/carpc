#pragma once



struct MemoryHeader
{
   void*    caller   = nullptr;
   size_t   size     = 0;
   uint64_t time     = 0;
};
constexpr size_t s_header_size = sizeof( MemoryHeader );

class MemoryMap
{
private:
   static const size_t s_invalid_entry = SIZE_MAX;
   static const size_t s_memory_map_size = 1042 * 1024 * 16;

   struct { void* address; } m_mem_map[ s_memory_map_size ];

public:
   MemoryMap( );
   ~MemoryMap( );

   bool insert( void* _address );
   bool remove( const void* _address );
   size_t find_free( ) const;
   void dump( ) const;
   const size_t records( ) const;
};
