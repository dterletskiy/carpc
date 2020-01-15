#pragma once

#include "common/Types.hpp"



namespace base {



class Stub
{
public:
   Stub( const std::string&, const std::string& );
   ~Stub( );

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
const std::string& Stub::interface_name( ) const
{
   return m_interface_name;
}

inline
const std::string& Stub::service_name( ) const
{
   return m_service_name;
}



} // namespace base
