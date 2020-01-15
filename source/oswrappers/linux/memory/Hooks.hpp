#pragma once



// #include <cstddef>

// void* malloc( size_t size );
// void free( void* p );

// void* operator new( size_t size );
// void operator delete( void* p );




const uint8_t s_buffer_size = 64;

class MemoryMap
{
private:
   static const size_t s_invalid_entry = SIZE_MAX;
   static const size_t s_memory_map_size = 1042 * 1024 * 16;
   static const uint8_t s_buffer_size = ::s_buffer_size;

   struct MemoryInfo
   {
      MemoryInfo( ) { }
      MemoryInfo( void*& _address, const size_t& _size )
         : address( _address )
         , size( _size )
      { }
      ~MemoryInfo( ) { }

      void*    address  = nullptr;
      size_t   size     = 0;
   } m_mem_map[ s_memory_map_size ];

public:
   MemoryMap( );
   ~MemoryMap( );

   bool insert( void* _address, const size_t _size );
   bool remove( const void* _address );
   size_t find_free( ) const;
   void dump( ) const;
};


