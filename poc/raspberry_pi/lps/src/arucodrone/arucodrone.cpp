/*
 * arucodrone.cpp
 *
 *  Created on: Aug 3, 2016
 *      Author: nikovertovec
 */

#include "arucodrone.h"

using namespace std;



// --------------------------------------------------------------------------
//! @brief   Constructor of the Aruco Drone class
//! @return  None
// --------------------------------------------------------------------------
ArucoDrone::ArucoDrone() :
	tick(0),
	pid_x(0.005,0,0.00),
	pid_y(0.005,0,0.00),
	pid_z(0.0001,0,0),
	holdpos(0,0,-1),
	reset(false)
	{}

//// --------------------------------------------------------------------------
////! @brief writes string to log file
////! @param the string to be logged
////! @return None
//// --------------------------------------------------------------------------
//void log( const string &text ){
//    ofstream log_file(
//        "log_file.txt", ios_base::out | ios_base::app );
//    log_file << text << end;
//}

// --------------------------------------------------------------------------
//! @brief Initializes Drone
//! @return  None
// --------------------------------------------------------------------------
void ArucoDrone::initialize_drone(){
	cout << "Initializing drone" << endl;
    //log_file << "Initializing drone" << endl;
    // Initialize
    if (!open()) {
        cout << "Failed to initialize." << endl;
        //log_file << "Failed to initialize." << endl;
        return;
    }

    // Battery
    cout << "Battery = " << getBatteryPercentage() << "[%]" << endl;
    //log_file << "Battery = " << getBatteryPercentage() << "[%]" << endl;
    // Outdoor mode
    setOutdoorMode(false);

    //set all speeds to zero
    //log_file << "setting up: speed.x = 0; speed.y = 0; speed.z = 0; command = off; drone_location.z = -1;" << endl;
    speed.x = 0;
    speed.y = 0;
    speed.z = 0;
    command = off;
    drone_location.z = -1;
    double m[3][3] = {{-1, 0, 0}, {0, 1, 0}, {0, 0, -1}};
	camerarot = cv::Mat(3, 3, CV_64F, m).inv();
    return;
}

// --------------------------------------------------------------------------
//! @brief   Destructor of the Aruco Drone class
//! @return  None
// --------------------------------------------------------------------------
ArucoDrone::~ArucoDrone() {
	close();
}


// --------------------------------------------------------------------------
//! @brief the main loop function during the flight that executes all other functions
//! @return None
// --------------------------------------------------------------------------
void ArucoDrone::fly(){
	int x = -1;
	// detect marker and updates the drone_location
	if(!onGround())
		x = detect();

    switch(command){
    	case off:
    		speed.x = 0;
			speed.y = 0;
			speed.z = 0;
    		break;
    	case hold:
    		if (onGround()){
    			cout << "can not hold position because the drone is still on the ground" << endl;
    			//log_file << "can not hold position because the drone is still on the ground" << endl;
    			command = off;
    			cout << "command changed to off" << endl;
    			break;
    		}
    		if(drone_location.z == -1){
    			command = off;
				cout << "command changed to off" << endl;
    		}
    		if(getAltitude() > 1.8){
    			command = off;
    		}
        	if(holdpos.z > 0) flytocoords(holdpos);
        	break;
    	case land:
    		landing();
    		//log_file << "setting command to off" << endl;
    		command = off;
    		cout << "command changed to off" << endl;
    		break;
    	case start:
    		if (onGround()) takeoff();
    		else{
    			if(drone_location.z == -1 && getAltitude() < 0.5){
    				//cout << "flying up, altitude = " << getAltitude() << endl;
    				move3D(0, 0, 1, 0);
    			}
    			if(drone_location.z != -1){
    				holdpos = drone_location;
    				//log_file << "setting command to hold" << endl;
    				command = off; // tmp measure, should be changed to hold
    				reset = true;
    				cout << "command changed to off" << endl;
    			}
    		}
    		break;
    }

    //if no marker was detected, don't fly
	if(x == -1){
		speed.x = 0;
		speed.y = 0;
		speed.z = 0;
	}

    //this will be the move function
	move3D(speed.x, speed.y, speed.z, 0); //currently not able to rotate

	//in the case, that the new speed isn't set
	speed.x = 0;
	speed.y = 0;
	speed.z = 0;

    if(reset){
    	pid_x.reset();
    	pid_y.reset();
    	pid_z.reset();
    	reset = false;
    }

    tick++;
}

// --------------------------------------------------------------------------
//! @brief Initializes the Ar.Drone, the Marker Detector, the PID controllers and the Commandline input thread
//! @return None
// --------------------------------------------------------------------------
void ArucoDrone::initAll(){
	//Initialize the AR Drone
	//log_file << "initialize_drone();" << endl;
	initialize_drone();

	//Initialize the Camera and the marker detect function
	//log_file << "initialize_drone();" << endl;
	initialize_detection();

	//Initialize PID clock
	//log_file << "pid_x.initClock();" << endl;
	pid_x.initClock();
	//log_file << "pid_y.initClock();" << endl;
	pid_y.initClock();
	//log_file << "pid_z.initClock();" << endl;
	pid_z.initClock();

	//Initialize thread to get input
	//log_file << "initialize_thread();" << endl;
	initialize_thread();

	cout << "PID settings:" << endl;
	cout << "PID X: p = " << pid_x.kp() << " i = " << pid_x.ki() << " d = " << pid_x.kd() << endl;
	cout << "PID Y: p = " << pid_y.kp() << " i = " << pid_y.ki() << " d = " << pid_y.kd() << endl;
	cout << "PID Z: p = " << pid_z.kp() << " i = " << pid_z.ki() << " d = " << pid_z.kd() << endl;
}

// --------------------------------------------------------------------------
//! @brief returns the location of the drone according to the GPS
//! @return a 3D Point of the drones location
// --------------------------------------------------------------------------
cv::Point3d ArucoDrone::get_GPS_position(){
	double x,y,z;
	getPosition(&x,&y,&z);
	return cv::Point3d(x,y,z);
}
