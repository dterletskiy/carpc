#include "api/exp/opencv.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OpenCV"



namespace experimental::opencv {

   void draw_contours( const ::cv::Mat& source, const ::cv::Mat& destination )
   {
      std::vector< std::vector< ::cv::Point > > contours;
      std::vector< ::cv::Vec4i > hierarchy;

      ::cv::findContours( source, contours, hierarchy, ::cv::RETR_EXTERNAL, ::cv::CHAIN_APPROX_SIMPLE );
      ::cv::drawContours( destination, contours, -1, ::cv::Scalar( 255, 0, 255 ), 2 );
   }

   void process_image( const ::cv::Mat& image )
   {
      ::cv::Mat image_gray;
      ::cv::cvtColor( image, image_gray, ::cv::COLOR_BGR2GRAY );

      ::cv::Mat image_blur;
      ::cv::GaussianBlur( image, image_blur, ::cv::Size( 3, 3 ), 3, 0 );

      ::cv::Mat image_canny;
      ::cv::Canny( image_blur, image_canny, 25, 75 );

      ::cv::Mat kernel = ::cv::getStructuringElement( ::cv::MORPH_RECT, ::cv::Size( 5, 5 ) );

      ::cv::Mat image_dilate;
      ::cv::dilate( image_canny, image_dilate, kernel );

      ::cv::Mat image_erode;
      ::cv::erode( image_dilate, image_erode, kernel );

      draw_contours( image_dilate, image );

      // ::cv::namedWindow( "Display Image", ::cv::WINDOW_AUTOSIZE );
      ::cv::imshow( "Image", image );
      // ::cv::imshow( "Image Gray", image_gray );
      // ::cv::imshow( "Image Blur", image_blur );
      // ::cv::imshow( "Image Canny", image_canny );
      // ::cv::imshow( "Image Dilate", image_dilate );
      // ::cv::imshow( "Image Erode", image_erode );
   }

   void process_image( const std::string& path )
   {
      ::cv::Mat image = ::cv::imread( path );
      process_image( image );
      ::cv::waitKey( 0 );
   }

   void process_video( const std::string& path )
   {
      ::cv::VideoCapture video_cap( path );
      ::cv::Mat image;

      while( true )
      {
         video_cap.read( image );
         process_image( image );
         ::cv::waitKey( 1 );
      }
   }

   void process_camera( const int camera_id )
   {
      ::cv::VideoCapture video_cap( camera_id );
      ::cv::Mat image;

      while( true )
      {
         video_cap.read( image );
         process_image( image );
         ::cv::waitKey( 1 );
      }
   }

} // namespace experimental::opencv
