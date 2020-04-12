#pragma once

#include "api/sys/service/IServiceThread.hpp"
#include "api/sys/comm/event/Types.hpp"
#include "api/sys/comm/event/IAsync.hpp"



namespace base {



class IRunnable
   : public IAsync
   , public std::enable_shared_from_this< IRunnable >
{
public:
   using tSptr = std::shared_ptr< IRunnable >;
   using tOperation = std::function< void( void ) >;
   using tOperationPtr = void(*)( void );

public:
   struct Signature : public IAsync::ISignature
   {
      Signature( ) = default;
      Signature( const Signature& other ) = default;
      ~Signature( ) override = default;

      const IAsync::ISignature* const create_copy( ) const override { return new Signature( *this ); }
      bool operator<( const IAsync::ISignature& signature ) const override
      {
         if( signature.type_id( ) != s_type_id )
            return s_type_id < signature.type_id( );

         return s_type_event < signature.type( );
      }
      const std::string name( ) const override
      {
         static const std::string s_name = base::format_string( s_type_id, "/", base::c_str( s_type_event ) );
         return s_name;
      }
      const EventTypeID& type_id( ) const override final { return s_type_id; }
      const eEventType type( ) const override final { return s_type_event; }

      static const EventTypeID   s_type_id;
      static const eEventType    s_type_event;
   };

public:
   IRunnable( const tOperation );
   ~IRunnable( ) override = default;

public:
   const bool send( );
   const bool send_to_context( IServiceThread::tWptr );

private:
   void process( IAsync::IConsumer* ) const override;

private:
   const Signature* const signature( ) const override;
   Signature m_signature;

private:
   tOperation m_operation = nullptr;
};



inline
IRunnable::IRunnable( const tOperation operation )
   : m_operation( operation )
{
}

inline
const IRunnable::Signature* const IRunnable::signature( ) const
{
   return &m_signature;
}



} // namespace base
