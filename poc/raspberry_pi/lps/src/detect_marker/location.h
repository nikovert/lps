//
//  location.h
//
//  Created by Niko Vertovec on 14/07/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#ifndef location_h
#define location_h

#include <aruco/aruco.h>

vector<cv::Point3d> setup(int id);
vector<cv::Point2d> setPixelCoords(aruco::Marker m);
cv::Mat getLocation(aruco::Marker m, aruco::CameraParameters TheCameraParameters, bool print);
double distance(int id);
void detect();
double distancetofly(cv::Point3d point);
cv::Point3d vectortofly(cv::Point3d point);
cv::Point3d MatPoint(cv::Mat mat);
extern double TheMarkerSize;
extern cv::Point3d drone_location;

#endif /* location.h */
