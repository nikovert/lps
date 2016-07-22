//
//  reaction.cpp
//  LPS
//
//  Created by Niko Vertovec on 16/07/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//
/*
#include "reaction.h"
#include "location.h"
#include "drone.h"

using namespace cv;
using namespace aruco;

MarkerDetector MDetector;
double TheMarkerSize = 8.89;
vector< Marker > TheMarkers;
CameraParameters TheCameraParameters;
pair< double, double > AvrgTime(0, 0); // determines the average time required for detection

void init_detect(Mat TheInputImage){
    // read camera parameters if passed
    TheCameraParameters.readFromXMLFile("../../LPS/include/out_camera_data.xml");
    TheCameraParameters.resize(TheInputImage.size());
}

void ar_detect(Mat TheInputImage){
            double tick = (double)getTickCount(); // for checking the speed
            // Detection of markers in the image passed
            MDetector.detect(TheInputImage, TheMarkers, TheCameraParameters, TheMarkerSize);
            // check the speed by calculating the mean speed of all iterations
            AvrgTime.first += ((double)getTickCount() - tick) / getTickFrequency();
            AvrgTime.second++;
            //            cout << "\rTime detection=" << 1000 * AvrgTime.first / AvrgTime.second << " milliseconds nmarkers=" << TheMarkers.size() << std::flush;
            
            for (unsigned int i = 0; i < TheMarkers.size(); i++) {
                //cout << endl << TheMarkers[i] << endl;
                cout << "tvec: " << getLocation(TheMarkers[i], TheCameraParameters) << endl;
            }
            
            if (TheMarkers.size() != 0)
                cout << endl;
}
*/