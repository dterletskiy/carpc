#pragma once

#include "api/sys/common/SharedCallback.hpp"
#include "api/sys/comm/async/event/Event.hpp"
#include "api/sys/command/Types.hpp"



namespace carpc::command {

   class Queue;



   class ICommand : public std::enable_shared_from_this< ICommand >
   {
      private:
         friend class Queue;

      private:
         enum class eAction : std::uint8_t
         {
            EXECUTE,
            PAUSE,
            RESUME,
            STOP
         };
         const char* c_str( const eAction& action );

      private:
         struct tData
         {
            command::ID id;
            command::eState state;
         };
         DEFINE_EVENT_S( Signal, tData, carpc::async::id::TSignature< queue::ID > );

      public:
         using tUptr = std::unique_ptr< ICommand >;
         using tSptr = std::shared_ptr< ICommand >;
         using tWptr = std::weak_ptr< ICommand >;

      public:
         ICommand( );
         virtual ~ICommand( );

      private:
         void action( const eAction& );
         // Execute blocker must be locked by user untill the command will not finish its suggested activities.
         // When command finishes its activity without stoping it, it must release this blocker
         // and its state will be changed to FINISHED.
         // After this command is ready to be deleted.
         virtual void execute( callback::tBlocker ) = 0;
         // Pause blocker must be locked by user untill the command will not finish pausing process
         // requested by this call.
         // When this blocker is released state of current command will be changed to PAUSED.
         // This suggests that command will be freezed and will not performe any activity.
         virtual void pause( callback::tBlocker ) = 0;
         // Pause blocker must be locked by user untill the command will not finish resuming process
         // requested by this call.
         // When this blocker is released state of current command will be changed to EXECUTING.
         // This suggests that command will continue all its activities from the point where it was paused.
         virtual void resume( callback::tBlocker ) = 0;
         // Pause blocker must be locked by user untill the command will not finish pausing process
         // requested by this call.
         // When this blocker is released state of current command will be changed to PAUSED.
         // This suggests that command will be stoped and will not performe any activity.
         // After this command is ready to be deleted.
         virtual void stop( callback::tBlocker ) = 0;

      public:
         const command::ID& id( ) const;
      private:
         const command::ID m_id = command::ID::generate( );

      public:
         const command::eState& state( ) const;
      private:
         bool state( const command::eState& );
         command::eState m_state = command::eState::IDLE;

      private:
         void parent_id( const queue::ID& );
         queue::ID m_parent_id = queue::ID::invalid;
   };



   inline
   const command::ID& ICommand::id( ) const
   {
      return m_id;
   }

   inline
   const command::eState& ICommand::state( ) const
   {
      return m_state;
   }

   inline
   void ICommand::parent_id( const queue::ID& parent_id )
   {
      m_parent_id = parent_id;
   }

} // namespace carpc::command
