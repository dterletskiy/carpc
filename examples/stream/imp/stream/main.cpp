// Framework
#include "api/sys/application/main.hpp"
#include "api/sys/common/IPC.hpp"

#include "api/stream/sensor.pb.h"



const base::application::Thread::Configuration::tVector services = { };

struct Data
{
   bool to_stream( base::ipc::tStream& stream ) const
   {
      return base::ipc::serialize( stream, m_id, m_name );
   }
   bool from_stream( base::ipc::tStream& stream )
   {
      return base::ipc::deserialize( stream, m_id, m_name );
   }

   void print( ) const
   {
      MSG_DBG( "id: 0x%X / name: %s", m_id, m_name.c_str( ) );
   }

   const std::size_t& id( ) const
   {
      return m_id;
   }

   const std::string& name( ) const
   {
      return m_name;
   }

   std::size_t m_id = 0xFFFFFFFF;
   std::string m_name = "NoName";
};

bool test( int argc, char** argv, char** envp )
{
   MSG_MARKER( "TEST" );

   base::ipc::tStream stream;

   {
      std::uint8_t value_uint8_t = 0xAB;
      std::uint16_t value_uint16_t = 12345;
      Data value_data{ 0xABCDEF, "DATA" };
      sensor::Data value_sensor_data;
      value_sensor_data.set_id( 0xAABBCCDD );
      value_sensor_data.set_name( "GPB_DATA" );

      base::ipc::serialize( stream, value_uint8_t, value_uint16_t, value_data, value_sensor_data );
   }

   {
      std::uint8_t value_uint8_t = 0;
      std::uint16_t value_uint16_t = 0;
      Data value_data;
      sensor::Data value_sensor_data;

      base::ipc::deserialize( stream, value_uint8_t, value_uint16_t, value_data, value_sensor_data );

      MSG_DBG( "value_uint8_t: 0x%X", value_uint8_t );
      MSG_DBG( "value_uint8_t: %d", value_uint16_t );
      MSG_DBG( "value_data.id: 0x%X / value_data.name: %s", value_data.id( ), value_data.name( ).c_str( ) );
      MSG_DBG( "value_sensor_data.id: 0x%X / value_sensor_data.name: %s", value_sensor_data.id( ), value_sensor_data.name( ).c_str( ) );
   }

   MSG_MARKER( "TEST" );

   return false;
}
