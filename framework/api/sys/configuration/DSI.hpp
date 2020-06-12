#pragma once

#include <cstdlib>
#include "api/sys/oswrappers/linux/socket.hpp"



namespace base::configuration::dsi {

   extern const size_t buffer_size;

   extern const os::linux::socket::configuration service_brocker;
   extern const os::linux::socket::configuration application;

}
