#pragma once

#include "api/sys/comm/Types.hpp"



namespace base {



template< typename TYPE >
EventPtr create_event( const eCommType comm_type = eCommType::IPC )
{
   return std::make_shared< TYPE >( comm_type );
}

template< typename TYPE >
EventPtr create_event( const typename TYPE::_DataType data, const eCommType comm_type = eCommType::IPC )
{
   return std::make_shared< TYPE >( data, comm_type );
}



template< typename TYPE >
EventPtr create_event( const typename TYPE::_IdType id, const eCommType comm_type = eCommType::IPC )
{
   return std::make_shared< TYPE >( id, comm_type );
}

template< typename TYPE >
EventPtr create_event( const typename TYPE::_DataType data, const typename TYPE::_IdType id, const eCommType comm_type = eCommType::IPC )
{
   return std::make_shared< TYPE >( data, id, comm_type );
}



} // namespace base
