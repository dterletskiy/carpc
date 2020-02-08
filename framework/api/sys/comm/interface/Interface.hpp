#pragma once

#include <string>

#include "api/sys/service/Types.hpp"
#include "api/sys/comm/event/Event.hpp"



namespace base {



class Server;
class Client;



class Interface
{
public:
   enum class eType { server, client, undefined };

public:
   Interface( const std::string&, const std::string&, const bool, const eType );
   virtual ~Interface( );

public:
   const std::string& name( ) const;
   const std::string& role( ) const;
   const std::string service_name( ) const;
   const bool cmp_names( const Interface* const ) const;
private:
   std::string m_name = "";
   std::string m_role = "";

public:
   const bool is_ipc( ) const;
   const eCommType comm_type( ) const;
private:
   bool m_is_ipc = false;

public:
   bool is_connected( ) const;
   virtual void connected( ) const = 0;
   virtual void disconnected( ) const = 0;
private:
   bool m_is_connected = false;

public:
   eType type( ) const;
private:
   eType m_type = eType::undefined;

protected:
   const bool register_server( const Server* const );
   const bool register_client( const Client* const );
   const bool unregister_server( const Server* const );
   const bool unregister_client( const Client* const );

public:
   ServiceThreadPtrW context( ) const;
private:
   ServiceThreadPtrW mp_service_thread;

protected:
   template< typename tDataRR >
   void set_notification( typename tDataRR::tEventConsumer* p_consumer )
   {
      tDataRR::tEvent::set_notification( p_consumer, typename tDataRR::tEvent::Signature( role( ), tDataRR::id ) );
   }
   template< typename tDataRR >
   void clear_notification( typename tDataRR::tEventConsumer* p_consumer )
   {
      tDataRR::tEvent::clear_notification( p_consumer, typename tDataRR::tEvent::Signature( role( ), tDataRR::id ) );
   }
   template< typename tDataRR >
   void clear_all_notifications( typename tDataRR::tEventConsumer* p_consumer )
   {
      tDataRR::tEvent::clear_all_notifications( p_consumer );
   }
   template< typename tDataRR >
   void create_send( std::shared_ptr< tDataRR > p_data )
   {
      typename tDataRR::tEventData data( p_data );
      tDataRR::tEvent::create_send( typename tDataRR::tEvent::Signature( role( ), tDataRR::id ), data, comm_type( ) );
   }
   template< typename tDataRR, typename... Args >
   void create_send( const Args&... args )
   {
      typename tDataRR::tEventData data( std::make_shared< tDataRR >( args... ) );
      tDataRR::tEvent::create_send( typename tDataRR::tEvent::Signature( role( ), tDataRR::id ), data, comm_type( ) );
   }
   template< typename tDataRR >
   const tDataRR* get_event_data( const typename tDataRR::tEvent& event )
   {
      return static_cast< tDataRR* >( event.data( )->ptr.get( ) );
   }
};



inline
const std::string& Interface::name( ) const
{
   return m_name;
}

inline
const std::string& Interface::role( ) const
{
   return m_role;
}

inline
const std::string Interface::service_name( ) const
{
   return m_role + std::string{"."} + m_name;
}

inline
const bool Interface::is_ipc( ) const
{
   return m_is_ipc;
}

inline
const eCommType Interface::comm_type( ) const
{
   return m_is_ipc ? base::eCommType::IPC : base::eCommType::ITC;
}

inline
const bool Interface::cmp_names( const Interface* const p_interface ) const
{
   return ( p_interface->name( ) == m_name ) && ( p_interface->role( ) == m_role );
}

inline
bool Interface::is_connected( ) const
{
   return m_is_connected;
}

inline
Interface::eType Interface::type( ) const
{
   return m_type;
}

inline
ServiceThreadPtrW Interface::context( ) const
{
   return mp_service_thread;
}



} // namespace base
