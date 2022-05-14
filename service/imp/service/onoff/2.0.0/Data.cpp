#include "Data.hpp"

namespace service::onoff::V2_0_0 {

   namespace method {

      BaseData::tSptr BaseData::create( carpc::ipc::tStream& stream )
      {
         eID id = eID::UNDEFINED;
         carpc::service::eType type = carpc::service::eType::UNDEFINED;
         if( false == carpc::ipc::deserialize( stream, id, type ) )
            return nullptr;

         tSptr ptr = nullptr;
         switch( id )
         {
            case eID::trigger_state:
            {
               switch( type )
               {
                  case carpc::service::eType::REQUEST:
                  {
                     ptr = std::make_shared< request::trigger_stateData >( );
                     break;
                  }
                  case carpc::service::eType::RESPONSE:
                  {
                     ptr = std::make_shared< response::trigger_stateData >( );
                     break;
                  }
                  default: break;
               }
               break;
            }
            case eID::start:
            {
               switch( type )
               {
                  case carpc::service::eType::REQUEST:
                  {
                     ptr = std::make_shared< request::startData >( );
                     break;
                  }
                  default: break;
               }
               break;
            }
            default: break;
         }

         if( nullptr != ptr )
            ptr->from_stream( stream );

         return  ptr;
      }

      bool BaseData::serrialize( carpc::ipc::tStream& stream )
      {
         if( false == carpc::ipc::serialize( stream, id( ), type( ) ) )
            return false;
         return to_stream( stream );
      }

      namespace request {

         trigger_stateData::trigger_stateData( const std::string& _state, const std::size_t& _timeout )
            : BaseData( )
            , state( _state)
            , timeout( _timeout)
         {
         }

         bool trigger_stateData::to_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::serialize( stream, state, timeout );
         }

         bool trigger_stateData::from_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::deserialize( stream, state, timeout );
         }

      } // namespace request

      namespace response {

         trigger_stateData::trigger_stateData( const bool& _result )
            : BaseData( )
            , result( _result)
         {
         }

         bool trigger_stateData::to_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::serialize( stream, result );
         }

         bool trigger_stateData::from_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::deserialize( stream, result );
         }

      } // namespace response

      namespace request {

         bool startData::to_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::serialize( stream );
         }

         bool startData::from_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::deserialize( stream );
         }

      } // namespace request

   } // namespace method

   namespace attribute {

      BaseData::tSptr BaseData::create( carpc::ipc::tStream& stream )
      {
         eID id = eID::UNDEFINED;
         carpc::service::eType type = carpc::service::eType::UNDEFINED;
         if( false == carpc::ipc::deserialize( stream, id, type ) )
            return nullptr;

         tSptr ptr = nullptr;
         switch( id )
         {
            case eID::current_state:
            {
               switch( type )
               {
                  case carpc::service::eType::NOTIFICATION:
                  {
                     ptr = std::make_shared< notification::current_stateData >( );
                     break;
                  }
                  default: break;
               }
            }
            default: break;
         }

         if( nullptr != ptr )
            ptr->from_stream( stream );

         return  ptr;
      }

      bool BaseData::serrialize( carpc::ipc::tStream& stream )
      {
         if( false == carpc::ipc::serialize( stream, id( ), type( ) ) )
            return false;
         return to_stream( stream );
      }

      namespace notification {

         current_stateData::current_stateData( const std::string& _value )
            : BaseData( )
            , value( _value )
         {
         }

         bool current_stateData::to_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::serialize( stream, value );
         }

         bool current_stateData::from_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::deserialize( stream, value );
         }

      } // namespace notification

   } // namespace attribute

} // namespace service::onoff::V2_0_0


