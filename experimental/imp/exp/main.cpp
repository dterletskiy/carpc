#include <iostream>

// Framework
#include "api/sys/application/main.hpp"
#include "api/sys/tools/Performance.hpp"
// Application
#include "api/exp/opencv.hpp"
#include "api/exp/dlib.hpp"



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



cv::Rect dlib_rectangle_to_opencv( dlib::rectangle r )
{
   return cv::Rect(
         cv::Point2i( r.left( ), r.top( ) ),
         cv::Point2i( r.right( ) + 1, r.bottom( ) + 1 )
      );
}

dlib::rectangle opencv_rect_to_dlib( cv::Rect r )
{
   return dlib::rectangle((long)r.tl().x, (long)r.tl().y, (long)r.br().x - 1, (long)r.br().y - 1);
}



bool test( int argc, char** argv, char** envp )
{
   // experimental::opencv::process_image( "/home/scorpion/Desktop/resources/linux.png" );
   // experimental::opencv::process_video( "/home/scorpion/Desktop/resources/video.mp4" );
   // experimental::opencv::process_camera( 0 );



   // experimental::dlib::dnn_mmod_face_detection_ex( mmod_human_face_detector, image_file_vector[0] );
   // experimental::dlib::face_detection_ex( image_file_vector[0] );
   // experimental::dlib::face_detection_ex( image_file_vector[1] );
   // experimental::dlib::face_detection_ex( image_file_vector[2] );







   if( true )
   {
      int camera_id = 0;

      cv::VideoCapture video_cap( camera_id );
      if( false == video_cap.isOpened( ) )
      {
         MSG_ERR( "Camera '%d' is not available", camera_id );
         return false;
      }


      // Load face detection.
      dlib::frontal_face_detector detector = dlib::get_frontal_face_detector( );

      // Load pose estimation models.
      dlib::shape_predictor pose_model;
      dlib::deserialize( shape_predictor_68_face_landmarks ) >> pose_model;

      dlib::image_window window;
      // Grab and process frames until the main window is closed by the user.
      while( false == window.is_closed( ) )
      {
         cv::Mat cv_image;
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
         dlib::cv_image< dlib::bgr_pixel > dlib_cv_image( cv_image );
         // Or alternative variant
         // dlib::array2d< dlib::bgr_pixel > dlib_image;
         // dlib::assign_image( dlib_image, dlib::cv_image< dlib::bgr_pixel >( cv_image ) );

         // Detect faces
         std::vector< dlib::rectangle > faces = detector( dlib_cv_image );
         MSG_DBG( "Detected faces: %zu", faces.size( ) );

         // Find the pose of each face.
         std::vector< dlib::full_object_detection > shapes;
         for( auto&& face : faces )
            shapes.push_back( pose_model( dlib_cv_image, face ) );

         // Display it all on the screen
         window.clear_overlay( );
         window.set_image( dlib_cv_image );
         window.add_overlay( faces, dlib::rgb_pixel( 255, 0, 0 ) );
         window.add_overlay( dlib::render_face_detections( shapes ) );
      }
   }









   return false;
}
