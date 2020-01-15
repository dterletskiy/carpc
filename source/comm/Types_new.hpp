#pragma once

#include "common/Types.hpp"
#include "tools/Tools.hpp"



namespace base {



   // link: https://stackoverflow.com/questions/25492589/can-i-use-sfinae-to-selectively-define-a-member-variable-in-a-template-class
   template< typename TYPE, typename Enable = void >
   class BaseEvent_ID;

   template< typename TYPE >
   class BaseEvent_ID< TYPE, std::enable_if_t< true == std::is_same< TYPE, std::string >::value > >
   {
   public:
      BaseEvent_ID( const std::string& name )
         : m_id ( name )
      { }
      const char* c_str( ) const { return m_id.c_str( ); }

   // const uint8_t* to_ipc_buffer( )
   // {
   //    const size_t size_id = m_id.size( ) + 1;
   //    const size_t buffer_size = size_id;
   //    const uint8_t* p_buffer = (uint8_t*)malloc( buffer_size );
   //    uint8_t* p_buffer_iterator = const_cast< uint8_t* >( p_buffer );
   //    memset( p_buffer_iterator, 0, buffer_size );

   //    memcpy( p_buffer_iterator, m_id.c_str( ), size_id );
   //    p_buffer_iterator += size_id;

   //    return p_buffer;
   // }

   // static BaseEvent_ID from_ipc_buffer( const uint8_t* p_buffer )
   // {
   //    uint8_t* p_buffer_iterator = const_cast< uint8_t* >( p_buffer );

   //    const size_t size_id = strlen( (char*)p_buffer_iterator ) + 1;
   //    char* p_id = (char*)malloc( size_id );
   //    memcpy( p_id, p_buffer_iterator, size_id );

   //    BaseEvent_ID event( p_id );
   //    free( p_id );

   //    return event;
   // }

   protected:
      TYPE           m_id;
   };

   template< typename TYPE >
   class BaseEvent_ID< TYPE, std::enable_if_t< true == std::is_integral< TYPE >::value > >
   {
   public:
      BaseEvent_ID( const std::string& name )
         : m_id ( tools::generate_id( "Event" ) )
         , m_name( name )
      { }
      const char* c_str( ) const { return m_name.c_str( ); }
   protected:
      TYPE           m_id;
      std::string    m_name;
   };

   template< typename TYPE >
   class TEvent_ID : public BaseEvent_ID< TYPE >
   {
   public:
      template< typename T >
         friend std::stringstream& operator << ( std::stringstream& os, const TEvent_ID< T >& id );
      template< typename T >
         friend std::stringstream& operator >> ( std::stringstream& is, TEvent_ID< T >& id );

      template< typename T >
         friend const uint8_t* to_ipc_buffer( const TEvent_ID< T >& id );

   public:
      TEvent_ID( const std::string& name ) : BaseEvent_ID< TYPE >( name ) { }
      ~TEvent_ID( ) { }

      const bool operator==( const TEvent_ID& id ) const { return BaseEvent_ID< TYPE >::m_id == id.m_id; }
      const bool operator!=( const TEvent_ID& id ) const { return BaseEvent_ID< TYPE >::m_id != id.m_id; }
      const bool operator>( const TEvent_ID& id ) const { return BaseEvent_ID< TYPE >::m_id > id.m_id; }
      const bool operator<( const TEvent_ID& id ) const { return BaseEvent_ID< TYPE >::m_id < id.m_id; }
   };

   template< typename T >
   std::stringstream& operator << ( std::stringstream& os, const TEvent_ID< T >& id )
   {
      if constexpr ( std::is_integral< T >::value )
      {
         os << id.m_id;
         os << id.m_name;
      }
      else if constexpr ( std::is_same< T, std::string >::value )
      {
         os << id.m_id;
      }

      return os;
   }

   template< typename T >
   std::stringstream& operator >> ( std::stringstream& is, TEvent_ID< T >& id )
   {
      if constexpr ( std::is_integral< T >::value )
      {
         is >> id.m_id;
         is >> id.m_name;
      }
      if constexpr( std::is_same< T, std::string >::value )
      {
         is >> id.m_id;
      }

      return is;
   }



   template< typename T >
   const uint8_t* to_ipc_buffer( )
   {
      const size_t size_id = sizeof( m_id );
      const size_t size_name = m_name.size( ) + 1;
      const size_t buffer_size = size_id + size_name;
      const uint8_t* p_buffer = (uint8_t*)malloc( buffer_size );
      uint8_t* p_buffer_iterator = const_cast< uint8_t* >( p_buffer );
      memset( p_buffer_iterator, 0, buffer_size );

      memcpy( p_buffer_iterator, &m_id, size_id );
      p_buffer_iterator += size_id;

      memcpy( p_buffer_iterator, m_name.c_str( ), size_name );
      p_buffer_iterator += size_id;

      return p_buffer;
   }

   // template< typename T >
   // static Event* from_ipc_buffer( const uint8_t* p_buffer )
   // {
   //    uint8_t* p_buffer_iterator = const_cast< uint8_t* >( p_buffer );

   //    const size_t size_id = sizeof( uint64_t );
   //    uint64_t id = 11111;
   //    memcpy( &id, p_buffer_iterator, size_id );
   //    p_buffer_iterator += size_id;
   //    DBG_TRC( "%d", id );

   //    const size_t size_name = strlen( (char*)p_buffer_iterator ) + 1;
   //    char* p_name = (char*)malloc( size_name );
   //    memcpy( p_name, p_buffer_iterator, size_name );
   //    DBG_TRC( "%s", p_name );

   //    return new Event( id, p_name );
   // }



} // namespace base



namespace base {

   enum class eCommType : size_t { IPC, ITC, ETC };

   using tEvent_ID = ID;
   // using tEvent_ID = std::string;

   class IEventConsumer;

   class Event;
   using EventPtr = std::shared_ptr< Event >;

   using Event_ID = TEvent_ID< tEvent_ID >;



} // namespace base
