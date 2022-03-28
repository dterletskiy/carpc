#pragma once

#include <functional>

#include "api/sys/application/Context.hpp"
#include "api/sys/comm/async/Types.hpp"
#include "api/sys/comm/async/IAsync.hpp"



namespace carpc::async {

   class IRunnable
      : public IAsync
      , public std::enable_shared_from_this< IRunnable >
   {
      public:
         using tSptr = std::shared_ptr< IRunnable >;
         using tOperation = std::function< void( void ) >;
         using tOperationPtr = void(*)( void );

      public:
         class Signature : public IAsync::ISignature
         {
            public:
               using tSptr = std::shared_ptr< Signature >;

            private:
               Signature( ) = default;
               Signature( const Signature& other ) = default;
            public:
               ~Signature( ) override = default;
               static tSptr create( )
               {
                  return tSptr( new Signature( ) );
               }

            public:
               const bool to_stream( ipc::tStream& ) const override
               {
                  return false;
               }
               const bool from_stream( ipc::tStream& ) override
               {
                  return false;
               }

               bool operator<( const IAsync::ISignature& signature ) const override
               {
                  return type_id( ) < signature.type_id( );
               }
               const std::string name( ) const override
               {
                  static const std::string s_name = format_string( "type_id: ", type_id( ).c_str( ) );
                  return s_name;
               }
               const tAsyncTypeID& type_id( ) const override final
               {
                  static const tAsyncTypeID s_type_id = tAsyncTypeID::generate< Signature >( );
                  return s_type_id;
               }
         };

      public:
         IRunnable( const tOperation, const tPriority& priority = { } );
         ~IRunnable( ) override = default;

      public:
         /******************************************************************************
          *
          * Sending RUNNABLE async object for execution to appropriate context.
          * Parameters:
          *    to_context - context for execution operation object.
          *    is_block - boolean flag what indicates is sender thread will be locked or not until execution will be finished.
          * In case if 'is_block = true' and 'to_context = current' sender thread will be also
          * destination thread for execution and this will lead to dead lock.
          * For this case there is additional implementation what does not allow to send block runnable object
          * to the same context. Current funtion returns false for this case with appropriate message.
          *
          *****************************************************************************/
         const bool send( const application::Context& to_context, const bool is_block = false );
      private:
         const bool send_to( const application::Context& to_context = application::Context::internal_local );

      private:
         void process( IAsync::IConsumer* ) const override;
         const eAsyncType type( ) const override final;

      private:
         const IAsync::ISignature::tSptr signature( ) const override;
         Signature::tSptr mp_signature = Signature::create( );

      public:
         const tPriority priority( ) const override;
      private:
         tPriority m_priority = { };

      private:
         tOperation m_operation = nullptr;
   };



   inline
   IRunnable::IRunnable( const tOperation operation, const tPriority& priority )
      : m_priority( priority )
      , m_operation( operation )
   {
   }

   inline
   const eAsyncType IRunnable::type( ) const
   {
      return eAsyncType::RUNNABLE;
   }

   inline
   const IAsync::ISignature::tSptr IRunnable::signature( ) const
   {
      return mp_signature;
   }

   inline
   const tPriority IRunnable::priority( ) const
   {
      return m_priority;
   }

} // namespace carpc::async
