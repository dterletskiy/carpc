#pragma once

#include "imp/sys/events/SysEvent.hpp"



namespace base {



class RootComponent;



class SysEventConsumer
   : public SysEvent::Consumer
{
public:
   static SysEventConsumer* instance( RootComponent* );
   ~SysEventConsumer( ) override;
private:
   SysEventConsumer( RootComponent* );
   SysEventConsumer( const SysEventConsumer& ) = delete;
   SysEventConsumer& operator=( const SysEventConsumer& ) = delete;
   static SysEventConsumer* mp_instance;

private:
   RootComponent* mp_root_component;

private:
   void process_event( const SysEvent::Event& ) override;
};




} // namespace base
