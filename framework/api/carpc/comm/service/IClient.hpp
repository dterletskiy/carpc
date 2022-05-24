#pragma once



namespace carpc::service {

   class IClient
   {
      public:
         IClient( ) = default;
         virtual ~IClient( ) = default;

      public:
         virtual void connected( ) = 0;
         virtual void disconnected( ) = 0;
   };


} // namespace carpc::service
