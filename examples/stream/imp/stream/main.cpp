#include <bitset>
// Framework
#include "api/sys/application/main.hpp"
#include "api/sys/common/IPC.hpp"
#include "api/sys/helpers/functions/print.hpp"

#include "api/stream/sensor.pb.h"



const carpc::application::Thread::Configuration::tVector services = { };

struct Data
{
   Data( ) = default;

   Data( const std::size_t& id, const std::string& name )
      : m_id( id )
      , m_name( name )
   {
   }

   bool to_stream( carpc::ipc::tStream& stream ) const
   {
      return carpc::ipc::serialize( stream, m_id, m_name );
   }
   bool from_stream( carpc::ipc::tStream& stream )
   {
      return carpc::ipc::deserialize( stream, m_id, m_name );
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

bool operator==( const Data& data_1, const Data data_2 )
{
   return data_1.id( ) == data_2.id( ) && data_1.name( ) == data_2.name( );
}



bool operator==( const sensor::Data& data_1, const sensor::Data data_2 )
{
   return data_1.id( ) == data_2.id( ) && data_1.name( ) == data_2.name( );
}



#define TEST( A, B ) \
   if( A == B ) \
   { \
      MSG_DBG( "%s == %s", #A, #B ); \
   } \
   else \
   { \
      MSG_ERR( "%s != %s", #A, #B ); \
   } \



bool test( int argc, char** argv, char** envp )
{
   MSG_MARKER( "TEST" );

   carpc::ipc::tStream stream;



   if( false )
   {
      bool                       value_bool = true,                        test_value_bool = false;
      std::uint8_t               value_uint8_t = 0xAB,                     test_value_uint8_t = 0;
      std::int16_t               value_int16_t = -12345,                   test_value_int16_t = 0;
      float                      value_float = 123.456,                    test_value_float = 0.0;
      double                     value_double = -654.321,                  test_value_double = -0.0;
      std::vector< int >         value_vector_int = { 11, 22, 33 },        test_value_vector_int = { 0 };
      std::set< int >            value_set_int = { 44, 55, 66 },           test_value_set_int = { 0 };
      std::optional< double >    value_optional_double = 987.654,          test_value_optional_double = std::nullopt;
      std::optional< long >      value_optional_long = std::nullopt,       test_value_optional_long = 123456789;
      std::string                value_string = "Hello CARPC",             test_value_string = { };
      Data                       value_data{ 0xABCDEF, "DATA" },           test_value_data{ 0, "" };
      sensor::Data               value_sensor_data,                        test_value_sensor_data;
      value_sensor_data.set_id( 0xAABBCCDD );
      value_sensor_data.set_name( "GPB_DATA" );
      std::shared_ptr< Data > p_value_data = std::make_shared< Data >( 0xABCDEF, "SHARED" );
      std::shared_ptr< Data > p_value_data_test = nullptr;//std::make_shared< Data >( 0x12345678, "NoName" );

      std::map<
            std::size_t,
            std::map<
               std::size_t,
               std::vector< std::optional< std::set< double > > >
            >
      > value =
      {
         {
            100,
            {
               { 11111, std::vector< std::optional< std::set< double > > >{ std::set< double >{ 1.1, 2.2, 3.3 } } },
               { 22222, std::vector< std::optional< std::set< double > > >{ std::set< double >{ 4.4, 5.5, 6.6 } } },
               { 33333, std::vector< std::optional< std::set< double > > >{ std::nullopt } }
            }
         },
         {
            200,
            {
               { 44444, std::vector< std::optional< std::set< double > > >{ std::nullopt } },
               { 55555, std::vector< std::optional< std::set< double > > >{ std::set< double >{ 7.7, 8.8, 9.9 } } },
               { 66666, std::vector< std::optional< std::set< double > > >
                  {
                     std::set< double >{ 1.0, 2.0, 3.0 }, std::nullopt, std::nullopt
                  }
               },
            }
         },
      }, test_value;

      carpc::ipc::serialize(
              stream
            , value_bool
            , value_uint8_t
            , value_int16_t
            , value_float
            , value_double
            , value_vector_int
            , value_set_int
            , value_optional_double
            , value_optional_long
            , value_string
            , value_data
            , value_sensor_data
            , value
            , p_value_data
         );

      carpc::ipc::deserialize(
              stream
            , test_value_bool
            , test_value_uint8_t
            , test_value_int16_t
            , test_value_float
            , test_value_double
            , test_value_vector_int
            , test_value_set_int
            , test_value_optional_double
            , test_value_optional_long
            , test_value_string
            , test_value_data
            , test_value_sensor_data
            , test_value
            , p_value_data_test
         );

      TEST( value_bool, test_value_bool );
      TEST( value_uint8_t, test_value_uint8_t );
      TEST( value_int16_t, test_value_int16_t );
      TEST( value_float, test_value_float );
      TEST( value_double, test_value_double );
      TEST( value_vector_int, test_value_vector_int );
      TEST( value_set_int, test_value_set_int );
      TEST( value_optional_double, test_value_optional_double );
      TEST( value_optional_long, test_value_optional_long );
      TEST( value_string, test_value_string );
      TEST( value_data, test_value_data );
      TEST( value_sensor_data, test_value_sensor_data );
      TEST( value, test_value );

      if( nullptr != p_value_data )
      {
         p_value_data->print( );
      }
      else
      {
         MSG_DBG( "p_value_data = nullptr" );
      }
      if( nullptr != p_value_data_test )
      {
         p_value_data_test->print( );
      }
      else
      {
         MSG_DBG( "p_value_data_test = nullptr" );
      }
   }

   if( false )
   {
      std::shared_ptr< Data > p_value_data = std::make_shared< Data >( 0xABCDEF, "SHARED" );
      std::shared_ptr< Data > p_value_data_test = nullptr;//std::make_shared< Data >( 0x12345678, "NoName" );
      if( nullptr != p_value_data )
      {
         p_value_data->print( );
      }
      else
      {
         MSG_DBG( "p_value_data = nullptr" );
      }
      if( nullptr != p_value_data_test )
      {
         p_value_data_test->print( );
      }
      else
      {
         MSG_DBG( "p_value_data_test = nullptr" );
      }

      carpc::ipc::serialize( stream, p_value_data );

      carpc::ipc::deserialize( stream, p_value_data_test );

      if( nullptr != p_value_data )
      {
         p_value_data->print( );
      }
      else
      {
         MSG_DBG( "p_value_data = nullptr" );
      }
      if( nullptr != p_value_data_test )
      {
         p_value_data_test->print( );
      }
      else
      {
         MSG_DBG( "p_value_data_test = nullptr" );
      }
   }

   if( false )
   {
      std::unique_ptr< Data > p_value_data = std::make_unique< Data >( 0xABCDEF, "UNIQUE" );
      std::unique_ptr< Data > p_value_data_test = nullptr;//std::make_unique< Data >( 0x12345678, "NoName" );
      if( nullptr != p_value_data )
      {
         p_value_data->print( );
      }
      else
      {
         MSG_DBG( "p_value_data = nullptr" );
      }
      if( nullptr != p_value_data_test )
      {
         p_value_data_test->print( );
      }
      else
      {
         MSG_DBG( "p_value_data_test = nullptr" );
      }

      carpc::ipc::serialize( stream, p_value_data );

      carpc::ipc::deserialize( stream, p_value_data_test );

      if( nullptr != p_value_data )
      {
         p_value_data->print( );
      }
      else
      {
         MSG_DBG( "p_value_data = nullptr" );
      }
      if( nullptr != p_value_data_test )
      {
         p_value_data_test->print( );
      }
      else
      {
         MSG_DBG( "p_value_data_test = nullptr" );
      }
   }





   MSG_MARKER( "TEST" );

   return false;
}
