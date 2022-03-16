#pragma once

// Framework
#include "api/sys/comm/timer/Timer.hpp"
#include "api/sys/services/onoff/Server.hpp"



namespace application::components::onoff {

   class Server
      : public base::onoff::Server
      , public base::timer::ITimerConsumer
   {
      public:
         Server( const std::string&, const std::string& name = { } );
         ~Server( );

      public:
         void request_start( ) override;
         void request_trigger_state( const std::string&, const size_t ) override;

      private:
         const std::string m_name = { };

      // Timer
      private:
         void process_timer( const base::comm::timer::ID ) override;
         struct TimerSeqID
         {
            std::string current_state;
            std::shared_ptr< base::timer::Timer > timer;
            base::comm::sequence::ID seq_id;
         };
         std::list< TimerSeqID > m_timers;
   };

} // namespace application::components::onoff
