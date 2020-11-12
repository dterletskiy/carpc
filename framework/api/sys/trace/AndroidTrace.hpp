#pragma once

#if OS == ANDROID

#include <stdio.h>
#include <android/log.h>



#define CLASS_ABBR "MAIN"



namespace base::trace::android {

   namespace simple {

      void dbg_lock( );
      void dbg_unlock( );

      extern char p_buffer[ 1024 ];

   }

} // namespace base::trace::android



#define DBG_ANDROID( LEVEL, USER_FORMAT, ... ) \
   ::base::trace::write::simple::dbg_lock( ); \
   ::sprintf( \
              ::base::trace::android::simple::p_buffer \
            , PREFIX_FORMAT_CODE, CLASS_ABBR, __FUNCTION__, __LINE__ \
   ); \
   __android_log_print( LEVEL, ::base::trace::android::simple::p_buffer, " " USER_FORMAT, ##__VA_ARGS__ ); \
   ::base::trace::write::simple::dbg_unlock( );



#undef CLASS_ABBR

#endif // OS == ANDROID
