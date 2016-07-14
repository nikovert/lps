//
//  markerlocation.hpp
//  detect_marker
//
//  Created by Niko Vertovec on 14/07/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#ifndef markerlocation_h
#define markerlocation_h

#include <aruco/aruco.h>

vector<cv::Point3d> setup(int id);
vector<cv::Point2d> setPixelCoords(aruco::Marker m);
cv::Mat getLocation(aruco::Marker m, aruco::CameraParameters TheCameraParameters);

#endif /* markerlocation_hpp */
