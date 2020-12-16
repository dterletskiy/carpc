#include "imp/experimental/api/onoff/Types.hpp"
#include "imp/experimental/api/onoff/Proxy.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Proxy"



namespace api::onoff {

   class EventConsumerProxy : public IEventConsumer
   {
      public:
         EventConsumerProxy( Proxy& );

         void process( const Event& ) override;

      private:
         Proxy& m_proxy;
   };



   EventConsumerProxy::EventConsumerProxy( Proxy& proxy )
      : m_proxy( proxy )
   {
   }

   void EventConsumerProxy::process( const Event& event )
   {
      eEventID id = static_cast< eEventID >( event.info_t( ).id( ) );
      switch( id )
      {
         case eEventID::ResponseBoot:
         {
            auto data = std::static_pointer_cast< ResponseBootData >( event.data( ).mp_data );
            m_proxy.response_boot( data->result );
            break;
         }
         default:
         {
            break;
         }
      }
   }

}



using namespace api::onoff;



Proxy::Proxy( const fw::service::tServiceRole& role )
   : fw::service::IProxy( ServiceID, role )
{
   mp_event_consumer = std::make_shared< EventConsumerProxy >( *this );
}

Proxy::~Proxy( )
{
}

void Proxy::request_boot( const std::string& message )
{
   SYS_INF( "message: %s", message.c_str( ) );

   EventData data = { std::make_shared< RequestBootData >( message ) };
   Event::create( convert( eEventID::RequestBoot ), data )->send( );

   Event::set_notification( mp_event_consumer, convert( eEventID::ResponseBoot ) );
}
