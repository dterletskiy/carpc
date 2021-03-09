#pragma once

#include "api/sys/command/Types.hpp"
#include "api/sys/command/ICommand.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "CommandQueue"



namespace base::command {

   class Queue
      : public ICommand::Signal::Consumer
   {
      public:
         enum class eState { IDLE, RUNNING, PAUSED, STOPPING, STOPPED };

      public:
         Queue( const std::string& );
         Queue( const Queue& ) = delete;
         ~Queue( );

      public:
         bool start( );
         bool stop( );
         bool pause( );
         bool resume( );
      private:
         eState m_state;

      private:
         void process_event( const typename ICommand::Signal::Event& ) override;

      public:
         const queue::tID& id( ) const;
      private:
         const queue::tID m_id = queue::tID::generate( );

      public:
         const std::string& name( ) const;
      private:
         std::string m_name;

      public:
         template< typename T, typename... Args >
            const command::tID& create_command( const Args&... );
      private:
         bool execute_next( );
      private:
         std::list< ICommand::tUptr > m_commands;
   };



   inline
   const queue::tID& Queue::id( ) const
   {
      return m_id;
   }

   inline
   const std::string& Queue::name( ) const
   {
      return m_name;
   }

   template< typename T, typename... Args >
   const command::tID& Queue::create_command( const Args&... args )
   {
      ICommand::tUptr p_command = std::make_unique< T >( args... );
      p_command->parent_id( m_id );
      m_commands.push_back( p_command );

      execute_next( );

      return p_command->id( );
   }

} // namespace base::events



#undef CLASS_ABBR
