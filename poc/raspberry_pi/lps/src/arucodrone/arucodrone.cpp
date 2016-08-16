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
	pid_x(0.01,0,0),
	pid_y(0.00,0,0),
	pid_z(0.00,0,0),
	holdpos(0,0,-1.0)
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
    double m[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	camerarot = Mat(3, 3, CV_64F, m).inv();
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

    // detect marker and updates the drone_location
    detect();

    //this will be the move function
    move3D(speed.x, -speed.y, speed.z, 0); //currently not able to rotate

    //move.cpp and this function will be removed in later versions
    //move3D(vx(), vy(), vz(), vr());

    //check will be removed in later versions
    //check();

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
    			break;
    		}
        	if(holdpos.z > 0) flytocoords(holdpos);
        	break;
    	case land:
    		landing();
    		//log_file << "setting command to off" << endl;
    		command = off;
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
    				command = hold;
    			}
			break;
    		}
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
