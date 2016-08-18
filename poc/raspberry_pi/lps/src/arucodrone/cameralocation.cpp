//
//  camerlocation.cpp
//
//  Created by Niko Vertovec on 30/06/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <aruco/aruco.h>
#include <opencv2/highgui.hpp>
#include "arucodrone.h"

using namespace cv;
using namespace aruco;

//this function can be deleted
// --------------------------------------------------------------------------
//! @brief calculates yaw pitch and roll of drone (source: http://answers.opencv.org/answers/57759/revisions/)
//! @param rotation Matrix and the Vec3d to which the euler angle should be written
//! @return
// --------------------------------------------------------------------------
void ArucoDrone::setEulerAngles(Mat &rotCamerMatrix,Vec3d &eulerAngles){
    Mat cameraMatrix,rotMatrix,transVect,rotMatrixX,rotMatrixY,rotMatrixZ;
    double* _r = rotCamerMatrix.ptr<double>();
    double projMatrix[12] = {_r[0],_r[1],_r[2],0,
                          _r[3],_r[4],_r[5],0,
                          _r[6],_r[7],_r[8],0};

    decomposeProjectionMatrix( Mat(3,4,CV_64FC1,projMatrix),
                               cameraMatrix,
                               rotMatrix,
                               transVect,
                               rotMatrixX,
                               rotMatrixY,
                               rotMatrixZ,
                               eulerAngles);
}


// --------------------------------------------------------------------------
//! @brief calculates the coordinates of the drone, may be used in future to calculate camera pose
//! @param the aruco marker, the camera parametera, and an  optional boolean if the location should be printed
//! @return a Matrix of the drone location
// --------------------------------------------------------------------------
void ArucoDrone::getLocation(Marker m, CameraParameters TheCameraParameters, Point3d *position, Mat *rotation, bool print){
    //bool debug = false;
    if(print) cout << "Marker id: " << m.id << endl;
    //if(print) log_file << "Marker id: " << m.id << endl;
    Mat rvec, tvec;
    vector<Point3d> world_coords = setWorldCoords(m.id);
    
    //solvePnP returns the rotation and the translation vectors
    solvePnP(world_coords, setPixelCoords(m), TheCameraParameters.CameraMatrix, TheCameraParameters.Distorsion, rvec, tvec);
    
    if(print)cout << "rvec: "<< rvec << endl << "tvec: " << tvec << endl;
    //if(print)log_file << "rvec: "<< rvec << endl << "tvec: " << tvec << endl;
    Mat R;
    //The direction of the rotation vector indicates the axis of rotation, while the length (or “norm”) of the vector gives the angle.
    //Rodrigues converts the vector to a matrix
    Rodrigues(rvec, R);
    *rotation = R;
    //sets euler angle
    //setEulerAngles(rvec, eulerAngle);

    if(print)cout << "R: "<< R << endl;
    //if(print)log_file << "R: "<< R << endl;
    //we now need to calculate the extrinsic matrix
    //The extrinsic matrix takes the form of a rigid transformation matrix: a 3x3 rotation matrix in the left-block, and 3x1 translation column-vector in the right:
    //We add an extra row of (0,0,0,1) added to the bottom. This makes the matrix square,
    Mat extrinsic = cv::Mat::eye(4, 4, CV_64F);
    R.copyTo(extrinsic.rowRange(0, 3).colRange(0, 3));
    tvec.copyTo(extrinsic.rowRange(0, 3).col(3));
    if(print)cout << "extrinsic: "<< extrinsic << endl;
    //if(print)log_file << "extrinsic: "<< extrinsic << endl;
    //When applying the inverse, we use the fact that the inverse of a rotation matrix is it's transpose, and inverting a translation matrix simply negates the translation vector.
    cv::Mat extrinsic_inv = extrinsic.inv();
    if(print)cout << "extrinsic_inv: "<< extrinsic_inv << endl;
    //if(print)log_file << "extrinsic_inv: "<< extrinsic_inv << endl;
    *position = cv::Point3d(extrinsic_inv.at<double>(0,3),extrinsic_inv.at<double>(1,3),extrinsic_inv.at<double>(2,3));
}
