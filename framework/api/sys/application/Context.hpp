#pragma once

#include "api/sys/common/Includes.hpp"
#include "api/sys/application/Types.hpp"
#include "api/sys/dsi/Types.hpp"



namespace base::application {

   class Context
   {
      public:
         using tOpt = std::optional< Context >;
         using tVector = std::vector< Context >;
         using tList = std::list< Context >;
         using tSet = std::set< Context >;

         enum class eInitType : std::uint8_t { INTERNAL_BROADCAST, INTERNAL_LOCAL, CURRENT, INVALID };

      public:
         static const Context invalid;
         static const Context internal_broadcast;
         static const Context internal_local;
         static Context current( );

      public:
         Context( const eInitType init_type );
         Context( const process::ID&, const thread::ID& );
         Context( const Context& );
         ~Context( ) = default;

      public:
         bool to_stream( base::dsi::tByteStream& ) const;
         bool from_stream( base::dsi::tByteStream& );

      public:
         Context& operator=( const Context& );
         bool operator==( const Context& ) const;
         bool operator!=( const Context& ) const;
         bool operator<( const Context& ) const;

      public:
         const std::string name( ) const;

      public:
         bool is_local( ) const;
         bool is_external( ) const;
         bool is_valid( ) const;

      public:
         const process::ID& pid( ) const;
         const thread::ID& tid( ) const;
      private:
         process::ID m_pid = process::ID::invalid;
         thread::ID m_tid = thread::ID::invalid;
   };



   inline
   const process::ID& Context::pid( ) const
   {
      return m_pid;
   }

   inline
   const thread::ID& Context::tid( ) const
   {
      return m_tid;
   }

} // namespace base::application
