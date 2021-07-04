#pragma once

#include <cstdint>
#include <string>
#include <set>
#include <map>
#include <memory>
#include <optional>
#include <functional>

#include "api/sys/application/Context.hpp"
#include "api/sys/comm/async/event/Event.hpp"
#include "api/sys/fsm/Types.hpp"
#include "api/sys/fsm/TState.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TStateManager"



namespace base::fsm {

   template< typename TYPES >
   class TStateManagerBase
   {
      public:
         DEFINE_EVENT_S( Signal, typename TYPES::tData, base::async::id::TSignature< manager::tID > );
   };



   template< typename TYPES >
   class TStateManager
      : public TStateManagerBase< TYPES >
      , public TStateManagerBase< TYPES >::Signal::Consumer
   {
      private:
         using tBase = TStateManagerBase< TYPES >;
      public:
         using tState = TState< TYPES >;
         using tManager = TStateManager< TYPES >;

      public:
         TStateManager( const std::string& );
         TStateManager( const tManager& ) = delete;
         virtual ~TStateManager( ) = default;

      public:
         bool run( const std::optional < typename TYPES::tID >& state_uid_opt = std::nullopt );
         bool stop( );
         bool pause( );
         bool resume( );
         bool is_started( ) const;
         bool is_running( ) const;
         template< typename StateType, typename ... Args >
            bool register_state( Args& ... args );

      public:
         const manager::tID& id( ) const;
      private:
         manager::tID m_id = manager::tID::generate( );

      public:
         const std::string& name( ) const;
      private:
         std::string m_name;

      private:
         application::Context m_context = application::Context::current( );

      public:
         enum class eStatus : std::uint8_t { CREATED, RUNNING, PAUSED, STOPPED };
         const char* const c_str( const eStatus& ) const;
         const eStatus& status( ) const;
      private:
         eStatus m_status = eStatus::CREATED;

      public:
         void signal( const typename TYPES::tData& data );
      private:
         void process_event( const typename tBase::Signal::Event& ) override;
         void set_state( const std::optional < typename TYPES::tID >& );
      private:
         typename tState::tSptr get_state( const typename TYPES::tID& ) const;
         typename tState::tSptr                                      mp_current_state = nullptr;
         std::map< typename TYPES::tID, typename tState::tSptr >     m_states;

      public:
         struct Subscriber
         {
            using tCallback = std::function< void( const typename state::tID& ) >;
            using tSptr = std::shared_ptr< Subscriber >;
            using tWptr = std::weak_ptr< Subscriber >;

            enum class eAction : std::uint8_t { IN, OUT };

            struct Comparator
            {
               bool operator( )( const tWptr& wp_1, const tWptr& wp_2 ) const
               {
                  auto sp_1 = wp_1.lock( );
                  auto sp_2 = wp_2.lock( );
                  if( !sp_1 && !sp_2 )
                     return false;
                  return sp_1 < sp_2;
               }
            };
            using tWptrSet = std::set< tWptr, Comparator >;

            virtual ~Subscriber( ) = default;

            virtual void state_action( const typename TYPES::tID&, const eAction& ) = 0;
         };
         void subscribe( const typename Subscriber::tSptr );
         void unsubscribe( const typename Subscriber::tSptr );
         void notify( const typename TYPES::tID&, const typename Subscriber::eAction& );
      private:
         typename Subscriber::tWptrSet       m_subscribers;
   };



   template< typename TYPES >
   TStateManager< TYPES >::TStateManager( const std::string& name )
      : m_name( name )
   {
   }

   template< typename TYPES >
   const char* const TStateManager< TYPES >::c_str( const eStatus& status ) const
   {
      switch( status )
      {
         case eStatus::CREATED:  return "CREATED";
         case eStatus::RUNNING:  return "RUNNING";
         case eStatus::PAUSED:   return "PAUSED";
         case eStatus::STOPPED:  return "STOPPED";
         default:                return "UNDEFINED";
      }
      return "UNDEFINED";
   }

   template< typename TYPES >
   const typename manager::tID& TStateManager< TYPES >::id( ) const
   {
      return m_id;
   }

   template< typename TYPES >
   const std::string& TStateManager< TYPES >::name( ) const
   {
      return m_name;
   }

   template< typename TYPES >
   const typename TStateManager< TYPES >::eStatus& TStateManager< TYPES >::status( ) const
   {
      return m_status;
   }

   template< typename TYPES >
   template< typename StateType, typename ... Args >
   bool TStateManager< TYPES >::register_state( Args& ... args )
   {
      if( true == is_started( ) )
      {
         SYS_WRN( "%s: started and new state can't be registered" );
         return false;
      }

      auto p_state = std::make_shared< StateType >( args... );
      p_state->manager( *this );
      auto result = m_states.emplace( std::make_pair( p_state->uid( ), p_state ) );
      if( false == result.second )
      {
         SYS_WRN( "%s: current state was already registered", m_name.c_str( ) );
         return false;
      }

      SYS_VRB( "%s: state '%s' registered", m_name.c_str( ), p_state->name( ).c_str( ) );

      return true;
   }

