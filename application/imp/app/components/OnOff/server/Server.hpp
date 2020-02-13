#pragma once

// Framework
#include "api/sys/comm/timer/Timer.hpp"
// Interface
#include "api/app/onoff/Server.hpp"



namespace application::components::onoff {



class Server
   : public api::onoff::Server
   , public base::ITimerConsumer
{
public:
   Server( const std::string&, const std::string& name = { } );
   ~Server( );

public:
   void request_trigger_state( const std::string&, const size_t ) override;

private:
   const std::string m_name = { };

// Timer
private:
   void process_timer( const base::TimerID ) override;
   struct TimerSeqID
   {
      std::shared_ptr< base::Timer > timer;
      size_t seq_id;
   };
   std::list< TimerSeqID > m_timers;
};



} // namespace application::components::onoff
