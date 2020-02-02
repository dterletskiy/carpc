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
   Server( const std::string& );
   ~Server( );

public:
   void request_trigger_state( const std::string&, const size_t ) override;

// Timer
private:
   void process_timer( const base::TimerID ) override;
   base::Timer m_timer;
};



} // namespace application::components::onoff
