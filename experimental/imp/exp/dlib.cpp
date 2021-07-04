#include "api/sys/tools/Performance.hpp"
#include "api/exp/opencv.hpp"
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

   void dnn_mmod_face_detection_ex( const char* const image_file, const char* const model_file )
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



   // Example based on: http://dlib.net/webcam_face_pose_ex.cpp.html
   bool face_detection( const int camera_id, const char* const model_file )
   {
      ::cv::VideoCapture video_cap( camera_id );
      if( false == video_cap.isOpened( ) )
      {
         MSG_ERR( "Camera '%d' is not available", camera_id );
         return false;
      }


      // Load face detection.
      ::dlib::frontal_face_detector detector = ::dlib::get_frontal_face_detector( );

      // Load pose estimation models.
      ::dlib::shape_predictor pose_model;
      ::dlib::deserialize( model_file ) >> pose_model;

      ::dlib::image_window window;
      // Grab and process frames until the main window is closed by the user.
      while( false == window.is_closed( ) )
      {
         ::cv::Mat cv_image;
         // Grab a frame
         if( false == video_cap.read( cv_image ) )
         {
            MSG_ERR( "Camera '%d' read error", camera_id );
            break;
         }

         // Turn OpenCV's Mat into something dlib can deal with.
         // Note that this just wraps the Mat object, it doesn't copy anything.
         // So dlib_cv_image is only valid as long as cv_image is valid.
         // Also don't do anything to cv_image that would cause it to reallocate the memory
         // which stores the image as that will make dlib_cv_image contain dangling pointers.
         // This basically means you shouldn't modify cv_image while using dlib_cv_image.
         ::dlib::cv_image< ::dlib::bgr_pixel > dlib_cv_image( cv_image );
         // Or alternative variant
         // dlib::array2d< dlib::bgr_pixel > dlib_image;
         // dlib::assign_image( dlib_image, dlib::cv_image< dlib::bgr_pixel >( cv_image ) );

         // Detect faces
         std::vector< ::dlib::rectangle > faces = detector( dlib_cv_image );
         MSG_DBG( "Detected faces: %zu", faces.size( ) );

         // Find the pose of each face.
         std::vector< ::dlib::full_object_detection > shapes;
         for( auto&& face : faces )
            shapes.push_back( pose_model( dlib_cv_image, face ) );

         // Display it all on the screen
         window.clear_overlay( );
         window.set_image( dlib_cv_image );
         window.add_overlay( faces, ::dlib::rgb_pixel( 255, 0, 0 ) );
         window.add_overlay( ::dlib::render_face_detections( shapes ) );
      }

      return true;
   }

} // namespace experimental::dlib



namespace experimental::dlib {

   ::cv::Rect dlib_rectangle_to_opencv( const ::dlib::rectangle& r )
   {
      return ::cv::Rect(
            ::cv::Point2i( r.left( ), r.top( ) ),
            ::cv::Point2i( r.right( ) + 1, r.bottom( ) + 1 )
         );
   }

   ::dlib::rectangle opencv_rect_to_dlib( const ::cv::Rect& r )
   {
      return ::dlib::rectangle( (long)r.tl().x, (long)r.tl().y, (long)r.br().x - 1, (long)r.br().y - 1 );
   }

} // namespace experimental::dlib