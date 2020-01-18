#pragma once



namespace base {



class IEventConsumer
{
public:
   IEventConsumer( ) = default;
   virtual ~IEventConsumer( ) = default;
};

template< typename _Generator >
class TEventConsumer
   : public IEventConsumer
{
public:
   using _EventType = typename _Generator::Config::EventType;

   TEventConsumer( ) = default;
   ~TEventConsumer( ) override { _EventType::clear_all_notifications( this ); }

   virtual void process_event( const _EventType& ) = 0;
};



} // namespace base
