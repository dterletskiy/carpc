#include <cstdint>

#pragma once



namespace hook::memory {



struct MemoryHeader
{
   void*    caller   = nullptr;
   size_t   size     = 0;
   uint64_t time     = 0;

   static MemoryHeader* cast( void* );
   static void dump( void* );
   static void write( void* _address, void* _caller, const size_t _size, const uint64_t _time );
};



using tHeaderType = MemoryHeader;
constexpr size_t s_header_size = sizeof( tHeaderType );

class MemoryMap
{
private:
   static const size_t s_invalid_entry = SIZE_MAX;
   static const size_t s_memory_map_size = 1042 * 1024 * 16;

   using tEntry = struct { void* address; };

   tEntry         m_mem_map[ s_memory_map_size ];
   size_t         m_min_track_size = 0;
   int            m_last_errno = 0;
   int            m_fd = -1;
   const char*    m_path = nullptr;

public:
   MemoryMap( const char*, const size_t size = 0 );
   ~MemoryMap( );

   void set_track_size( const size_t size );

   bool insert( void* _address, void* _caller, size_t _size );
   bool remove( const void* _address );
   size_t find_free( ) const;
   void dump( ) const;
   const size_t records( ) const;
};


} // namespace hook::memory
