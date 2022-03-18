#pragma once

#include "api/sys/command/ICommand.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "CommandQueue"



namespace carpc::command {

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
         const queue::ID& id( ) const;
      private:
         const queue::ID m_id = queue::ID::generate( );

      public:
         const queue::Name& name( ) const;
      private:
         queue::Name m_name;

      public:
         template< typename T, typename... Args >
            const command::ID& create_command( const Args&... );
      private:
         bool execute_next( );
      private:
         std::list< ICommand::tUptr > m_commands;
   };



   inline
   const queue::ID& Queue::id( ) const
   {
      return m_id;
   }

   inline
   const queue::Name& Queue::name( ) const
   {
      return m_name;
   }

   template< typename T, typename... Args >
   const command::ID& Queue::create_command( const Args&... args )
   {
      ICommand::tUptr p_command = std::make_unique< T >( args... );
      p_command->parent_id( m_id );
      m_commands.push_back( p_command );

      execute_next( );

      return p_command->id( );
   }

} // namespace carpc::events



#undef CLASS_ABBR
