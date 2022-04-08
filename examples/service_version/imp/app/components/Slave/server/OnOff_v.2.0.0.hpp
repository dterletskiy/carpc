#pragma once

// Framework
#include "api/sys/comm/timer/Timer.hpp"
#include "api/app/services/onoff/2.0/Server.hpp"



namespace application::components::slave::server::V2_0_0 {

   class OnOff
      : public service::onoff::V2_0_0::Server
      , public carpc::timer::ITimerConsumer
   {
      public:
         OnOff( const std::string& );
         ~OnOff( );

      public:
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

} // namespace application::components::slave::server::V2_0_0
