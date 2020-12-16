#pragma once

#include "imp/experimental/base/event/IInfo.hpp"



namespace fw::event {

   template< typename _Common >
   class TInfo : public IInfo
   {
      public:
         TInfo( const tID& id ) : m_id( id ) { }
         TInfo( const TInfo< _Common >& other ) : m_id( other.m_id ) { }
         ~TInfo( ) = default;

         bool operator<( const IInfo& other ) const override { return m_id < static_cast< const TInfo< _Common >& >( other ).m_id; }

         const std::string to_string( ) const override { return std::to_string( m_id ); }

         virtual const std::shared_ptr< IInfo > duplicate( ) const { return std::make_shared< TInfo< _Common > >( m_id ); }

      public:
         const tID& id( ) const { return m_id; }
      private:
         tID m_id;
   };

} // namespace fw::event
