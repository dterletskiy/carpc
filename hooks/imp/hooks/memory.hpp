#pragma once



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
      MemoryInfo( void*& _caller, void*& _address, const size_t& _size )
         : caller( _caller )
         , address( _address )
         , size( _size )
      { }
      ~MemoryInfo( ) { }

      void*    caller   = nullptr;
      void*    address  = nullptr;
      size_t   size     = 0;
   } m_mem_map[ s_memory_map_size ];

public:
   MemoryMap( );
   ~MemoryMap( );

   bool insert( void* _caller, void* _address, const size_t _size );
   bool remove( const void* _address );
   size_t find_free( ) const;
   void dump( ) const;
   const size_t records( ) const;
};