   template< typename TYPES >
   typename TStateManager< TYPES >::tState::tSptr TStateManager< TYPES >::get_state( const typename TYPES::tID& state_uid ) const
   {
      auto iterator = m_states.find( state_uid );
      if( m_states.end( ) == iterator )
      {
         SYS_WRN( "%s: state wan't found", m_name.c_str( ) );
         return nullptr;
      }
      return iterator->second;      
   }

   template< typename TYPES >
   bool TStateManager< TYPES >::run( const std::optional < typename TYPES::tID >& state_uid_opt )
   {
      if( true == is_started( ) )
      {
         SYS_WRN( "%s: started (%s)", m_name.c_str( ), c_str( m_status ) );
         return false;
      }

      SYS_VRB( "%s: started", m_name.c_str( ) );

      tBase::Signal::Event::set_notification( this, { m_id } );

      SYS_VRB( "%s: running", m_name.c_str( ) );
      m_status = eStatus::RUNNING;

      set_state( state_uid_opt.value_or( m_states.begin( )->first ) );

      return true;
   }

   template< typename TYPES >
   bool TStateManager< TYPES >::stop( )
   {
      if( false == is_started( ) )
      {
         SYS_WRN( "%s: not started (%s)", m_name.c_str( ), c_str( m_status ) );
         return false;
      }

      tBase::Signal::Event::clear_all_notifications( this );

      SYS_VRB( "%s: stopped", m_name.c_str( ) );
      m_status = eStatus::STOPPED;
      return true;
   }

   template< typename TYPES >
   bool TStateManager< TYPES >::pause( )
   {
      if( false == is_running( ) )
      {
         SYS_WRN( "%s: not running (%s)", m_name.c_str( ), c_str( m_status ) );
         return false;
      }

      SYS_VRB( "%s: paused", m_name.c_str( ) );
      m_status = eStatus::PAUSED;
      return true;
   }

   template< typename TYPES >
   bool TStateManager< TYPES >::resume( )
   {
      if( true == is_running( ) )
      {
         SYS_WRN( "%s: running (%s)", m_name.c_str( ), c_str( m_status ) );
         return false;
      }

      SYS_VRB( "%s: running", m_name.c_str( ) );
      m_status = eStatus::RUNNING;
      return true;
   }

   template< typename TYPES >
   bool TStateManager< TYPES >::is_started( ) const
   {
      return eStatus::RUNNING == m_status || eStatus::PAUSED == m_status;
   }

   template< typename TYPES >
   bool TStateManager< TYPES >::is_running( ) const
   {
      return eStatus::RUNNING == m_status;
   }

   template< typename TYPES >
   void TStateManager< TYPES >::set_state( const std::optional < typename TYPES::tID >& state_uid_opt )
   {
      if( !state_uid_opt )
         return;

      const auto state_uid = state_uid_opt.value( );
      if( mp_current_state && state_uid == mp_current_state->uid( ) )
         return;

      auto p_next_state = get_state( state_uid );
      if( nullptr == p_next_state )
         return;

      // Leave current state
      if( mp_current_state )
      {
         mp_current_state->out( );
         notify( mp_current_state->uid( ), Subscriber::eAction::OUT );
      }

      // Enter new state
      mp_current_state = p_next_state;
      const auto next_state_uid_opt = mp_current_state->in( );
      notify( mp_current_state->uid( ), Subscriber::eAction::IN );

      // Set next state in case if entering new state leads to immidiate transition.
      set_state( next_state_uid_opt );
   }

   template< typename TYPES >
   void TStateManager< TYPES >::signal( const typename TYPES::tData& data )
   {
      // Sending signal event to consumer (this manager) placed in context
      // where it has been created
      tBase::Signal::Event::create_send( { m_id }, data, m_context );
   }

   template< typename TYPES >
   void TStateManager< TYPES >::process_event( const typename tBase::Signal::Event& event )
   {
      if( false == is_running( ) )
      {
         SYS_WRN( "%s: not running", m_name.c_str( ) );
         return;
      }
      if( m_id != event.info( ).id( ) )
      {
         SYS_WRN( "%s: not my id", m_name.c_str( ) );
         return;
      }
      if( nullptr == event.data( ) )
      {
         SYS_WRN( "%s: there is no event data", m_name.c_str( ) );
         return;
      }

      set_state( mp_current_state->process( *event.data( ) ) );
   }

   template< typename TYPES >
   void TStateManager< TYPES >::subscribe( const typename Subscriber::tSptr p_subscriber )
   {
      m_subscribers.insert( p_subscriber );
   }

   template< typename TYPES >
   void TStateManager< TYPES >::unsubscribe( const typename Subscriber::tSptr p_subscriber )
   {
      m_subscribers.erase( p_subscriber );
   }

   template< typename TYPES >
   void TStateManager< TYPES >::notify( const typename TYPES::tID& state_uid, const typename Subscriber::eAction& action )
   {
      for( auto iterator = m_subscribers.begin( ); iterator != m_subscribers.end( ); ++iterator )
      {
         if( auto sp_subscriber = iterator->lock( ) )
         {
            sp_subscriber->state_action( state_uid, action );
         }
         else
         {
            iterator = m_subscribers.erase( iterator );
            if( m_subscribers.end( ) == iterator )
               break;
         }
      }
   }

} // namespace base::events



#undef CLASS_ABBR
