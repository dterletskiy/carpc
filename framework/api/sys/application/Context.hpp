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

         enum class eInitType : std::uint8_t { Default, Auto };

      public:
         struct process
         {
            using tID = application::process::ID;
            static const tID broadcast;
            static const tID local;
         };
         struct thread
         {
            using tID = application::thread::ID;
            static const tID broadcast;
            static const tID local;
         };

         static const Context& invalid( )
         {
            static Context context( process::tID::invalid( ), thread::tID::invalid( ) );
            return context;
         }

      public:
         Context( const eInitType init_type = eInitType::Default );
         Context( const process::tID& );
         Context( const thread::tID& );
         Context( const process::tID&, const thread::tID& );
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
         const process::tID& pid( ) const;
         const thread::tID& tid( ) const;
      private:
         process::tID m_pid = process::local;
         thread::tID m_tid = thread::broadcast;
   };



   inline
   const Context::process::tID& Context::pid( ) const
   {
      return m_pid;
   }

   inline
   const Context::thread::tID& Context::tid( ) const
   {
      return m_tid;
   }

} // namespace base::application
