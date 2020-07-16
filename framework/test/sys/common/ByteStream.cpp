#include "api/sys/helpers/macros/strings.hpp"
#include "api/sys/common/ByteStream.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TEST:STREAM"



namespace base::dsi::byte_stream::test {

   enum class eEnum { one, two, three };

   bool run( )
   {

      base::ByteStream stream( 64 );

      {
         size_t value_integral = 0xAAAAAAAA;
         eEnum value_enum = eEnum::three;
         std::string value_string = "Hello world!";
         std::optional< std::string > value_optional_string = "OptionalString";
         std::vector< std::string > value_vector_string = { "one", "two", "three", "four", "five" };
         std::map< size_t, std::string > value_map = { { 0xAAAAAAAA, "one" }, { 0xBBBBBBBB, "two" }, { 0xCCCCCCCC, "three" }, { 0xDDDDDDDD, "four" }, { 0xEEEEEEEE, "five" } };
         bool value_bool = true;
         void* value_pointer = &value_integral;

         stream.push( value_integral );
         stream.dump( );
         stream.push( value_enum );
         stream.dump( );
         stream.push( value_string );
         stream.dump( );
         stream.push( value_optional_string );
         stream.dump( );
         stream.push( value_vector_string );
         stream.dump( );
         stream.push( value_map );
         stream.dump( );
         stream.push( value_bool );
         stream.dump( );
         stream.push( value_pointer );
         stream.dump( );



         {
            size_t test_value_integral = 0;
            eEnum test_value_enum = eEnum::one;
            std::string test_value_string = "";
            std::optional< std::string > test_value_optional_string = "";
            std::vector< std::string > test_value_vector_string;
            std::map< size_t, std::string > test_value_map;
            bool test_value_bool = false;
            void* test_value_pointer = nullptr;

            // stream.get( sizeof( size_t ) + sizeof( eEnum ), test_value_string );
            // DBG_WRN( "string: %s", test_value_string.c_str( ) );

            // stream.get( 0, test_value_integral, test_value_enum, test_value_string, test_value_optional_string, test_value_vector_string
            //    , test_value_map, test_value_bool, test_value_pointer
            // );

            DBG_MSG( "integral result: %s", BOOL_TO_STRING( value_integral == test_value_integral ) );
            DBG_MSG( "enum result: %s", BOOL_TO_STRING( value_enum == test_value_enum ) );
            DBG_MSG( "string result: %s", BOOL_TO_STRING( value_string == test_value_string ) );
            DBG_MSG( "optional result: %s", BOOL_TO_STRING( value_optional_string == test_value_optional_string ) );
            DBG_MSG( "vector result: %s", BOOL_TO_STRING( value_vector_string == test_value_vector_string ) );
            DBG_MSG( "map result: %s", BOOL_TO_STRING( value_map == test_value_map ) );
            DBG_MSG( "bool result: %s", BOOL_TO_STRING( value_bool == test_value_bool ) );
            DBG_MSG( "bool result: %s", BOOL_TO_STRING( value_pointer == test_value_pointer ) );
            DBG_MSG( "%p -> %p", value_pointer, test_value_pointer );
         }

         {
            size_t test_value_integral = 0;
            eEnum test_value_enum = eEnum::one;
            std::string test_value_string = "";
            std::optional< std::string > test_value_optional_string = "";
            std::vector< std::string > test_value_vector_string;
            std::map< size_t, std::string > test_value_map;
            bool test_value_bool = false;
            void* test_value_pointer = nullptr;

            stream.pop( test_value_integral );
            stream.dump( );
            stream.pop( test_value_enum );
            stream.dump( );
            stream.pop( test_value_string );
            stream.dump( );
            stream.pop( test_value_optional_string );
            stream.dump( );
            stream.pop( test_value_vector_string );
            stream.dump( );
            stream.pop( test_value_map );
            stream.dump( );
            stream.pop( test_value_bool );
            stream.dump( );
            stream.pop( test_value_pointer );
            stream.dump( );

            DBG_MSG( "integral result: %s", BOOL_TO_STRING( value_integral == test_value_integral ) );
            DBG_MSG( "enum result: %s", BOOL_TO_STRING( value_enum == test_value_enum ) );
            DBG_MSG( "string result: %s", BOOL_TO_STRING( value_string == test_value_string ) );
            DBG_MSG( "optional result: %s", BOOL_TO_STRING( value_optional_string == test_value_optional_string ) );
            DBG_MSG( "vector result: %s", BOOL_TO_STRING( value_vector_string == test_value_vector_string ) );
            DBG_MSG( "map result: %s", BOOL_TO_STRING( value_map == test_value_map ) );
            DBG_MSG( "bool result: %s", BOOL_TO_STRING( value_bool == test_value_bool ) );
            DBG_MSG( "bool result: %s", BOOL_TO_STRING( value_pointer == test_value_pointer ) );
            DBG_MSG( "%p -> %p", value_pointer, test_value_pointer );
         }

      }

      stream.push( 0xAAAAAAAA, std::string( "string" ), 0xBBBBBBBB );

      base::ByteStream stream_new( 1024 );
      stream_new.push( stream, 0xCCCCCCCC );
      stream.reset( );

      DBG_WRN( "--------------------------------------------------------------" );
      stream_new.dump( );
      DBG_WRN( "--------------------------------------------------------------" );
      stream_new.pop( stream );
      stream.dump( );
      DBG_WRN( "--------------------------------------------------------------" );
      stream_new.dump( );
      DBG_WRN( "--------------------------------------------------------------" );

      return true;
   }
}
