#pragma once

#include "api/sys/component/IComponent.hpp"
#include "api/sys/tools/Performance.hpp"



namespace base {



class Component : public IComponent
{
public:
   Component( const IServiceThread::tSptr, const std::string& );
   ~Component( ) override;

public:
   const std::string& name( ) const override final;
private:
   std::string                m_name;

public:
   const ID& id( ) const override final;
private:
   ID                         m_id;

private:
   IServiceThread::tWptr      mp_service;

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
