#pragma once

#include "api/sys/comm/event/Types.hpp"



namespace base::async {

   class IAsync
   {
      public:
         using tSptr = std::shared_ptr< IAsync >;

      public:
         struct ISignature
         {
            ISignature( ) = default;
            ISignature( const ISignature& ) = default;
            virtual ~ISignature( ) = default;

            virtual const ISignature* const create_copy( ) const = 0;

            virtual bool operator<( const ISignature& ) const = 0;

            virtual const std::string name( ) const = 0;

            virtual const tAsyncTypeID& type_id( ) const = 0;
            virtual const eAsyncType type( ) const = 0;
         };

         struct IConsumer
         {
            IConsumer( ) = default;
            virtual ~IConsumer( ) = default;
         };

      public:
         IAsync( ) = default;
         virtual ~IAsync( ) = default;

      public:
         virtual void process( IConsumer* p_consumer = nullptr ) const = 0;
         virtual const ISignature* const signature( ) const = 0;
   };

} // namespace base::async

