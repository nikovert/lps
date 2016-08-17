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
#include <raspicam/raspicam_cv.h>
#include <unistd.h>
#include "arucodrone.h"

using namespace cv;
using namespace aruco;

raspicam::RaspiCam_Cv Camera; //Camera object
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
    string TheIntrinsicFile;
    double TheMarkerSize;
    int Matwidth;
    Mat pid_matrix;
    
    void read(const FileNode& node){
        node["TheIntrinsicFile"] >> TheIntrinsicFile;
        node["TheMarkerSize"] >> TheMarkerSize;
        node["Matwidth"] >> Matwidth;
        node["pid_matrix"] >> pid_matrix;
        validate();
    }
    
    void validate() {
        goodInput = true;
        if (TheIntrinsicFile.empty()){
            cerr << "No intrinsic file provided in settings file" << endl;
            //log_file << "No intrinsic file provided in settings file" << endl;
            goodInput = false;
        }
        if (TheMarkerSize <= 0){
            cerr << "Marker size not provided" << endl;
            //log_file << "Marker size not provided" << endl;
            goodInput = false;
        }
        if (Matwidth <= 0){
			cerr << "Mat width not provided" << endl;
			//log_file << "Mat width not provided" << endl;
			goodInput = false;
		}
        if (pid_matrix.empty()){
			cerr << "WARNING! No PID values where given" << endl;
			//log_file << "WARNING! No PID values where given" << endl;
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


// --------------------------------------------------------------------------
//! @brief initializes the marker detection and the PID controllers
//! @return None
// --------------------------------------------------------------------------
void ArucoDrone::initialize_detection(){
	cout << "Reading settings from xml file" << endl;
	//log_file << "Reading settings from xml file" << endl;
    try {
        //! [file_read]
        Settings s;
        const string inputSettingsFile = "../src/include/inputSettings.xml";
        FileStorage fs(inputSettingsFile, FileStorage::READ); // Read the settings
        if (!fs.isOpened()) {
            cout << "Could not open the configuration file: \"" << inputSettingsFile << "\"" << endl;
            //log_file << "Could not open the configuration file: \"" << inputSettingsFile << "\"" << endl;
            return;
        }
        fs["Settings"] >> s;
        fs.release();  // close Settings file
        
        if (!s.goodInput)
        {
            cout << "Invalid input detected. Application stopping. " << endl;
            //log_file << "Invalid input detected. Application stopping. " << endl;
            return;
        }

        //set camera params
        Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
        
        //Open camera
        cout<<"Opening Camera..."<<endl;
        //log_file<<"Opening Camera..."<<endl;
        
        // check video is open
        if ( !Camera.open()) {
        	cerr<<"Error opening camera"<<endl;
        	//log_file<<"Error opening camera"<<endl;
        	return;
        }
        Camera.grab();
        
        // read first image to get the dimensions
        Camera.retrieve(TheInputImage);
        
        // read camera parameters if passed
        if (s.TheIntrinsicFile != "") {
            TheCameraParameters.readFromXMLFile(s.TheIntrinsicFile);
            TheCameraParameters.resize(TheInputImage.size());
        }
        TheMarkerSize = s.TheMarkerSize;
        Matwidth = s.Matwidth;

    	// PID controllers for X,Y and Z direction
    	//pid_x.set(s.pid_matrix.at<double>(0,0), s.pid_matrix.at<double>(1,0), s.pid_matrix.at<double>(2,0));
		//pid_y.set(s.pid_matrix.at<double>(0,1), s.pid_matrix.at<double>(1,1), s.pid_matrix.at<double>(2,1));
		//pid_z.set(s.pid_matrix.at<double>(0,2), s.pid_matrix.at<double>(1,2), s.pid_matrix.at<double>(2,2));
        
        //Calculates the speed at which the markers are detected
			double tick = (double)getTickCount(); // for checking the speed
			// Detection of markers in the image passed
			MDetector.detect(TheInputImage, TheMarkers, TheCameraParameters, TheMarkerSize);
			// check the speed by calculating the mean speed of all iterations
			AvrgTime.first += ((double)getTickCount() - tick) / getTickFrequency();
			AvrgTime.second++;
			cout << "Time detection=" << 1000 * AvrgTime.first / AvrgTime.second << " milliseconds nmarkers=" << TheMarkers.size() << endl;
			//log_file << "Time detection=" << 1000 * AvrgTime.first / AvrgTime.second << " milliseconds nmarkers=" << TheMarkers.size() << endl;

    } catch (std::exception &ex)
    
    {
        cout << "Exception :" << ex.what() << endl;
        //log_file << "Exception :" << ex.what() << endl;
    }
}

// --------------------------------------------------------------------------
//! @brief can be looped, detects the marker and sets the drone_location and the euler angle
//! @return None
// --------------------------------------------------------------------------
void ArucoDrone::detect(){
    try {
        Camera.grab();
        // Detection of markers in the image passed
        MDetector.detect(TheInputImage, TheMarkers, TheCameraParameters, TheMarkerSize);
        //cout << "nmarkers = " << TheMarkers.size();
        if(TheMarkers.size()>0){
        	Point3d position, position_tmp;
        	Mat rotation, rotation_tmp;
        	getLocation(TheMarkers[0], TheCameraParameters, &position, &rotation, false);
            for (unsigned int i = 1; i < TheMarkers.size(); i++) {
            	getLocation(TheMarkers[i], TheCameraParameters, &position_tmp, &rotation_tmp ,false);
            	position += position_tmp;
            	rotation += rotation_tmp;
            }
            drone_location.x = position.x / TheMarkers.size();
            drone_location.y = position.y / TheMarkers.size();
            drone_location.z = position.z / TheMarkers.size() * -1;
            rot = rotation / TheMarkers.size();

            //cout << "\rDrone position: x = " << drone_location.x << "\ty = " << drone_location.y << "\tz = " << drone_location.z; // "\e[A" to go up a line
        }else{
        	//currently only GPS data works!
        	//get IMU data
        }
        Camera.retrieve (TheInputImage);
    } catch (std::exception &ex){
    	cout << "Exception :" << ex.what() << endl;
    	//log_file << "Exception :" << ex.what() << endl;
    }
}





 
 
