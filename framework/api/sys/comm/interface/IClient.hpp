#pragma once



namespace base {

   class IClient
   {
      public:
         IClient( );
         virtual ~IClient( );

      public:
         virtual void connected( ) = 0;
         virtual void disconnected( ) = 0;
   };


} // namespace base
