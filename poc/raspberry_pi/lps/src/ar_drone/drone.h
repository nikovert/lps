//
//  drone.h
//  ar_drone
//
//  Created by Niko Vertovec on 14/07/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#ifndef drone_h
#define drone_h

#include "move.h"
#include "ardrone/ardrone.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <aruco/aruco.h>
#include <aruco/cvdrawingutils.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <raspicam/raspicam_cv.h>
#include "../detect_marker/location.h"


void fly();
void ar_detect(cv::Mat TheInputImage);
void init_detect(cv::Mat TheInputImage);
    
extern aruco::MarkerDetector MDetector;
extern double TheMarkerSize;
extern vector< aruco::Marker > TheMarkers;
extern aruco::CameraParameters TheCameraParameters;
extern pair< double, double > AvrgTime;

extern ARDrone ardrone;
extern bool drone_initialised;

#endif /* drone_h */
