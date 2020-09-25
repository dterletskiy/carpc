#pragma once

#include "api/sys/service/IServiceThread.hpp"
#include "api/sys/comm/event/Types.hpp"
#include "api/sys/comm/event/IAsync.hpp"



namespace base {

   class IRunnable
      : public IAsync
      , public std::enable_shared_from_this< IRunnable >
   {
      public:
         using tSptr = std::shared_ptr< IRunnable >;
         using tOperation = std::function< void( void ) >;
         using tOperationPtr = void(*)( void );

      public:
         struct Signature : public IAsync::ISignature
         {
            Signature( ) = default;
            Signature( const Signature& other ) = default;
            ~Signature( ) override = default;

            const IAsync::ISignature* const create_copy( ) const override { return new Signature( *this ); }
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
         IRunnable( const tOperation );
         ~IRunnable( ) override = default;

      public:
         const bool send( );
         const bool send_to_context( IServiceThread::tWptr );

      private:
         void process( IAsync::IConsumer* ) const override;

      private:
         const Signature* const signature( ) const override;
         Signature m_signature;

      private:
         tOperation m_operation = nullptr;
   };



   inline
   IRunnable::IRunnable( const tOperation operation )
      : m_operation( operation )
   {
   }

   inline
   const IRunnable::Signature* const IRunnable::signature( ) const
   {
      return &m_signature;
   }

} // namespace base
