#pragma once

#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/highgui.hpp>



namespace experimental::opencv {

   void draw_contours( const ::cv::Mat& source, const ::cv::Mat& destination );

   void process_image( const ::cv::Mat& image );

   void process_image( const std::string& path );

   void process_video( const std::string& path );

   void process_camera( const int camera_id );

} // namespace experimental::opencv
