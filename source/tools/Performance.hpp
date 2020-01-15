#pragma once

#include "common/Types.hpp"

namespace base::tools {



class Performance
{
public:
   Performance( const std::string& );
   ~Performance( );

   void start( );
   void stop( );

private:
   std::string                                                             m_name;
   std::optional< std::chrono::time_point< std::chrono::system_clock > >   m_start;
   std::optional< std::chrono::time_point< std::chrono::system_clock > >   m_finish;
   std::optional< std::chrono::microseconds >                              m_delta;
};



} // namespace base::tools
