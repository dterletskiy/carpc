#pragma once

#include "api/sys/common/ID.hpp"



namespace base::async::callable {

   class ICallable
   {
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
         virtual ~ICallable( ) = default;

         virtual void call( ) const = 0;
   };

} // namespace base::async::callable

