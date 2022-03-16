#pragma once

#include "api/sys/comm/async/event/Event.hpp"
#include "api/sys/command/Types.hpp"



namespace base::command {

   class Queue;



   class ICommand
   {
      private:
         friend class Queue;

      private:
         enum class eSignal : std::uint8_t { FINISH };
         struct tData
         {
            command::ID id;
            eSignal signal;
         };
         DEFINE_EVENT_S( Signal, tData, base::async::id::TSignature< queue::ID > );

      public:
         using tUptr = std::unique_ptr< ICommand >;
         using tSptr = std::shared_ptr< ICommand >;

      public:
         ICommand( );
         virtual ~ICommand( );

      private:
         virtual void execute( ) = 0;
         virtual void stop( ) = 0;
         virtual void pause( ) = 0;
         virtual void resume( ) = 0;
      protected:
         void finish( );

      public:
         const command::ID& id( ) const;
      private:
         const command::ID m_id = command::ID::generate( );

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
   void ICommand::parent_id( const queue::ID& parent_id )
   {
      m_parent_id = parent_id;
   }

} // namespace base::command
