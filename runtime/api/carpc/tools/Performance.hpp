#pragma once

#include <string>
#include <optional>
#include <chrono>



namespace carpc::tools {



   class Performance
   {
      public:
         Performance( const std::string& );
         ~Performance( );

         void start( const std::string& message = "" );
         void stop( const std::string& message = "" );
         long int info( ) const;

      private:
         std::string                                                             m_name;
         std::optional< std::chrono::time_point< std::chrono::system_clock > >   m_start;
         std::optional< std::chrono::time_point< std::chrono::system_clock > >   m_finish;
         std::optional< std::chrono::microseconds >                              m_delta;
   };



} // namespace carpc::tools
