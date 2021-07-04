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



namespace experimental::dlib {

   // Example based on: http://dlib.net/dnn_mmod_face_detection_ex.cpp.html
   // const char* const model_file = "mmod_human_face_detector.dat";
   void dnn_mmod_face_detection_ex( const char* const image_file, const char* const model_file );

   // Example based on: http://dlib.net/face_detection_ex.cpp.html
   void face_detection_ex( const char* const image_file );

   // Example based on: http://dlib.net/webcam_face_pose_ex.cpp.html
   // const char* const model_file = "shape_predictor_68_face_landmarks.dat";
   bool face_detection( const int camera_id, const char* const model_file );

} // namespace experimental::dlib
