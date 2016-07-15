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
#include <aruco/aruco.h>
#include <aruco/cvdrawingutils.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "location.h"

using namespace cv;
using namespace aruco;

string TheInputVideo;
string TheIntrinsicFile;
double TheMarkerSize = -1;
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

/************************************
 *
 *
 *
 *
 ************************************/

class Settings{
public:
    Settings() : goodInput(false) {}
    bool goodInput;
    
    void read(const FileNode& node){
        node["TheInputVideo" ] >> TheInputVideo;
        node["TheIntrinsicFile"] >> TheIntrinsicFile;
        node["TheMarkerSize"] >> TheMarkerSize;
        validate();
    }

    void validate() {
        goodInput = true;
//        if (boardSize.width <= 0 || boardSize.height <= 0)
//        {
//            cerr << "Invalid Board size: " << boardSize.width << " " << boardSize.height << endl;
//            goodInput = false;
//        }
//        if (squareSize <= 10e-6)
//        {
//            cerr << "Invalid square size " << squareSize << endl;
//            goodInput = false;
//        }
//        if (nrFrames <= 0)
//        {
//            cerr << "Invalid number of frames " << nrFrames << endl;
//            goodInput = false;
//        }
//        
//        if (input.empty())      // Check for valid input
//            inputType = INVALID;
//        else
//        {
//            if (input[0] >= '0' && input[0] <= '9')
//            {
//                stringstream ss(input);
//                ss >> cameraID;
//                inputType = CAMERA;
//            }
//            else
//            {
//                if (readStringList(input, imageList))
//                {
//                    inputType = IMAGE_LIST;
//                    nrFrames = (nrFrames < (int)imageList.size()) ? nrFrames : (int)imageList.size();
//                }
//                else
//                    inputType = VIDEO_FILE;
//            }
//            if (inputType == CAMERA)
//                inputCapture.open(cameraID);
//            if (inputType == VIDEO_FILE)
//                inputCapture.open(input);
//            if (inputType != IMAGE_LIST && !inputCapture.isOpened())
//                inputType = INVALID;
//        }
//        if (inputType == INVALID)
//        {
//            cerr << " Input does not exist: " << input;
//            goodInput = false;
//        }
//        
//        flag = CALIB_FIX_K4 | CALIB_FIX_K5;
//        if(calibFixPrincipalPoint) flag |= CALIB_FIX_PRINCIPAL_POINT;
//        if(calibZeroTangentDist)   flag |= CALIB_ZERO_TANGENT_DIST;
//        if(aspectRatio)            flag |= CALIB_FIX_ASPECT_RATIO;
//        
//        if (useFisheye) {
//            // the fisheye model has its own enum, so overwrite the flags
//            flag = fisheye::CALIB_FIX_SKEW | fisheye::CALIB_RECOMPUTE_EXTRINSIC |
//            // fisheye::CALIB_FIX_K1 |
//            fisheye::CALIB_FIX_K2 | fisheye::CALIB_FIX_K3 | fisheye::CALIB_FIX_K4;
//        }
//        
//        calibrationPattern = NOT_EXISTING;
//        if (!patternToUse.compare("CHESSBOARD")) calibrationPattern = CHESSBOARD;
//        if (!patternToUse.compare("CIRCLES_GRID")) calibrationPattern = CIRCLES_GRID;
//        if (!patternToUse.compare("ASYMMETRIC_CIRCLES_GRID")) calibrationPattern = ASYMMETRIC_CIRCLES_GRID;
//        if (calibrationPattern == NOT_EXISTING)
//        {
//            cerr << " Camera calibration mode does not exist: " << patternToUse << endl;
//            goodInput = false;
//        }
//        atImageList = 0;
    }
};

static inline void read(const FileNode& node, Settings& x, const Settings& default_value = Settings()) {
    if(node.empty())
        x = default_value;
    else
        x.read(node);
}

//bool readArguments(int argc, char **argv) {
//    if (argc < 2) {
//        cerr << "Invalid number of arguments" << endl;
//        cerr << "Usage: (in.avi|live[:idx_cam=0]) [intrinsics.yml] [size]" << endl;
//        return false;
//    }
//    
//    TheInputVideo = argv[1];
//    if (argc >= 3)
//        TheIntrinsicFile = argv[2]; //Camera calibration file, not sure!
//    if (argc >= 4)
//        TheMarkerSize = atof(argv[3]);
//    
//    if (argc == 3)
//        cerr << "NOTE: You need makersize to see 3d info!!!!" << endl;
//    return true;
//}
//
//int findParam(std::string param, int argc, char *argv[]) {
//    for (int i = 0; i < argc; i++)
//        if (string(argv[i]) == param)
//            return i;
//    
//    return -1;
//}

/************************************
 *
 *
 *
 *
 ************************************/

void detect(int argc, char **argv){
    
    try {
        
/////////////////////////////////////////////////////////////////////////////////////////////////// parse arguments
        //! [file_read]
        Settings s;
        const string inputSettingsFile = "../../LPS/include/inputSettings.xml";
        FileStorage fs(inputSettingsFile, FileStorage::READ); // Read the settings
        if (!fs.isOpened()) {
            cout << "Could not open the configuration file: \"" << inputSettingsFile << "\"" << endl;
            return;
        }
        
        fs["Settings"] >> s;
        fs.release();                                         // close Settings file
        //! [file_read]
        
        //FileStorage fout("settings.yml", FileStorage::WRITE); // write config as YAML
        //fout << "Settings" << s;
        
        if (!s.goodInput)
        {
            cout << "Invalid input detected. Application stopping. " << endl;
            return;
        }
        
////////////////////////////////////////////////
        
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
            return;
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
        
        do {
            
            // copy image
            
            index++; // number of images captured
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
            
            key = cv::waitKey(waitTime); // wait for key to be pressed
            
            if (isVideoFile)
                TheVideoCapturer.retrieve(TheInputImage);
            
        } while (key != 27 && (TheVideoCapturer.grab() || !isVideoFile));
        
    } catch (std::exception &ex)
    
    {
        cout << "Exception :" << ex.what() << endl;
    }
}
