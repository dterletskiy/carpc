#pragma once

#include <string>
#include <memory>

#include "carpc/common/Priority.hpp"
#include "carpc/comm/async/Types.hpp"



namespace carpc::async {

   class IAsync
   {
      public:
         using tSptr = std::shared_ptr< IAsync >;

      public:
         struct ISignature
         {
            using tSptr = std::shared_ptr< ISignature >;

            ISignature( ) = default;
            ISignature( const ISignature& ) = default;
            virtual ~ISignature( ) = default;

            virtual const bool to_stream( ipc::tStream& ) const = 0;
            virtual const bool from_stream( ipc::tStream& ) = 0;

            virtual bool operator<( const ISignature& ) const = 0;

            virtual const std::string dbg_name( ) const = 0;

            virtual const tAsyncTypeID& type_id( ) const = 0;
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
         virtual const ISignature::tSptr signature( ) const = 0;
         virtual const tPriority priority( ) const = 0;
         virtual const eAsyncType type( ) const = 0;
   };

} // namespace carpc::async

