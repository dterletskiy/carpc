#pragma once

#include "api/sys/comm/event/Types.hpp"
#include "api/sys/comm/event/TEvent.hpp"



namespace base {



/****************************************
 *
 * TGenerator declaration
 *
 ***************************************/
template< typename _DataType, typename _IdType, typename _EventNamespace, typename _ServiceType = NoServiceType >
class TGenerator
{
private:
   using _Generator     = TGenerator< _DataType, _IdType, _EventNamespace, _ServiceType >;
   using _EventType     = TEvent< _Generator >;
   using _ConsumerType  = TEventConsumer< _Generator >;

public:
   struct Config
   {
      using EventType      = _EventType;
      using DataType       = _DataType;
      using IdType         = _IdType;
      using ConsumerType   = _ConsumerType;
      using ServiceType    = _ServiceType;
   };
};



} // namespace base
