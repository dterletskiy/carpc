#pragma once

#include "IComponent.hpp"
#include "tools/Performance.hpp"



namespace base {



class Component
   : public IComponent
{
public:
   Component( const IServicePtr, const std::string& );
   ~Component( ) override;

public:
   const std::string& name( ) const;
   const ID& id( ) const;
   IServicePtr service( ) const;
private:
   std::string    m_name;
   ID             m_id;
   IServicePtrW   mp_service;

protected:
   void start_performance( );
   void stop_performance( );
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
void Component::start_performance( )
{
   m_performance.start( );
}

inline
void Component::stop_performance( )
{
   m_performance.stop( );
}



} // namespace base
