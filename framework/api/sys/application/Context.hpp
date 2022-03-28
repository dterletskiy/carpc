#pragma once

#include "api/sys/application/Types.hpp"
#include "api/sys/common/IPC.hpp"



namespace carpc::application {

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
         Context( const thread::ID&, const process::ID& pid = process::current_id( ) );
         Context( const Context& );
         ~Context( ) = default;

      public:
         bool to_stream( ipc::tStream& ) const;
         bool from_stream( carpc::ipc::tStream& );

      public:
         Context& operator=( const Context& );
         bool operator==( const Context& ) const;
         bool operator!=( const Context& ) const;
         bool operator<( const Context& ) const;

      public:
         const std::string name( ) const;

      public:
         bool is_internal( ) const;
         bool is_internal_local( ) const;
         bool is_internal_broadcast( ) const;
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

} // namespace carpc::application
