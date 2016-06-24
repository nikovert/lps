/*****************************************************************************************
 Copyright 2011 Rafael Muñoz Salinas. All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are
 permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this list of
 conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice, this list
 of conditions and the following disclaimer in the documentation and/or other materials
 provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY Rafael Muñoz Salinas ''AS IS'' AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Rafael Muñoz Salinas OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 The views and conclusions contained in the software and documentation are those of the
 authors and should not be interpreted as representing official policies, either expressed
 or implied, of Rafael Muñoz Salinas.
 ********************************************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include "aruco.h"
#include "cvdrawingutils.h"
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace aruco;

string TheInputVideo;
string TheIntrinsicFile;
float TheMarkerSize = -1;
int ThePyrDownLevel;
MarkerDetector MDetector;
VideoCapture TheVideoCapturer;
vector< Marker > TheMarkers;
Mat TheInputImage, TheInputImageCopy;
CameraParameters TheCameraParameters;
void cvTackBarEvents(int pos, void *);
bool readCameraParameters(string TheIntrinsicFile, CameraParameters &CP, Size size);

pair< double, double > AvrgTime(0, 0); // determines the average time required for detection
double ThresParam1, ThresParam2;
int iThresParam1, iThresParam2;
int waitTime = 0;

void set(Mat rotation_matrix, Mat translation_matrix, Mat distortion, Mat camera_intrinsics, Point3d world_coords, Point2d pixel_coords);

/************************************
 *
 *
 *
 *
 ************************************/
bool readArguments(int argc, char **argv) {
    if (argc < 2) {
        cerr << "Invalid number of arguments" << endl;
        cerr << "Usage: (in.avi|live[:idx_cam=0]) [intrinsics.yml] [size]" << endl;
        return false;
    }
    
    TheInputVideo = argv[1];
    if (argc >= 3)
        TheIntrinsicFile = argv[2]; //Camera calibration file, not sure!
    if (argc >= 4)
        TheMarkerSize = atof(argv[3]);
    
    if (argc == 3)
        cerr << "NOTE: You need makersize to see 3d info!!!!" << endl;
    return true;
}

int findParam(std::string param, int argc, char *argv[]) {
    for (int i = 0; i < argc; i++)
        if (string(argv[i]) == param)
            return i;
    
    return -1;
}
/************************************
 *
 *
 *
 *
 ************************************/
int main(int argc, char **argv) {
    try {
        if (readArguments(argc, argv) == false) {
            return 0;
        }
        
        // parse arguments
        
        // read from camera or from  file
        if (TheInputVideo.find("live") != string::npos) {
            int vIdx = 0;
            // check if the :idx is here
            char cad[100];
            if (TheInputVideo.find(":") != string::npos) {
                std::replace(TheInputVideo.begin(), TheInputVideo.end(), ':', ' ');
                sscanf(TheInputVideo.c_str(), "%s %d", cad, &vIdx);
            }
            cout << "Opening camera index " << vIdx << endl;
            TheVideoCapturer.open(vIdx);
            waitTime = 10;
        } else
            TheVideoCapturer.open(TheInputVideo);
        // check video is open
        if (!TheVideoCapturer.isOpened()) {
            cerr << "Could not open video" << endl;
            return -1;
        }
        bool isVideoFile = false;
        if (TheInputVideo.find(".avi") != std::string::npos || TheInputVideo.find("live") != string::npos)
            isVideoFile = true;
        // read first image to get the dimensions
        TheVideoCapturer >> TheInputImage;
        
        // read camera parameters if passed
        if (TheIntrinsicFile != "") {
            TheCameraParameters.readFromXMLFile(TheIntrinsicFile);
            TheCameraParameters.resize(TheInputImage.size());
        }
        // Configure other parameters
        if (ThePyrDownLevel > 0)
            MDetector.pyrDown(ThePyrDownLevel);
        
        char key = 0;
        int index = 0;
        // capture until press ESC or until the end of the video
//        TheVideoCapturer.retrieve(TheInputImage);
        
        do {
            
            // copy image
            
            index++; // number of images captured
            double tick = (double)getTickCount(); // for checking the speed
            // Detection of markers in the image passed
            MDetector.detect(TheInputImage, TheMarkers, TheCameraParameters, TheMarkerSize);
            // check the speed by calculating the mean speed of all iterations
            AvrgTime.first += ((double)getTickCount() - tick) / getTickFrequency();
            AvrgTime.second++;
            cout << "\rTime detection=" << 1000 * AvrgTime.first / AvrgTime.second << " milliseconds nmarkers=" << TheMarkers.size() << std::flush;
            
            // print marker info and draw the markers in image
            
            //3D points for test purposes
            vector<Point3d>     world_coords;
            
            world_coords.push_back (Point3d (10.91666666666667, 10.01041666666667, 0));
            world_coords.push_back (Point3d (10.91666666666667, 8.34375, 0));
            world_coords.push_back (Point3d (16.08333333333334, 8.34375, 0));
            world_coords.push_back (Point3d (16.08333333333334, 10.01041666666667, 0));
            //
            
            for (unsigned int i = 0; i < TheMarkers.size(); i++) {
                TheMarkers[i].calculateExtrinsics(TheMarkerSize, TheCameraParameters.CameraMatrix, TheCameraParameters.Distorsion, true); //not sure if it should be true
                
                //giving information to realworld.cpp
                set(TheMarkers[i].Rvec, TheMarkers[i].Tvec, TheCameraParameters.Distorsion, TheCameraParameters.CameraMatrix, world_coords[i], TheMarkers[i].getCenter());
                
                cout << endl << TheMarkers[i];
            }
            if (TheMarkers.size() != 0)
                cout << endl;
            
            key = cv::waitKey(waitTime); // wait for key to be pressed
            
            if (isVideoFile)
                TheVideoCapturer.retrieve(TheInputImage);
            
        } while (key != 27 && (TheVideoCapturer.grab() || !isVideoFile));
        
    } catch (std::exception &ex)
    
    {
        cout << "Exception :" << ex.what() << endl;
    }
}
