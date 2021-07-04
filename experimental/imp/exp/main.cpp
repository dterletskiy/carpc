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



bool test( int argc, char** argv, char** envp )
{
   // std::size_t array[ ] = { 4, 5, 55, 4, 457, 45, 68, 1, 87, 41 };

   // base::print( array, true );
   // odd_even_sort( array );
   // base::print( array, true );



   FR_HALPER::FR fraction = FR_HALPER::DecToFraction( 0.4612297096963400519 );
   MSG_DBG( "fraction: %d / %d", fraction.x, fraction.y );




   return false;
}
