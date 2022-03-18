#pragma once



namespace carpc {

   using YES = char;
   struct NO { YES m[2]; };

   template< bool b >
   struct Bool2Type
   {
      using type = YES;
   };

   template< >
   struct Bool2Type< false >
   {
      using type = NO;
   };


} // namespace carpc
