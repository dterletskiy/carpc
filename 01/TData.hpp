#pragma once

#include "carpc/runtime/comm/async/Types.hpp"



namespace carpc::async {

   template< typename TYPE >
   struct TData
   {
      const bool to_stream( ipc::tStream& stream ) const
      {
         return true;
      }
      const bool from_stream( ipc::tStream& stream )
      {
         return true;
      }
   };

   using tEmptyData = TData< void >;

} // namespace carpc::async
