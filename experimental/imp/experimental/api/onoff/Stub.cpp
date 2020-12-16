#include "imp/experimental/api/onoff/Types.hpp"
#include "imp/experimental/api/onoff/Stub.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Stub"



namespace api::onoff {

   class EventConsumerStub : public IEventConsumer
   {
      public:
         EventConsumerStub( Stub& );

         void process( const Event& ) override;

      private:
         Stub& m_stub;
   };



   EventConsumerStub::EventConsumerStub( Stub& stub )
      : m_stub( stub )
   {
   }

   void EventConsumerStub::process( const Event& event )
   {
      eEventID id = static_cast< eEventID >( event.info_t( ).id( ) );
      switch( id )
      {
         case eEventID::RequestBoot:
         {
            auto data = std::static_pointer_cast< RequestBootData >( event.data( ).mp_data );
            m_stub.request_boot( data->message );
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



Stub::Stub( const fw::service::tServiceRole& role )
   : fw::service::IStub( ServiceID, role )
{
   mp_event_consumer = std::make_shared< EventConsumerStub >( *this );

   Event::set_notification( mp_event_consumer, convert( eEventID::RequestBoot ) );
}

Stub::~Stub( )
{
}

void Stub::response_boot( const bool result )
{
   EventData data = { std::make_shared< ResponseBootData >( result ) };
   Event::create( convert( eEventID::ResponseBoot ), data )->send( );
}
