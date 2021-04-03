#pragma once

// Framework
#include "api/sys/component/RootComponent.hpp"
#include "api/sys/comm/timer/Timer.hpp"
// Application
#include "imp/controller/components/onoff/server/Server.hpp"
#include "imp/controller/components/onoff/client/Client.hpp"



namespace controller::event {

   enum class eID : size_t { boot, shutdown, ping, undefined };
   const char* c_str( const eID );

   struct Data
   {
      bool to_stream( base::ipc::tStream& _stream ) const { return true; }
      bool from_stream( base::ipc::tStream& _stream ) { return true; }
   };

   DEFINE_IPC_EVENT( App, Data, base::async::id::TSignature< eID > );

}



namespace controller::components::onoff {

   class Component
      : public base::component::RootComponent
      , public base::ITimerConsumer
      , public controller::event::App::Consumer
   {
      public:
         static base::component::IComponent::tSptr creator( base::application::IThread& service );

      private:
         Component( base::application::IThread& service, const std::string& );
      public:
         ~Component( ) override;

      private:
         void boot( const std::string& ) override;

      private:
         void process_timer( const base::Timer::ID ) override;
         base::Timer m_timer;

      public:
         void on_timer( const base::Timer::ID );

      private:
         void process_event( const controller::event::App::Event& ) override;

      private:
         Server m_server_onoff;
         Client m_client_onoff;
   };

} // namespace controller::components::onoff
