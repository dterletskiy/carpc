#pragma once

#include <memory>

#include "api/sys/common/SharedCallback.hpp"
#include "api/sys/application/Types.hpp"



namespace carpc::application {

   class IComponent
   {
      public:
         using tSptr = std::shared_ptr< IComponent >;
         using tSptrList = std::list< tSptr >;
         using tCreator = IComponent::tSptr (*)( );
         using tCreatorVector = std::vector< tCreator >;

      public:
         IComponent( const std::string& );
         virtual ~IComponent( ) = default;

      public:
         virtual const bool is_root( ) const = 0;

      public:
         const component::ID& id( ) const;
         const component::Name& name( ) const;
      protected:
         component::ID     m_id = component::ID::generate( );
         component::Name   m_name{ "NoName_Component" };

      public:
         virtual void process_boot( const std::string& message = "process_boot" ) = 0;
         virtual void process_shutdown( carpc::callback::tBlocker ) { }
   };



   inline
   const component::Name& IComponent::name( ) const
   {
      return m_name;
   }

   inline
   const component::ID& IComponent::id( ) const
   {
      return m_id;
   }

} // namespace carpc::application
