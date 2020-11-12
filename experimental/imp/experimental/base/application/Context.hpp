#pragma once

#include <functional>
#include <boost/asio/io_service.hpp>



namespace fw::application {

   class Context
   {
      public:
         using tFunction = std::function< void( void ) >;

      public:
         Context( const std::string& name = "NoName" );
         ~Context( );

         void run( );
         void stop( );
         void post( tFunction function );


      public:
         const std::string& name( ) const;
      private:
         std::string m_name;

      private:
         boost::asio::io_service       m_service;
         boost::asio::io_service::work m_work;
   };

} // namespace fw::application
