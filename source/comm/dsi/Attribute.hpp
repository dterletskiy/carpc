#pragma once

#include "common/Types.hpp"



namespace base {



template< typename T >
class Attribute
{
public:
   Attribute( ) { };
   ~Attribute( ) { };

public:
   bool is_valid( ) const;
   const T& value( ) const;
   void value( const T& );
private:
   std::optional< T > m_value;
};



template< typename T >
bool Attribute< T >::is_valid( ) const
{
   return std::nullopt != m_value;
}

template< typename T >
const T& Attribute< T >::value( ) const
{
   return m_value.value_or( { } );
}

template< typename T >
void Attribute< T >::value( const T& _value )
{
   m_value = _value;
}




} // namespace base
