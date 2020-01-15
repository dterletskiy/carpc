#pragma once



namespace base {



class IEventConsumer
{
public:
   IEventConsumer( ) { }
   virtual ~IEventConsumer( ) { }
};

template< typename _Generator >
class TEventConsumer
   : public IEventConsumer
{
public:
   using _EventType = typename _Generator::Config::EventType;

   TEventConsumer( ) { }
   ~TEventConsumer( ) override { _EventType::clear_all_notifications( this ); }

   virtual void process_event( const _EventType& ) = 0;
};



} // namespace base
