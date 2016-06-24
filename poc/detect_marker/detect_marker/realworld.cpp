//
//  realworld.cpp
//  detect_marker
//
//  Created by Niko Vertovec on 23/06/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//  got parts of the source code from http://stackoverflow.com/questions/15727527/how-to-determine-world-coordinates-of-a-camera
//

#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace std;
using namespace cv;

//we need the camera xml file (diffrent for each camera)    // doesn't change

//we need the world coordinates                             // doesn't change

//we need the pixel coordinates                             // changes

//we need the rotation and translation vector               // changes
// Marker.calculateExtrinsics() then get Rvec and Tvec

void set(Mat rotation_matrix, Mat translation_matrix, Mat distortion, Mat camera_intrinsics, Point3d world_coords, Point2d pixel_coords){
    cout << endl << "this wouldbe the point where I calculate my position" << endl;
}

void getCoordinates (int argc, char** argv)
{
    const char          *calibration_filename = argc >= 2 ? argv [1] : "M1011_camera.xml";
    FileStorage         camera_data (calibration_filename, FileStorage::READ);
    Mat                 camera_intrinsics, distortion;
    vector<Point3d>     world_coords;
    vector<Point2d>     pixel_coords;
    Mat                 rotation_vector, translation_vector, rotation_matrix, inverted_rotation_matrix, cw_translate;
    Mat                 cw_transform = cv::Mat::eye (4, 4, CV_64FC1);
    
    
    // Read camera data
    camera_data ["camera_matrix"] >> camera_intrinsics;
    camera_data ["distortion_coefficients"] >> distortion;
    camera_data.release ();
    
    // Target rectangle coordinates in feet
    world_coords.push_back (Point3d (10.91666666666667, 10.01041666666667, 0));
    world_coords.push_back (Point3d (10.91666666666667, 8.34375, 0));
    world_coords.push_back (Point3d (16.08333333333334, 8.34375, 0));
    world_coords.push_back (Point3d (16.08333333333334, 10.01041666666667, 0));
    
    // Coordinates of rectangle in camera
    pixel_coords.push_back (Point2d (284, 204));
    pixel_coords.push_back (Point2d (286, 249));
    pixel_coords.push_back (Point2d (421, 259));
    pixel_coords.push_back (Point2d (416, 216));
    
    // Get vectors for world->camera transform
    solvePnP (world_coords, pixel_coords, camera_intrinsics, distortion, rotation_vector, translation_vector, false, 0);
    //dump_matrix (rotation_vector, String ("Rotation vector"));            //do not know what this function does!
    //dump_matrix (translation_vector, String ("Translation vector"));
    
    // We need inverse of the world->camera transform (camera->world) to calculate
    // the camera's location
    
    // cameraTranslationVector contains camera coordinates. cameraRotationVector contains camera pose.
    
    // this part was added but not tested
        cv::Mat R;
        cv::Rodrigues(rotation_vector, R);
        
        cv::Mat cameraRotationVector;
        
        cv::Rodrigues(R.t(),cameraRotationVector);
        
        cv::Mat cameraTranslationVector = -R.t()*translation_vector;
    //
    Rodrigues (rotation_vector, rotation_matrix);
    Rodrigues (rotation_matrix.t (), cameraRotationVector);
    Mat t = translation_vector.t ();
    cameraTranslationVector = -cameraRotationVector * t;
    
    printf ("Camera position %f, %f, %f\n", cameraTranslationVector.at<double>(0), cameraTranslationVector.at<double>(1), cameraTranslationVector.at<double>(2));
    printf ("Camera pose %f, %f, %f\n", cameraRotationVector.at<double>(0), cameraRotationVector.at<double>(1), cameraRotationVector.at<double>(2));
}