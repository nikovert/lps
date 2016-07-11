//
//  camerlocation.cpp
//  detect_marker
//
//  Created by Niko Vertovec on 30/06/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "aruco.h"
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace aruco;

vector<Point3d> setWorldCoords(Point3d top_left, Point3d top_right, Point3d bottom_left, Point3d bottom_right){
    vector<Point3d> world_coords;
    world_coords.push_back (top_left);
    world_coords.push_back (top_right);
    world_coords.push_back (bottom_left);
    world_coords.push_back (bottom_right);
    return world_coords;
}

vector<Point2d> setPixelCoords(Marker m, bool center){
    vector<Point2d> pixel_coords;
    if(center) {
    int a = sqrt(m.getArea())/2;
        pixel_coords.push_back (Point2d (m.getCenter().x - a, m.getCenter().y + a));
        pixel_coords.push_back (Point2d (m.getCenter().x + a, m.getCenter().y + a));
        pixel_coords.push_back (Point2d (m.getCenter().x + a, m.getCenter().y - a));
        pixel_coords.push_back (Point2d (m.getCenter().x - a, m.getCenter().y - a));
    }else{
        pixel_coords.push_back (m[0]);
        pixel_coords.push_back (m[1]);
        pixel_coords.push_back (m[2]);
        pixel_coords.push_back (m[3]);
    }return pixel_coords;
}


Mat getLocation(Marker m, CameraParameters TheCameraParameters){
    vector<Point3d> world_coords;
    //bool debug = false;
    cout << "Marker id: " << m.id << endl;
    Mat rvec, tvec;
    if(m.id==50){
        cout <<"setting world coords for id 50" << endl;
        world_coords = setWorldCoords(Point3d(-8.89,12.7,0),Point3d(0,12.7,0),Point3d(0,3.81,0),Point3d(-8.89,3.81,0));
    }
    if(m.id==698){
        cout <<"setting world coords for id 698" << endl;
        world_coords = setWorldCoords(Point3d (0, 0, 0), Point3d (8.89, 0, 0), Point3d (8.89, -8.89, 0), Point3d (0, -8.89, 0));
    }
    if(m.id!=50 && m.id!=698) return tvec;
    
    //solvePnP returns the rotation and the translation vectors
    solvePnP(world_coords, setPixelCoords(m,false), TheCameraParameters.CameraMatrix, TheCameraParameters.Distorsion, rvec, tvec);
    cout << "tvec: " << tvec << endl << endl;
    return tvec;
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
