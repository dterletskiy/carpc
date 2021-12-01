// Framework
#include "api/sys/application/main.hpp"



const base::application::Thread::Configuration::tVector services = { };
const char* const application_name{ "EXP" };



#include <memory>
#include <future>
#include "api/sys/oswrappers/Thread.hpp"



void thread_loop( const std::string form, std::future< bool >& future )
{
   MSG_DBG( "Handle the submitted form: %s", form.c_str( ) );
   sleep( 10 );
   MSG_DBG( "done" );
}

bool form_processor_packaged_task( const std::string& form )
{
   MSG_DBG( "Handle the submitted form: %s", form.c_str( ) );
   sleep( 10 );
   MSG_DBG( "done" );
   return true;
};

std::future< bool > submit_form_packaged_task( const std::string& form )
{
   MSG_INF( "enter" );

   std::packaged_task< bool ( const std::string& ) > task( form_processor_packaged_task );
   std::future< bool > future = task.get_future( );

   std::thread thread( std::move( task ), form );
   thread.detach( );

   MSG_INF( "exit" );
   return std::move( future );
}

bool form_processor_promise(
      const std::string& form,
      std::shared_ptr< std::promise< bool > > sp_promise
   )
{
   MSG_DBG( "Handle the submitted form: %s", form.c_str( ) );
   sleep( 10 );
   MSG_DBG( "done" );
   sp_promise->set_value( true );
   sleep( 5 );
   MSG_DBG( "exit" );

   return true;
};

std::future< bool > submit_form_promise( const std::string& form )
{
   MSG_INF( "enter" );

   std::shared_ptr< std::promise< bool > > sp_promise =
      std::make_shared< std::promise< bool > >( );
   std::future< bool > future = sp_promise->get_future( );

   std::thread thread( form_processor_promise, form, sp_promise );
   thread.detach( );

   MSG_INF( "exit" );
   return std::move( future );
}

bool test( int argc, char** argv, char** envp )
{
   std::string form = "Form_0x7BFA1C";

   auto check = submit_form_promise( form );

   MSG_INF( "waiting form" );
   if( check.get( ) )
   {
      MSG_DBG( "Wow I've got a passport!" );
   }
   else
   {
      MSG_DBG( "Dammit, they found a mistake again!" );
   }

   sleep( 10 );

   return false;
}
