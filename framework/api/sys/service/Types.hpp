#pragma once

#include "api/sys/common/Types.hpp"



namespace base {

   class ServiceProcess;
   using ServiceProcessPtr    = std::shared_ptr< ServiceProcess >;
   using ServiceProcessPtrW   = std::weak_ptr< ServiceProcess >;
   extern const ServiceProcessPtr InvalidServicePtrPtr;

   class ServiceBrocker;
   using ServiceBrockerPtr    = std::shared_ptr< ServiceBrocker >;
   using ServiceBrockerPtrW   = std::weak_ptr< ServiceBrocker >;
   extern const ServiceBrockerPtr InvalidServiceBrockerPtr;

   class ServiceBrockerDSI;
   using ServiceBrockerDsiPtr   = std::shared_ptr< ServiceBrockerDSI >;
   using ServiceBrockerDsiPtrW  = std::weak_ptr< ServiceBrockerDSI >;
   extern const ServiceBrockerDsiPtr InvalidServiceBrockerDsiPtr;

   class Service;
   using ServicePtr           = std::shared_ptr< Service >;
   using ServicePtrList       = std::list< ServicePtr >;
   using ServicePtrW          = std::weak_ptr< Service >;
   extern const ServicePtr InvalidServicePtr;



   class Component;
   using ComponentCreator = std::shared_ptr< Component > (*)( std::shared_ptr< Service > );
   using ComponentCreatorVector = std::vector< ComponentCreator >;
   struct ServiceInfo
   {
      std::string             m_name;
      ComponentCreatorVector  m_component_creators;
      size_t                  m_wd_timeout;
   };
   using ServiceInfoList = std::list< ServiceInfo >;
   using ServiceInfoVector = std::vector< ServiceInfo >;

} // namespace base
