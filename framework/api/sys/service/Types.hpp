#pragma once

#include "api/sys/common/Types.hpp"



namespace base {

   class ServiceProcess;
   using ServiceProcessPtr                = std::shared_ptr< ServiceProcess >;
   using ServiceProcessPtrW               = std::weak_ptr< ServiceProcess >;
   extern const ServiceProcessPtr         InvalidServicePtrPtr;

   class ServiceBrockerThread;
   using ServiceBrockerThreadPtr          = std::shared_ptr< ServiceBrockerThread >;
   using ServiceBrockerThreadPtrW         = std::weak_ptr< ServiceBrockerThread >;
   extern const ServiceBrockerThreadPtr   InvalidServiceBrockerThreadPtr;

   class ServiceThread;
   using ServiceThreadPtr                 = std::shared_ptr< ServiceThread >;
   using ServiceThreadPtrList             = std::list< ServiceThreadPtr >;
   using ServiceThreadPtrW                = std::weak_ptr< ServiceThread >;
   extern const ServiceThreadPtr          InvalidServiceThreadPtr;



   class Component;
   using ComponentCreator                 = std::shared_ptr< Component > (*)( std::shared_ptr< ServiceThread > );
   using ComponentCreatorVector           = std::vector< ComponentCreator >;
   struct ServiceInfo
   {
      std::string             m_name;
      ComponentCreatorVector  m_component_creators;
      size_t                  m_wd_timeout;
   };
   using ServiceInfoList                  = std::list< ServiceInfo >;
   using ServiceInfoVector                = std::vector< ServiceInfo >;

} // namespace base
