#pragma once

// Framework
#include "api/sys/comm/timer/Timer.hpp"
#include "api/service/onoff/2.0.0/Server.hpp"



namespace application::components::onoff {

   class Server
      : public service::onoff::V2_0_0::Server
      , public carpc::timer::ITimerConsumer
   {
      public:
         Server( const std::string&, const std::string& name = { } );
         ~Server( );

      public:
         void connected( ) override;
         void disconnected( ) override;

      public:
         void request_start( ) override;
         void request_trigger_state( const std::string&, const size_t ) override;

      private:
         const std::string m_name = { };

      // Timer
      private:
         void process_timer( const carpc::comm::timer::ID ) override;
         struct TimerSeqID
         {
            std::string current_state;
            std::shared_ptr< carpc::timer::Timer > timer;
            carpc::comm::sequence::ID seq_id;
         };
         std::list< TimerSeqID > m_timers;
   };

} // namespace application::components::onoff
