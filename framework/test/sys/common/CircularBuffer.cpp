#include "api/sys/helpers/macros/strings.hpp"
#include "api/sys/common/CircularBuffer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TEST:C_BUFFER"



namespace base::dsi::circular_buffer::test {

   bool run( )
   {
      auto push_back = [ ]( base::CircularBuffer& cb, const size_t size, const size_t marker )
      {
         DBG_MSG( "push_back %zu bytes", size );
         base::RawBuffer rb;
         rb.alloc( size );
         rb.fill( marker, 1 );
         const base::CircularBuffer::ePush result = cb.push_back( rb );
         DBG_WRN( "result: %s", base::CircularBuffer::c_str( result ) );
         cb.dump( );
         printf( "\n\n" );
      };

      auto move_front = [ ]( base::CircularBuffer& cb, const size_t size )
      {
         DBG_MSG( "move_front %zu bytes", size );
         base::RawBuffer rb;
         rb.alloc( size );
         const bool result = cb.move_front( rb );
         rb.dump( );
         DBG_WRN( "result: %s", BOOL_TO_STRING( result ) );
         cb.dump( );
         printf( "\n\n" );
      };

      auto pop_back = [ ]( base::CircularBuffer& cb, const size_t size )
      {
         DBG_MSG( "pop_back %zu bytes", size );
         cb.pop_back( size );
         cb.dump( );
         printf( "\n\n" );
      };



      base::CircularBuffer cb( 10, false, true );
      cb.dump( );
      printf( "\n\n" );

      push_back( cb, 5, 0xA0 );
      push_back( cb, 5, 0xB0 );
      move_front( cb, 3 );
      push_back( cb, 3, 0xC0 );
      push_back( cb, 8, 0xD0 );
      pop_back( cb, 1 );
      push_back( cb, 5, 0xE0 );

      base::CircularBuffer cb_new( cb );
      cb_new.dump( );

      DBG_WRN( "--------------------------------------------------------------" );
      cb.push_back( cb_new );
      cb.dump( );
      cb_new.dump( );
      DBG_WRN( "--------------------------------------------------------------" );
      cb.move_front( cb_new );
      cb.dump( );
      cb_new.dump( );

      return true;
   }

}