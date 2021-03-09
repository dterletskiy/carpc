#pragma once

#include "api/sys/trace/Types.hpp"



namespace base::trace::android {

   extern thread_local char p_buffer[ 1024 ];

} // namespace base::trace::android



#if OS == OS_ANDROID

#define CLASS_ABBR "MAIN"

#define TRACE_ANDROID( LEVEL, USER_FORMAT, ... ) \
   ::sprintf( \
              ::base::trace::android::p_buffer \
            , PREFIX_FORMAT_CODE, CLASS_ABBR, __FUNCTION__, __LINE__ \
   ); \
   __android_log_print( LEVEL, ::base::trace::android::p_buffer, "" USER_FORMAT, ##__VA_ARGS__ );



#undef CLASS_ABBR

#endif // OS == OS_ANDROID
