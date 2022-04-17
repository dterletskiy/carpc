#pragma once

#include <type_traits>

#ifdef USE_GPB
   #include <google/protobuf/message.h>
#endif

#include "api/sys/common/Types.hpp"



#define CARPC_IS_SAME_TYPES( TYPE_A, TYPE_B )         std::is_same_v< TYPE_A, TYPE_B >

#define CARPC_IS_INTEGRAL_TYPE( TYPE )                std::is_integral_v< TYPE >
#define CARPC_IS_FLOATING_POINT_TYPE( TYPE )          std::is_floating_point_v< TYPE >
#define CARPC_IS_ENUM_TYPE( TYPE )                    std::is_enum_v< TYPE >

#define CARPC_IS_IPC_TYPE( TYPE )                     CARPC_IS_SAME_TYPES( carpc::IPC, TYPE )
#define CARPC_IS_NO_IPC_TYPE( TYPE )                  CARPC_IS_SAME_TYPES( carpc::NO_IPC, TYPE )

#ifdef USE_GPB
   #define CARPC_IS_GPB_TYPE( TYPE )                  std::is_base_of_v< ::google::protobuf::Message, TYPE >
#else
   #define CARPC_IS_GPB_TYPE( TYPE )                  std::is_base_of_v< TYPE, carpc::Dummy >
#endif



#define CARPC_ENABLE_IF_SAME_TYPES( TYPE_A, TYPE_B )  std::enable_if_t< CARPC_IS_SAME_TYPES( TYPE_A, TYPE_B ) >

#define CARPC_ENABLE_IF_INTEGRAL_TYPE( TYPE )         std::enable_if_t< CARPC_IS_INTEGRAL_TYPE( TYPE ) >
#define CARPC_ENABLE_IF_FLOATING_POINT_TYPE( TYPE )   std::enable_if_t< CARPC_IS_FLOATING_POINT_TYPE( TYPE ) >
#define CARPC_ENABLE_IF_ENUM_TYPE( TYPE )             std::enable_if_t< CARPC_IS_ENUM_TYPE( TYPE ) >

#define CARPC_ENABLE_IF_IPC_TYPE( TYPE )              std::enable_if_t< CARPC_IS_IPC_TYPE( TYPE ) >
#define CARPC_ENABLE_IF_NO_IPC_TYPE( TYPE )           std::enable_if_t< CARPC_IS_NO_IPC_TYPE( TYPE ) >

#define CARPC_ENABLE_IF_GPB_TYPE( TYPE )              std::enable_if_t< CARPC_IS_GPB_TYPE( TYPE ) >
