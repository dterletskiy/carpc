#pragma once

#include <dlib/dnn.h>
#include <dlib/gui_widgets.h>
#include <dlib/clustering.h>
#include <dlib/string.h>
#include <dlib/image_io.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/opencv/cv_image.h>
#include <dlib/opencv.h>

#include <opencv4/opencv2/highgui.hpp>



namespace experimental::dlib {

   void dnn_mmod_face_detection_ex( const char* const model_file, const char* const image_file );

   void face_detection_ex( const char* const image_file );

} // namespace experimental::dlib
