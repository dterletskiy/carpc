#include <cstdint>
#include <pthread.h>

#pragma once



namespace hook::memory {



   struct MemoryHeader
   {
      void*       caller   = nullptr;
      std::size_t size     = 0;
      uint64_t    time     = 0;
      pthread_t   thread   = 0;

      static MemoryHeader* cast( void* );
      static void dump( void*, const int fd = -1, const char* const message = "" );
      static void write( void* _address, void* _caller, const std::size_t _size, const uint64_t _time, const pthread_t _thread  );
   };



   using tHeaderType = MemoryHeader;
   constexpr std::size_t s_header_size = sizeof( tHeaderType );

   class MemoryMap
   {
      private:
         static const std::size_t s_invalid_entry = SIZE_MAX;
         static const std::size_t s_memory_map_size = 1042 * 1024;

         using tEntry = struct { void* address; };

         tEntry         m_mem_map[ s_memory_map_size ];
         // tEntry*        m_mem_map = nullptr;
         std::size_t    m_records = 0;
         std::size_t    m_min_track_size = 0;
         int            m_last_errno = 0;
         int            m_fd = -1;
         const char*    m_path = nullptr;
         std::size_t    m_common_size = 0;

      public:
         MemoryMap( const char*, const std::size_t size = 0 );
         ~MemoryMap( );

         void init( );
         void deinit( );
         void set_track_size( const std::size_t size );
         void set_track_file( const char* const file );

         bool insert( void* _address, void* _caller, std::size_t _size );
         bool remove( const void* _address );
         std::size_t find_free( ) const;
         void dump( ) const;
         const std::size_t records( ) const;
   };


} // namespace hook::memory
