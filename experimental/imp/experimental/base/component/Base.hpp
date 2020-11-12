#pragma once

#include <string>
#include <vector>
#include <memory>
#include <boost/asio/io_service.hpp>



namespace fw::component {

   class Base
   {
      public:
         using tCreator = std::unique_ptr< Base > (*)( );
         using tCreatorsVector = std::vector< tCreator >;

      public:
         Base( const std::string& name = "NoName" );
         virtual ~Base( );

      public:
         const std::string& name( ) const;
      private:
         std::string m_name = "NoName";
   };

}
