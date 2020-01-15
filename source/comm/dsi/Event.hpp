#pragma once

#include "common/Types.hpp"



namespace base {



class Event
{
public:
   Event( const std::string&, const std::string& );
   ~Event( );

public:
   const std::string& interface_name( ) const;
private:
   std::string m_interface_name;

public:
   const std::string& service_name( ) const;
private:
   std::string m_service_name;
};



inline
const std::string& Event::interface_name( ) const
{
   return m_interface_name;
}

inline
const std::string& Event::service_name( ) const
{
   return m_service_name;
}



} // namespace base
