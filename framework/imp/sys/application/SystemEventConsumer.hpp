#pragma once

#include "api/sys/events/Events.hpp"



namespace carpc::application {

   class IThread;



   class SystemEventConsumer : public events::system::System::Consumer
   {
      public:
         SystemEventConsumer( IThread& );
         ~SystemEventConsumer( ) override;
      private:
         SystemEventConsumer( const SystemEventConsumer& ) = delete;
         SystemEventConsumer& operator=( const SystemEventConsumer& ) = delete;

      private:
         IThread& m_app_thread;

      private:
         void process_event( const events::system::System::Event& ) override;
   };

} // namespace carpc::application
