//
//  camerlocation.cpp
//  detect_marker
//
//  Created by Niko Vertovec on 30/06/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <aruco/aruco.h>
#include <opencv2/highgui.hpp>
#include "location.h"

using namespace cv;
using namespace aruco;

Mat getLocation(Marker m, CameraParameters TheCameraParameters){
    //bool debug = false;
    cout << "Marker id: " << m.id << endl;
    Mat rvec, tvec;
    vector<Point3d> world_coords = setup(m.id);
    if(m.id!=50 && m.id!=698) return tvec;
    
    //solvePnP returns the rotation and the translation vectors
    solvePnP(world_coords, setPixelCoords(m), TheCameraParameters.CameraMatrix, TheCameraParameters.Distorsion, rvec, tvec);
    
    cout << "tvec: " << tvec << endl << endl;
    return tvec;
    
    //May be used for pose estimate at later point
    
    /*
    Mat imagePoints;
    Mat jacobian;
    double aspectRatio=0;
    projectPoints(world_coords, rvec, tvec, TheCameraParameters.CameraMatrix, TheCameraParameters.Distorsion, imagePoints, jacobian, aspectRatio);
    
    //cout << "imagePoints: " << imagePoints << endl;
    //cout << "pre given Points: " << setPixelCoords(m,false) << endl;
    waitKey(100);
    
    if(debug)cout << "rvec: "<< rvec << endl << "tvec: " << tvec << endl;
    Mat R;
    //The direction of the rotation vector indicates the axis of rotation, while the length (or “norm”) of the vector gives the angle.
    //Rodrigues converts the vector to a matrix
    Rodrigues(rvec, R);
    if(debug)cout << "R: "<< R << endl;
    //we now need to calculate the extrinsic matrix
    //The extrinsic matrix takes the form of a rigid transformation matrix: a 3x3 rotation matrix in the left-block, and 3x1 translation column-vector in the right:
    //We add an extra row of (0,0,0,1) added to the bottom. This makes the matrix square,
    Mat extrinsic = cv::Mat::eye(4, 4, CV_64F);
    R.copyTo(extrinsic.rowRange(0, 3).colRange(0, 3));
    tvec.copyTo(extrinsic.rowRange(0, 3).col(3));
    if(debug)cout << "extrinsic: "<< extrinsic << endl;
    //When applying the inverse, we use the fact that the inverse of a rotation matrix is it's transpose, and inverting a translation matrix simply negates the translation vector.
    cv::Mat extrinsic_inv = extrinsic.inv();
    if(debug)cout << "extrinsic_inv: "<< extrinsic_inv << endl;
    cout << "tvec: " << tvec << endl;
     */
}
