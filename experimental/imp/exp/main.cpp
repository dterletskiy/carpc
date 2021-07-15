#include <iostream>

// Framework
#include "api/sys/application/main.hpp"
// Application
// #include "api/exp/opencv.hpp"
// #include "api/exp/dlib.hpp"



const base::application::Thread::Configuration::tVector services =
   {
   };
const char* const application_name{ "EXP" };




const char* const mmod_human_face_detector = "/mnt/p4v/Source/Examples/opencv/resources/mmod_human_face_detector.dat";
const char* const shape_predictor_68_face_landmarks = "/mnt/p4v/Source/Examples/opencv/resources/shape_predictor_68_face_landmarks.dat";



std::vector< const char* > image_file_vector = {
   "/home/scorpion/Desktop/resources/Hanna Hilton/1577456212_101.jpg",
   "/home/scorpion/Desktop/resources/Hanna Hilton/1577456193_38.jpg",
   "/home/scorpion/Desktop/resources/Hanna Hilton/1577456219_77.jpg"
};




#include <algorithm>
#include "api/sys/helpers/functions/print.hpp"

template < typename T, std::size_t N >
void odd_even_sort( T ( &array )[ N ] )
{
   for( std::size_t i = 0; i < N; i++ )
   {
      // (i % 2) ? 0 : 1 возвращает 1, если i четное, 0, если i не четное
      for( std::size_t j = (i % 2) ? 0 : 1; j + 1 < N; j += 2 )
      {
         if( array[ j ] > array[ j + 1 ] )
         {
            std::swap( array[ j ], array[ j + 1 ] );
         }
      }
   }
}



#define POWER_TYPE double

namespace FR_HALPER
{
   struct FR
   {
      FR( int xx, int yy ) : x( xx ), y( yy ) {  }
      FR inv(  ) { return FR( y, x ); }
      FR operator + ( const int & value ) { return FR( x + y * value, y ); }
      int x;
      int y;
   };

   // Dec to fraction without using iFraction
   static FR DecToFraction( const POWER_TYPE &value )
   {
      const POWER_TYPE eps = 1.0E-8;
      POWER_TYPE iter = value;
      int FP;
      std::vector< int > arrayFP;

      int step = 0;
      while( ( iter >= eps ) && ( step < 100 ) )
      {
         iter = 1 / iter;

         // FP = trunc(iter);
         FP = static_cast< int >( iter );
         if( FP > iter ) FP--;

         arrayFP.push_back( FP );
         iter = iter - FP;
         step++;
      }
      
      FR Fracrion( FP, 1 );
      for ( step = ( arrayFP.size(  ) - 2 ); step >= 0; step-- )
      {
         Fracrion = Fracrion.inv(  ) + arrayFP[ step ];
         if( step != 0 )
            if( Fracrion.x * arrayFP[ step-1 ] + Fracrion.y < 0 )
               break;
      }

      POWER_TYPE RelativeError = value / ( static_cast< POWER_TYPE >( Fracrion.y ) / static_cast< POWER_TYPE >( Fracrion.x ) );
      if(
            ( RelativeError < ( 1 - eps ) )
         || ( RelativeError > ( 1 + eps ) )
      )
      {
         Fracrion.x = static_cast< int >( 1 / eps );
         Fracrion.y = static_cast< int >( value / eps );
         printf( "Warning!!!\n" );
      }

      return Fracrion.inv(  );
   }
}



#include "api/sys/helpers/functions/print.hpp"
#include "api/sys/common/CircularBuffer.hpp"
#include "api/sys/common/ByteStream.hpp"
#include "api/sys/common/IPC.hpp"



bool test( int argc, char** argv, char** envp )
{
   // std::size_t array[ ] = { 4, 5, 55, 4, 457, 45, 68, 1, 87, 41 };

   // base::print( array, true );
   // odd_even_sort( array );
   // base::print( array, true );



   // FR_HALPER::FR fraction = FR_HALPER::DecToFraction( 0.4612297096963400519 );
   // MSG_DBG( "fraction: %d / %d", fraction.x, fraction.y );






   // const std::size_t buffer_size = 10;
   // const char buffer[ buffer_size ] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

   // MSG_MARKER( "1" );
   // base::CircularBuffer cb_1( buffer_size );
   // cb_1.push_back( buffer, buffer_size );
   // cb_1.dump( );
   // MSG_MARKER( "2" );
   // base::CircularBuffer cb_2( std::move( cb_1 ) );
   // cb_1.dump( );
   // cb_2.dump( );
   // MSG_MARKER( "3" );




   #if 0
   {
      const char buffer[ 10 ] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

      MSG_MARKER( "1" );
      base::CircularBuffer cb( 10 );
      cb.dump( );

      MSG_MARKER( "2" );
      cb.push_back( buffer, 9 );
      cb.dump( );

      MSG_MARKER( "3" );
      cb.push_back( buffer, 5 );
      cb.dump( );

      MSG_MARKER( "4" );
      cb.pop_back( 5 );
      cb.dump( );

      MSG_MARKER( "5" );
      cb.pop_front( 2 );
      cb.dump( );

      MSG_MARKER( "6" );
      cb.push_back( buffer, 5 );
      cb.dump( );

      MSG_MARKER( "7" );
      cb.push_back( buffer, 4 );
      cb.dump( );

      MSG_MARKER( "8" );
      cb.pop_back( 9 );
      cb.dump( );
   }
   #endif



   #if 0
   {
      const std::size_t buffer_size = 32;
      const void* const buffer = malloc( buffer_size );
      std::memset( (void*)buffer, 0xFF, buffer_size );
      void* const buffer_r = malloc( buffer_size );

      const std::size_t size_t_value = 12345;
      std::size_t size_t_value_r;

      const std::vector< std::size_t > vector = { 111, 222, 333, 444, 555 };
      std::vector< std::size_t > vector_r;

      const std::string string( "Component Architecture RPC" );
      std::string string_r;



      base::ByteStream stream;
      stream << size_t_value;
      stream << vector;
      stream << string;
      stream.push( buffer, buffer_size );

      base::ByteStream stream_new( std::move( stream ) );

      stream.dump( );
      stream_new.dump( );

      stream >> size_t_value_r;
      base::print( size_t_value_r, true );
      stream >> vector_r;
      base::print( vector_r, true );
      stream >> string_r;
      base::print( string_r, true );
      stream.pop( buffer_r, buffer_size );
      base::print( buffer_r, buffer_size, true );
   }
   #endif



   #if 0
   {
      const std::size_t array[ ] = { 100, 101, 102, 103, 104, 105, 106, 107, 108, 109 };
      std::size_t array_r[ ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      base::ByteStream stream( 128 );
      stream.push( *array );
      stream.dump( );
      stream.pop( *array_r );
      stream.dump( );
      base::print( *array_r, true );
   }
   #endif



   #if 0
   {
      const std::size_t buffer_size = 32;
      const void* const buffer = malloc( buffer_size );
      std::memset( (void*)buffer, 0xFF, buffer_size );
   }
   #endif








   return false;
}
