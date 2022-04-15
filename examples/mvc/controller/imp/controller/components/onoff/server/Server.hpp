#pragma once

#include "api/sys/comm/timer/Timer.hpp"
#include "api/service/onoff/1.0.0/Server.hpp"



namespace controller::components::onoff {

   class Server
      : public service::onoff::V1_0_0::Server
      , public carpc::timer::ITimerConsumer
   {
      public:
         Server( );
         ~Server( );

      private:
         void connected( ) override;
         void disconnected( ) override;

      public:
         void request_start( ) override;
         void request_trigger_state( const std::string&, const size_t ) override;

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

} // namespace controller::components::onoff
