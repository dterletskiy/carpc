#pragma once

#include <vsomeip/vsomeip.hpp>



#define SAMPLE_SERVICE_ID       0x1234
#define SAMPLE_INSTANCE_ID      0x5678
#define SAMPLE_METHOD_ID        0x0421

#define SAMPLE_EVENT_ID         0x8778
#define SAMPLE_GET_METHOD_ID    0x0001
#define SAMPLE_SET_METHOD_ID    0x0002

#define SAMPLE_EVENTGROUP_ID    0x4465

#define OTHER_SAMPLE_SERVICE_ID 0x0248
#define OTHER_SAMPLE_INSTANCE_ID 0x5422
#define OTHER_SAMPLE_METHOD_ID  0x1421



namespace hybrid::interface::someip {

   class Server
   {
      public:
         Server( );
         ~Server( );

         bool init( );
         void start( );
         void stop( );
         void on_state( vsomeip::state_type_e state );
         void on_message( const std::shared_ptr< vsomeip::message >& message );

      private:
         std::shared_ptr< vsomeip::application > m_app;
         bool m_is_registered = false;

      private:
         void response( const std::shared_ptr< vsomeip::message >& _message );
   };

} // namespace hybrid::interface::someip
