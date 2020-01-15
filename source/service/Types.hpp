#pragma once

#include "common/Types.hpp"



namespace base {

   class IServiceProcess;
   using IServiceProcessPtr   = std::shared_ptr< IServiceProcess >;
   using IServiceProcessPtrW  = std::weak_ptr< IServiceProcess >;
   extern const IServiceProcessPtr InvalidServicePtrPtr;

   class IServiceBrocker;
   using IServiceBrockerPtr   = std::shared_ptr< IServiceBrocker >;
   using IServiceBrockerPtrW  = std::weak_ptr< IServiceBrocker >;
   extern const IServiceBrockerPtr InvalidServiceBrockerPtr;

   class IServiceBrockerDSI;
   using IServiceBrockerDsiPtr   = std::shared_ptr< IServiceBrockerDSI >;
   using IServiceBrockerDsiPtrW  = std::weak_ptr< IServiceBrockerDSI >;
   extern const IServiceBrockerDsiPtr InvalidServiceBrockerDsiPtr;

   class IService;
   using IServicePtr          = std::shared_ptr< IService >;
   using IServicePtrList      = std::list< IServicePtr >;
   using IServicePtrW         = std::weak_ptr< IService >;
   extern const IServicePtr InvalidServicePtr;



   class IComponent;
   using ComponentCreator = std::shared_ptr< IComponent > (*)( std::shared_ptr< IService > );
   // typedef IComponentPtr ( *ComponentCreator )( IServicePtr );
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
