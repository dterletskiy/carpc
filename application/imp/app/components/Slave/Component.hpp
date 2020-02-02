#pragma once

// Framework
#include "api/sys/component/Component.hpp"
// Application



namespace application::components::slave {



class Component
   : public base::Component
{
public:
   static base::ComponentPtr creator( base::ServiceThreadPtr );

private:
   Component( const base::ServiceThreadPtr, const std::string& );
public:
   ~Component( ) override;
};



} // namespace application::components::slave
