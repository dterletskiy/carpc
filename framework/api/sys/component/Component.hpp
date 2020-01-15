#pragma once

#include "api/sys/service/Types.hpp"
#include "api/sys/component/Types.hpp"
#include "api/sys/tools/Performance.hpp"



namespace base {



class Component
{
public:
   Component( const ServicePtr, const std::string& );
   virtual ~Component( );

public:
   const std::string& name( ) const;
   const ID& id( ) const;
   ServicePtr service( ) const;
private:
   std::string    m_name;
   ID             m_id;
   ServicePtrW    mp_service;

protected:
   void start_performance( const std::string& message = "" );
   void stop_performance( const std::string& message = "" );
private:
   base::tools::Performance   m_performance;
};



inline
const std::string& Component::name( ) const
{
   return m_name;
}

inline
const ID& Component::id( ) const
{
   return m_id;
}

inline
void Component::start_performance( const std::string& message )
{
   m_performance.start( message );
}

inline
void Component::stop_performance( const std::string& message )
{
   m_performance.stop( message );
}



} // namespace base
