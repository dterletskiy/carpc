#pragma once

// Framework
#include "api/sys/component/Component.hpp"
// Application



namespace application::components {



class Slave
   : public base::Component
{
public:
   static base::ComponentPtr creator( base::ServiceThreadPtr );

private:
   Slave( const base::ServiceThreadPtr, const std::string& );
public:
   ~Slave( ) override;
};



} // namespace application::components
