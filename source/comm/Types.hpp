#pragma once

#include "common/Types.hpp"
#include "common/ByteBuffer.hpp"
#include "common/Helpers.hpp"
#include "tools/Tools.hpp"



namespace base {



   template< typename T >
   class TBaseEvent_ID
   {
   public:
      bool to_buffer( ByteBuffer& buffer )
      {
         if constexpr( std::is_integral< T >::value )
         {
            return buffer.push_back( m_id );
         }
         else if constexpr( std::is_same< T, std::string >::value )
         {
            bool result = buffer.push_back( m_id );
            if( false == result ) return false;
            return buffer.push_back( (size_t)m_id.size( ) );
         }
         return false;
      }
      bool from_buffer( ByteBuffer& buffer )
      {
         if constexpr( std::is_integral< T >::value )
         {
            return buffer.pop_back( m_id );
         }
         else if constexpr( std::is_same< T, std::string >::value )
         {
            size_t size = 0;
            bool result = buffer.pop_back( size );
            if( false == result ) return false;
            return buffer.pop_back( m_id, size + 1 ); 
         }
         return false;
      }

   public:
      TBaseEvent_ID( const std::string& name ) { id( name.c_str( ) ); }
      TBaseEvent_ID( const char* const name ) { id( name ); }
      ~TBaseEvent_ID( ) { }

      const char* c_str( ) const { return base::to_string( m_id ).c_str( ); }
      const std::string to_string( ) const { return base::to_string( m_id ); }

   public:
      const bool operator==( const TBaseEvent_ID& id ) const { return m_id == id.m_id; }
      const bool operator!=( const TBaseEvent_ID& id ) const { return m_id != id.m_id; }
      const bool operator>( const TBaseEvent_ID& id ) const  { return m_id > id.m_id; }
      const bool operator<( const TBaseEvent_ID& id ) const  { return m_id < id.m_id; }

   private:
      void id( const char* const name )
      {
         if constexpr( std::is_integral< T >::value )
         {
            m_id = tools::generate_id( "Event" );
         }
         else if constexpr( std::is_same< T, std::string >::value )
         {
            m_id = name;
         }
      }
   protected:
      T     m_id;
   };



   // template< typename T >
   // class TBaseEvent_ID
   // {
   // public:
   //    TBaseEvent_ID( const std::string& name ) : m_id( tools::generate_id( "Event" ) ) { }
   //    TBaseEvent_ID( const char* const name ) : m_id( tools::generate_id( "Event" ) ) { }
   //    ~TBaseEvent_ID( ) { }

   // public:
   //    const bool operator==( const TBaseEvent_ID& id ) const { return m_id == id.m_id; }
   //    const bool operator!=( const TBaseEvent_ID& id ) const { return m_id != id.m_id; }
   //    const bool operator>( const TBaseEvent_ID& id ) const  { return m_id > id.m_id; }
   //    const bool operator<( const TBaseEvent_ID& id ) const  { return m_id < id.m_id; }

   // protected:
   //    T     m_id;
   // };



   // template< >
   // class TBaseEvent_ID< std::string >
   // {
   // public:
   //    TBaseEvent_ID( const std::string& name ) : m_id( name ) { }
   //    TBaseEvent_ID( const char* const name ) : m_id( name ) { }
   //    ~TBaseEvent_ID( ) { }

   // public:
   //    const bool operator==( const TBaseEvent_ID& id ) const { return m_id == id.m_id; }
   //    const bool operator!=( const TBaseEvent_ID& id ) const { return m_id != id.m_id; }
   //    const bool operator>( const TBaseEvent_ID& id ) const  { return m_id > id.m_id; }
   //    const bool operator<( const TBaseEvent_ID& id ) const  { return m_id < id.m_id; }

   // protected:
   //    std::string m_id;
   // };



   template< typename T, bool TNameEnabler >
   class TEvent_ID;



   template< typename T >
   class TEvent_ID< T, false > : public TBaseEvent_ID< T >
   {
   public:
      TEvent_ID( const std::string& name ) : TBaseEvent_ID< T >( name ) { }
      TEvent_ID( const char* const name ) : TBaseEvent_ID< T >( name ) { }
      ~TEvent_ID( ) { }
   };



   template< typename T >
   class TEvent_ID< T, true > : public TBaseEvent_ID< T >
   {
   public:
      TEvent_ID( const std::string& name )
         : TBaseEvent_ID< T >( name )
         , m_name( name )
      { }
      TEvent_ID( const char* const name )
         : TBaseEvent_ID< T >( name )
         , m_name( name )
      { }
      ~TEvent_ID( ) { }

      const char* c_str( ) const { return m_name.c_str( ); }

   private:
      std::string    m_name;
   };



   template< >
   class TEvent_ID< std::string, true > : public TBaseEvent_ID< std::string >
   {
   public:
      TEvent_ID( const std::string& name ) : TBaseEvent_ID< std::string >( name ) { }
      TEvent_ID( const char* const name ) : TBaseEvent_ID< std::string >( name ) { }
      ~TEvent_ID( ) { }
   };



} // namespace base



namespace base {

   enum class eCommType : size_t { IPC, ITC, ETC };

   class IEventConsumer;

   class Event;
   using EventPtr = std::shared_ptr< Event >;

   // using tID = ID;
   using tID = std::string;
   using Event_ID = TEvent_ID< tID, false >;



} // namespace base
