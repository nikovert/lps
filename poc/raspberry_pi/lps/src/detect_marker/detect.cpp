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
#include <raspicam/raspicam_cv.h>
#include <unistd.h>

using namespace cv;
using namespace aruco;

bool detection_init = false;
raspicam::RaspiCam_Cv Camera; //Camera object
string TheIntrinsicFile;
double TheMarkerSize = -1;
int ThePyrDownLevel;
MarkerDetector MDetector;
vector< Marker > TheMarkers;
Mat TheInputImage, TheInputImageCopy;
CameraParameters TheCameraParameters;
void cvTackBarEvents(int pos, void *);
bool readCameraParameters(string TheIntrinsicFile, CameraParameters &CP, Size size);

pair< double, double > AvrgTime(0, 0); // determines the average time required for detection
double ThresParam1, ThresParam2;
int iThresParam1, iThresParam2;
int waitTime = 0;

//saves inputs form xml file
class Settings{
public:
    Settings() : goodInput(false) {}
    bool goodInput;
    
    void read(const FileNode& node){
        node["TheIntrinsicFile"] >> TheIntrinsicFile;
        node["TheMarkerSize"] >> TheMarkerSize;
        validate();
    }
    
    void validate() {
        goodInput = true;
        if (TheIntrinsicFile.empty()){
            cerr << "No intrinsic file provided in settings file" << endl;
            goodInput = false;
        }
        if (TheMarkerSize <= 0){
            cerr << "Marker size not provided " << endl;
            goodInput = false;
        }
    }
};


// --------------------------------------------------------------------------
//! @brief reads xml settings file
//! @param the Settings
//! @return  None
// --------------------------------------------------------------------------
static inline void read(const FileNode& node, Settings& x, const Settings& default_value = Settings()) {
    if(node.empty())
        x = default_value;
    else
        x.read(node);
}

//Mat average(vector<Mat> positions){
//    Mat tvec;
//    for(int i = 0; i<positions.size();i++){
//        tvec += positions[i];
//    }
//    tvec/=positions.size();
//    return tvec;
//}


// --------------------------------------------------------------------------
//! @brief converts a Mat to a 3d point, not universal, do not use!!
//! @param a 1x3 Matrix
//! @return  a 3D Point
// --------------------------------------------------------------------------
Point3d MatPoint(Mat mat){
    if(mat.rows==3){
        return Point3d(mat.at<double>(0, 0), mat.at<double>(1, 0), mat.at<double>(2, 0));
    }
    if(mat.cols==3){
        return Point3d(mat.at<double>(0, 0), mat.at<double>(1, 0), mat.at<double>(2, 0));
    }
    cerr << "Mat size is wrong!";
    return Point3d(0,0,0);
}

// --------------------------------------------------------------------------
//! @brief initializes the marker detection, must be executed first time
//! @param None
//! @return None
// --------------------------------------------------------------------------
void initialize_detection(){
    try {
        //! [file_read]
        Settings s;
        const string inputSettingsFile = "../src/include/inputSettings.xml";
        FileStorage fs(inputSettingsFile, FileStorage::READ); // Read the settings
        if (!fs.isOpened()) {
            cout << "Could not open the configuration file: \"" << inputSettingsFile << "\"" << endl;
            return;
        }
        
        fs["Settings"] >> s;
        fs.release();  // close Settings file
        
        if (!s.goodInput)
        {
            cout << "Invalid input detected. Application stopping. " << endl;
            return;
        }

        //set camera params
        Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
        
        //Open camera
        cout<<"Opening Camera..."<<endl;
        
        // check video is open
        if ( !Camera.open()) {cerr<<"Error opening camera"<<endl;return ;}
        Camera.grab();
        
        // read first image to get the dimensions
        Camera.retrieve(TheInputImage);
        
        // read camera parameters if passed
        if (TheIntrinsicFile != "") {
            TheCameraParameters.readFromXMLFile(TheIntrinsicFile);
            TheCameraParameters.resize(TheInputImage.size());
        }
        // Configure other parameters
        if (ThePyrDownLevel > 0)
            MDetector.pyrDown(ThePyrDownLevel);
        
        int index = 0;
                index++; // number of images captured
                double tick = (double)getTickCount(); // for checking the speed
                // Detection of markers in the image passed
                MDetector.detect(TheInputImage, TheMarkers, TheCameraParameters, TheMarkerSize);
                // check the speed by calculating the mean speed of all iterations
                AvrgTime.first += ((double)getTickCount() - tick) / getTickFrequency();
                AvrgTime.second++;
                cout << "\rTime detection=" << 1000 * AvrgTime.first / AvrgTime.second << " milliseconds nmarkers=" << TheMarkers.size() << std::flush;
        
        //Camera.release(); needs to be called when done
        
        detection_init = true;
        
        cout << endl;
    } catch (std::exception &ex)
    
    {
        cout << "Exception :" << ex.what() << endl;
    }
}


// --------------------------------------------------------------------------
//! @brief can be looped, detects the marker and sets the drone_location
//! @param None
//! @return None
// --------------------------------------------------------------------------
void detect(){
    try {
        if(!detection_init){
            cout << "initialize_detection()" << endl;
            initialize_detection();
        }
        Camera.grab();
        // Detection of markers in the image passed
        MDetector.detect(TheInputImage, TheMarkers, TheCameraParameters, TheMarkerSize);
        
        Mat position;
        
        if(TheMarkers.size()>0){
            position = getLocation(TheMarkers[0], TheCameraParameters, false);
            for (unsigned int i = 1; i < TheMarkers.size(); i++) {
                add(getLocation(TheMarkers[i], TheCameraParameters, false), position, position);
            }
            position /= TheMarkers.size();
            drone_location = MatPoint(position);
            cout << "\rDrone position: x = " << drone_location.x << "\ty = " << drone_location.y << "\tz = " << drone_location.z; // "\e[A" to go up a line
        }else{
        	//currently only GPS data works!
        	//get IMU data
        }
        Camera.retrieve (TheInputImage);
        //detect();
    } catch (std::exception &ex){ cout << "Exception :" << ex.what() << endl; }
}





 
 
