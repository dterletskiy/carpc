#pragma once

// Framework
#include "api/sys/application/RootComponent.hpp"
#include "api/sys/comm/timer/Timer.hpp"
// Application
#include "imp/core/components/onoff/server/Server.hpp"



namespace core::event {

   enum class eID : size_t { boot, shutdown, ping, undefined };
   const char* c_str( const eID );

   struct Data
   {
      bool to_stream( carpc::ipc::tStream& _stream ) const { return true; }
      bool from_stream( carpc::ipc::tStream& _stream ) { return true; }
   };

   DEFINE_IPC_EVENT( App, Data, carpc::async::id::TSignature< eID > );

}



namespace core::components::onoff {

   class Component
      : public carpc::application::RootComponent
      , public carpc::timer::ITimerConsumer
      , public core::event::App::Consumer
   {
      public:
         static carpc::application::IComponent::tSptr creator( );

      private:
         Component( const std::string& );
      public:
         ~Component( ) override;

      private:
         void process_boot( const std::string& ) override;

      private:
         void process_timer( const carpc::comm::timer::ID ) override;
         carpc::timer::Timer m_timer;

      public:
         void on_timer( const carpc::comm::timer::ID );

      private:
         void process_event( const core::event::App::Event& ) override;

      private:
         Server m_server_onoff;
   };

} // namespace core::components::onoff
