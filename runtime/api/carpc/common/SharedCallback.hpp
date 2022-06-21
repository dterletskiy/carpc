#pragma once

#include <functional>
#include <memory>

#include "carpc/common/ID.hpp"
#include "carpc/common/Name.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "Callback"



namespace carpc::callback {

   class SharedCallback;
   using ID = carpc::TID< SharedCallback >;
   using Name = carpc::TName< SharedCallback >;

   class SharedCallback
   {
      protected:
         SharedCallback( )
         {
            SYS_VRB( "'%s': created", m_id.name( ).c_str( ) );
         }
      public:
         virtual ~SharedCallback( )
         {
            SYS_VRB( "'%s': destroyed", m_id.name( ).c_str( ) );
         }

      public:
         const ID& id( ) const
         {
            return m_id;
         }
      private:
         ID m_id = ID::generate( );
   };



   template< typename DATA >
   class TSharedCallback : public SharedCallback
   {
      public:
         using tObject = TSharedCallback< DATA >;
         using tSptr = std::shared_ptr< tObject >;
         using tWptr = std::weak_ptr< tObject >;
         using tData = DATA;
         using tFinalizer = std::function< void( const tData& ) >;

      private:
         TSharedCallback( tFinalizer finalizer, tData data )
            : m_finalizer( finalizer )
            , m_data( data )
         { }
         template< typename... Args >
         TSharedCallback( tFinalizer finalizer, Args... args )
            : m_finalizer( finalizer )
            , m_data( args... )
         { }
      public:
         ~TSharedCallback( ) override
         {
            m_finalizer( m_data );
         }

      public:
         static tSptr create( tFinalizer finalizer, tData data )
         {
            return tSptr( new tObject( finalizer, data ) );
         }

         template< typename... Args >
         static tSptr create( tFinalizer finalizer, Args... args )
         {
            return tSptr( new tObject( finalizer, args... ) );
         }

      private:
         const tFinalizer  m_finalizer;
         const tData       m_data;
   };



   template< >
   class TSharedCallback< void > : public SharedCallback
   {
      public:
         using tObject = TSharedCallback< void >;
         using tSptr = std::shared_ptr< tObject >;
         using tWptr = std::weak_ptr< tObject >;
         using tFinalizer = std::function< void( ) >;

      private:
         TSharedCallback( tFinalizer finalizer )
            : m_finalizer( finalizer )
         { }
      public:
         ~TSharedCallback( ) override
         {
            m_finalizer( );
         }

      public:
         static tSptr create( tFinalizer finalizer )
         {
            return tSptr( new tObject( finalizer ) );
         }

         static bool is_locked( const tWptr& wp )
         {
            return 0 != wp.use_count( );
         }

         static bool is_unlocked( const tWptr& wp )
         {
            return 0 == wp.use_count( );
         }

      private:
         const tFinalizer  m_finalizer;
   };



   using tBlockerRoot = TSharedCallback< void >;
   using tBlocker = tBlockerRoot::tSptr;
   using tBlockerLink = tBlockerRoot::tWptr;

} // namespace carpc::callback



#undef CLASS_ABBR
