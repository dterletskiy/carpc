#pragma once

#include <memory>

#include "api/sys/application/Types.hpp"



namespace base::application {

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
         virtual void boot( const std::string& message = "boot" ) = 0;
         virtual void shutdown( const std::string& message = "shutdown" ) = 0;
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

} // namespace base::application
