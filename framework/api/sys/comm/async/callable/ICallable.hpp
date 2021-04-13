#pragma once

#include "api/sys/application/Context.hpp"
#include "api/sys/comm/async/Types.hpp"
#include "api/sys/comm/async/IAsync.hpp"



namespace base::async {

   class ICallable
      : public IAsync
      , public std::enable_shared_from_this< ICallable >
   {
      public:
         using tSptr = std::shared_ptr< ICallable >;

      protected:
         template< int ... >
         struct sequence { };

         template< int N, int ...S >
         struct gen_sequence : gen_sequence< N - 1, N - 1, S... > { };

         template< int ...S >
         struct gen_sequence< 0, S... >
         {
            typedef sequence< S... > type;
         };

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
         virtual ~ICallable( ) = default;

      public:
         const bool send( const application::Context& to_context = application::Context::internal_local );

      public:
         virtual void call( ) const = 0;
      private:
         void process( IConsumer* p_consumer = nullptr ) const override;
         const eAsyncType type( ) const override final;

      private:
         const IAsync::ISignature::tSptr signature( ) const override;
         Signature::tSptr mp_signature = Signature::create( );

      public:
         const tPriority priority( ) const override;
      private:
         tPriority m_priority = { };
   };



   inline
   const eAsyncType ICallable::type( ) const
   {
      return eAsyncType::CALLABLE;
   }

   inline
   const IAsync::ISignature::tSptr ICallable::signature( ) const
   {
      return mp_signature;
   }

   inline
   const tPriority ICallable::priority( ) const
   {
      return m_priority;
   }

} // namespace base::async

