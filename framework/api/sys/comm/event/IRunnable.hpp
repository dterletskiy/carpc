#pragma once

#include "api/sys/application/IThread.hpp"
#include "api/sys/comm/event/Types.hpp"
#include "api/sys/comm/event/IAsync.hpp"



namespace base::async {

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
               const bool to_stream( dsi::tByteStream& ) const override
               {
                  return false;
               }
               const bool from_stream( dsi::tByteStream& ) override
               {
                  return false;
               }

               bool operator<( const IAsync::ISignature& signature ) const override
               {
                  if( signature.type_id( ) != type_id( ) )
                     return type_id( ) < signature.type_id( );

                  return type( ) < signature.type( );
               }
               const std::string name( ) const override
               {
                  static const std::string s_name = format_string( "type_id: ", type_id( ).c_str( ), ", type: ", c_str( type( ) ) );
                  return s_name;
               }
               const tAsyncTypeID& type_id( ) const override final
               {
                  static const tAsyncTypeID s_type_id = tAsyncTypeID::generate< Signature >( );
                  return s_type_id;
               }
               const eAsyncType type( ) const override final
               {
                  return eAsyncType::RUNNABLE;
               }
         };

      public:
         IRunnable( const tOperation, const tPriority& priority = { } );
         ~IRunnable( ) override = default;

      public:
         const bool send( );
         const bool send_to_context( application::IThread::tWptr );

      private:
         void process( IAsync::IConsumer* ) const override;

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
   const IAsync::ISignature::tSptr IRunnable::signature( ) const
   {
      return mp_signature;
   }

   inline
   const tPriority IRunnable::priority( ) const
   {
      return m_priority;
   }

} // namespace base::async
