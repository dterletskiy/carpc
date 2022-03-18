#pragma once

#include <functional>
#include <memory>



namespace carpc {



template< typename DATA >
class SharedCallback
{
public:
   using Data = DATA;
   using Finalizer = std::function< void( const Data& ) >;

public:
   SharedCallback( Data data, Finalizer finalizer )
      : m_data( data )
      , m_finalizer( finalizer )
   { }

   ~SharedCallback( )
   {
      m_finalizer( m_data );
   }

private:
   const Finalizer   m_finalizer;
   Data              m_data;
};



template< >
class SharedCallback< void >
{
public:
   using Finalizer = std::function< void( ) >;

public:
   SharedCallback( Finalizer finalizer )
      : m_finalizer( finalizer )
   { }

   ~SharedCallback( )
   {
      m_finalizer( );
   }

   static std::shared_ptr< SharedCallback< void > > create( Finalizer finalizer )
   {
      return std::make_shared< SharedCallback< void > >( finalizer );
   }

private:
   const Finalizer   m_finalizer;
};



/// Blocker is reference-counter mechanism, used in asyncronous parts of logic
/// Entity, receives this Blocker may yeld Blocker owner lifetime.
using Blocker = std::shared_ptr< void >;

/// Shared callback with no data, used as Blocker with defined Final Action
using BlockerRoot = SharedCallback< void >;



} // namespace carpc
