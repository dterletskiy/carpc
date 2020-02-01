#pragma once

// Framework
#include "api/sys/component/Component.hpp"
// Application



namespace application::components {



class Driver
   : public base::Component
{
public:
   static base::ComponentPtr creator( base::ServiceThreadPtr );

private:
   Driver( const base::ServiceThreadPtr, const std::string& );
public:
   ~Driver( ) override;
};



} // namespace application::components
