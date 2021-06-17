#include "api/sys/tools/Performance.hpp"
#include "api/exp/dlib.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OpenCV"



namespace experimental::dlib {

   template< long num_filters, typename SUBNET > using con5d = ::dlib::con< num_filters, 5, 5, 2, 2, SUBNET >;
   template< long num_filters, typename SUBNET > using con5  = ::dlib::con< num_filters, 5, 5, 1, 1, SUBNET >;

   template <typename SUBNET> using downsampler  = ::dlib::relu<
            ::dlib::affine<
               con5d< 32, ::dlib::relu< ::dlib::affine< con5d< 32, ::dlib::relu< ::dlib::affine< con5d< 16, SUBNET > > > > > > >
            >
         >;
   template <typename SUBNET> using rcon5  = ::dlib::relu< ::dlib::affine< con5< 45, SUBNET > > >;

   using net_type = ::dlib::loss_mmod<
            ::dlib::con< 1, 9, 9, 1, 1, rcon5< rcon5< rcon5< downsampler< ::dlib::input_rgb_image_pyramid< ::dlib::pyramid_down< 6 > > > > > > >
         >;

   void dnn_mmod_face_detection_ex( const char* const model_file, const char* const image_file )
   {
      base::tools::Performance performance( "dnn_mmod_face_detection_ex" );
      performance.start( );

      net_type net;
      ::dlib::deserialize( model_file ) >> net;

      ::dlib::image_window window;

      ::dlib::matrix< ::dlib::rgb_pixel > image;
      ::dlib::load_image( image, image_file );

      // while( image.size( ) < 1800 * 1800 )
      //    ::dlib::pyramid_up( image );

      auto dets = net( image );
      window.clear_overlay( );
      window.set_image( image );
      for( auto&& d : dets )
         window.add_overlay( d );

      performance.stop( );
      sleep( 10 );
   }

} // namespace experimental::dlib



namespace experimental::dlib {

   void face_detection_ex( const char* const image_file )
   {
      base::tools::Performance performance( "face_detection_ex" );
      performance.start( );

      ::dlib::array2d< ::dlib::bgr_pixel > image;
      ::dlib::load_image( image, image_file );
      // ::dlib::pyramid_up( image );

      ::dlib::frontal_face_detector detector = ::dlib::get_frontal_face_detector( );
      std::vector< ::dlib::rectangle > dets = detector( image );
      // std::cout << "Number of faces detected: " << dets.size( ) << std::endl;

      ::dlib::image_window window;
      window.clear_overlay( );
      window.set_image( image );
      window.add_overlay( dets, ::dlib::rgb_pixel( 255, 0, 0 ) );

      performance.stop( );
      sleep( 10 );
   }

} // namespace experimental::dlib
