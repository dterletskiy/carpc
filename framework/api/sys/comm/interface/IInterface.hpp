#pragma once

#include <string>

#include "api/sys/service/IServiceThread.hpp"
#include "api/sys/comm/event/Event.hpp"
#include "api/sys/comm/event/Runnable.hpp"
#include "api/sys/comm/interface/Types.hpp"



namespace base {

   DEFINE_ENUM( eInterface, size_t, ServerConnected, ServerDisconnected, ClientConnected, ClientDisconnected, Undefined );
   struct InterfaceEventData
   {
      bool to_buffer( base::ByteBufferT& ) const;
      bool from_buffer( base::ByteBufferT& );

      const void* ptr = nullptr;
   };
   DECLARE_IPC_EVENT_RR( InterfaceEvent, InterfaceEventData, eInterface );

} // namespace base



namespace base {



class IInterface
   : public InterfaceEvent::Consumer
{
public:
   IInterface( const std::string&, const std::string& );
   virtual ~IInterface( );

public:
   virtual void connected( const void* const ) = 0;
   virtual void disconnected( const void* const ) = 0;

private:
   void process_event( const InterfaceEvent::Event& ) override;

public:
   const std::string& name( ) const;
   const std::string& role( ) const;
   const std::string service_name( ) const;
   const bool cmp_names( const IInterface* const ) const;
private:
   std::string m_name = "";
   std::string m_role = "";

public:
   IServiceThread::tWptr context( ) const;
private:
   IServiceThread::tWptr mp_service_thread;
};



inline
const std::string& IInterface::name( ) const
{
   return m_name;
}

inline
const std::string& IInterface::role( ) const
{
   return m_role;
}

inline
const std::string IInterface::service_name( ) const
{
   return m_role + std::string{"."} + m_name;
}

inline
const bool IInterface::cmp_names( const IInterface* const p_interface ) const
{
   return ( p_interface->name( ) == m_name ) && ( p_interface->role( ) == m_role );
}

inline
IServiceThread::tWptr IInterface::context( ) const
{
   return mp_service_thread;
}



} // namespace base
