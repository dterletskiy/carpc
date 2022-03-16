#pragma once

// Framework
#include "api/sys/application/RootComponent.hpp"
#include "api/sys/comm/timer/Timer.hpp"
// Application
#include "imp/hmi/components/onoff/client/Client.hpp"



namespace hmi::event {

   enum class eID : size_t { boot, shutdown, ping, undefined };
   const char* c_str( const eID );

   struct Data
   {
      bool to_stream( base::ipc::tStream& _stream ) const { return true; }
      bool from_stream( base::ipc::tStream& _stream ) { return true; }
   };

   DEFINE_IPC_EVENT( App, Data, base::async::id::TSignature< eID > );

}



namespace hmi::components::onoff {

   class Component
      : public base::application::RootComponent
      , public base::timer::ITimerConsumer
      , public hmi::event::App::Consumer
   {
      public:
         static base::application::IComponent::tSptr creator( );

      private:
         Component( const std::string& );
      public:
         ~Component( ) override;

      private:
         void boot( const std::string& ) override;

      private:
         void process_timer( const base::comm::timer::ID ) override;
         base::timer::Timer m_timer;

      public:
         void on_timer( const base::comm::timer::ID );

      private:
         void process_event( const hmi::event::App::Event& ) override;

      private:
         Client m_client_onoff;
   };

} // namespace hmi::components::onoff
