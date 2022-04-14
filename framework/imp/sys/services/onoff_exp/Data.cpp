#include "api/sys/services/onoff_exp/Data.hpp"



namespace service::onoff::ipc {

   namespace method {

      BaseData::tSptr BaseData::create( carpc::ipc::tStream& stream )
      {
         onoff::method::eID id = onoff::method::eID::Undefined;
         carpc::service::eType type = carpc::service::eType::UNDEFINED;
         if( false == carpc::ipc::deserialize( stream, id, type ) )
            return nullptr;

         tSptr ptr = nullptr;
         switch( id )
         {
            case onoff::method::eID::TriggerState:
            {
               switch( type )
               {
                  case carpc::service::eType::REQUEST:
                  {
                     ptr = std::make_shared< request::TriggerStateData >( );
                     break;
                  }
                  case carpc::service::eType::RESPONSE:
                  {
                     ptr = std::make_shared< request::TriggerStateData >( );
                     break;
                  }
                  default:
                  {
                     break;
                  }
               }
            }
            default:
            {
               break;
            }
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

         TriggerStateData::TriggerStateData( const std::string& _state, const std::size_t _delay )
            : BaseData( )
            , state( _state )
            , delay( _delay )
         {
         }

         bool TriggerStateData::to_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::serialize( stream, state, delay );
         }

         bool TriggerStateData::from_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::deserialize( stream, state, delay );
         }

      }



      namespace response {

         TriggerStateData::TriggerStateData( const bool _result )
            : BaseData( )
            , result( _result )
         {
         }

         bool TriggerStateData::to_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::serialize( stream, result );
         }

         bool TriggerStateData::from_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::deserialize( stream, result );
         }

      }



      namespace request {

         bool StartData::to_stream( carpc::ipc::tStream& stream )
         {
            return true;
         }

         bool StartData::from_stream( carpc::ipc::tStream& stream )
         {
            return true;
         }

      }

   }



   namespace attribute {

      BaseData::tSptr BaseData::create( carpc::ipc::tStream& stream )
      {
         onoff::attribute::eID id = onoff::attribute::eID::Undefined;
         carpc::service::eType type = carpc::service::eType::UNDEFINED;
         if( false == carpc::ipc::deserialize( stream, id, type ) )
            return nullptr;

         tSptr ptr = nullptr;
         switch( id )
         {
            case onoff::attribute::eID::CurrentState:
            {
               switch( type )
               {
                  case carpc::service::eType::NOTIFICATION:
                  {
                     ptr = std::make_shared< notification::CurrentStateData >( );
                     break;
                  }
                  default:
                  {
                     break;
                  }
               }

            }
            default:
            {
               break;
            }
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

         CurrentStateData::CurrentStateData( const std::string& _state )
            : BaseData( )
            , state( _state )
         {
         }

         bool CurrentStateData::to_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::serialize( stream, state );
         }

         bool CurrentStateData::from_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::deserialize( stream, state );
         }

      }

   }

} // namespace service::onoff::ipc



namespace service::onoff::no_ipc {

   namespace method {

      namespace request {

         TriggerStateData::TriggerStateData( const std::string& _state, const std::size_t _delay )
            : BaseData( )
            , state( _state )
            , delay( _delay )
         {
         }

      }



      namespace response {

         TriggerStateData::TriggerStateData( const bool _result )
            : BaseData( )
            , result( _result )
         {
         }

      }

   }



   namespace attribute {

      namespace notification {

         CurrentStateData::CurrentStateData( const std::string& _state )
            : BaseData( )
            , state( _state )
         {
         }

      }

   }

} // namespace service::onoff::no_ipc
