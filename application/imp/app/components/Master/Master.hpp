#pragma once

// Framework
#include "api/sys/component/Component.hpp"
// Application



namespace application::components {



class Master
   : public base::Component
{
public:
   static base::ComponentPtr creator( base::ServiceThreadPtr );

private:
   Master( const base::ServiceThreadPtr, const std::string& );
public:
   ~Master( ) override;
};



} // namespace application::components
