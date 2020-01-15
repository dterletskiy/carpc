#include "common/ByteBuffer.hpp"
#include "oswrappers/linux/socket/Helpers.hpp"
#include "oswrappers/Thread.hpp"

#include "service/ServiceProcess.hpp"
#include "application/components/OnOff.hpp"
#include "application/components/Driver.hpp"
#include "application/components/Master.hpp"
#include "application/components/Slave.hpp"

#include "Trace.hpp"
#define CLASS_ABBR "MAIN"




// void thread_loop( int index )
// {
//    int count = 0;
//    while( count < 10000 )
//    {
//       DBG_TRC( "Thread ID: %#x / index: %d", pthread_self( ), index );
//       ++count;
//    }
// }

// base::os::Thread thread __attribute__ (( section ("THREAD"), init_priority (101) )) = { thread_loop, 45 };

// void __constructor__( ) __attribute__(( constructor(102) ));
// void __destructor__( ) __attribute__(( destructor(102) ));

// void __constructor__( )
// {
//    DBG_INF( );
//    // thread.run( );
// }

// void __destructor__( )
// {
//    DBG_INF( );
//    // thread.join( );
// }



#include "oswrappers/linux/memory/Hooks.hpp"
extern MemoryMap s_memory_map;



#include "events/Events.hpp"

class EventData
{
public:
   EventData( const uint64_t _id, const std::string& _info )
      : id( _id )
      , info( _info )
   { }
   EventData( const EventData& data )
   {
      id = data.id;
      info = data.info;
   }
   ~EventData( ) { };

   uint64_t       id;
   std::string    info;
};
DECLARE_EVENT( Event, EventData, IEventConsumer );
INIT_EVENT( Event );



template< typename TYPE >
void foo( const TYPE& value )
{
   DBG_WRN( "undefined" );
}

template< >
void foo< uint64_t >( const uint64_t& value )
{
   DBG_INF( "uint64_t: %ld", value );
}

void foo( const uint64_t& value )
{
   DBG_INF( "uint64_t: %ld", value );
}


int main( int argc, char *argv[] )
{
#if 0
   base::ByteBuffer buffer( 100 );

   uint64_t uint64_in = 789;
   std::string string_in( "fuck_string" );
   double double_in = 123.456;

   buffer << uint64_in << string_in << double_in;
   buffer.print( );

   uint64_t uint64_out = 0;
   std::string string_out;
   double double_out = 0;

   buffer >> double_out >> string_out >> uint64_out;
   buffer.print( );

   DBG_INF( "%f", double_out );
   DBG_INF( "%s", string_out.c_str( ) );
   DBG_INF( "%ld", uint64_out );
#endif

#if 0
   base::ByteBuffer buffer( 100 );

   uint64_t uint64_in = 789;
   std::string string_in( "fuck_string" );
   double double_in = 123.456;

   buffer.push_back( uint64_in );
   buffer.print( );
   buffer.push_back( string_in );
   buffer.print( );
   buffer.push_back( double_in );
   buffer.print( );

   uint64_t uint64_out = 0;
   std::string string_out;
   double double_out = 0;

   buffer.pop_back( double_out );
   buffer.print( );
   buffer.pop_back( string_out );
   buffer.print( );
   buffer.pop_back( uint64_out );
   buffer.print( );

   DBG_INF( "%f", double_out );
   DBG_INF( "%s", string_out.c_str( ) );
   DBG_INF( "%ld", uint64_out );
#endif

#if 1
   DBG_INF( "-----------------------------------------------" );
   {
      // base::ByteBuffer buffer( 100 );
      base::TBaseEvent_ID< uint64_t > event_id( "event" );
      // DBG_INF( "%s", event_id.c_str( ) );
      // event_id.to_buffer( buffer );
   }

   {
      base::ByteBuffer buffer( 100 );
      base::TBaseEvent_ID< std::string > event_name( "event" );
      DBG_INF( "%s", event_name.c_str( ) );
      buffer << event_name;
      buffer.print( );

      base::TBaseEvent_ID< std::string > event_name_new( "event_new" );
      DBG_INF( "%s", event_name_new.c_str( ) );
      buffer >> event_name_new;
      buffer.print( );
      DBG_INF( "%s", event_name_new.c_str( ) );
   }
   DBG_INF( "-----------------------------------------------" );
#endif

   


// return 0;











   s_memory_map.dump( );

   base::ServiceInfoVector services =
   {
        { "OnOff_Service", { application::onoff::creator }, 5 }
      , { "Driver_Service", { application::driver::creator }, 10 }
      , { "Device_Service", { application::master::creator, application::slave::creator }, 10 }
   };

   base::IServiceProcessPtr p_process = base::ServiceProcess::instance( );
   if( p_process->init( services ) )
   {
      DBG_MSG( "Booting..." );
      p_process->boot( );
   }

    s_memory_map.dump( );



   DBG_TRC( "Main: program exiting." );
   DBG_INF( "Main: program exiting." );
   DBG_MSG( "Main: program exiting." );
   DBG_WRN( "Main: program exiting." );
   DBG_ERR( "Main: program exiting." );

   return 0;
}
