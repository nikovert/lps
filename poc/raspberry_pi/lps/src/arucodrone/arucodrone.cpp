/*
 * arucodrone.cpp
 *
 *  Created on: Aug 3, 2016
 *      Author: nikovertovec
 */

#include "arucodrone.h"

using namespace std;

cv::Point3d drone_location;

// --------------------------------------------------------------------------
//! @brief   Constructor
// --------------------------------------------------------------------------
ArucoDrone::ArucoDrone():
	tick(0),
	land(false),
	// PID controllers for X,Y and Z direction
	pid_x(0,0,0,0),
	pid_y(0,0,0,0),
	pid_z(0,0,0,0)
	{}


// --------------------------------------------------------------------------
//! @brief   Initialisizes Drone, first time only
//! @param None
//! @return  None
// --------------------------------------------------------------------------
void ArucoDrone::initialize_drone(){
    cout << "Initializing drone" << endl;
    // Initialize
    if (!open()) {
        cout << "Failed to initialize." << endl;
        return;
    }

    // Battery
    cout << "Battery = " << getBatteryPercentage() << "[%]" << endl;

    // Outdoor mode
    setOutdoorMode(true);

    return;
}

ArucoDrone::~ArucoDrone() {
	close();
}

int s;
void ArucoDrone::fly(){

    // Take off
    if (onGround()) takeoff();

    // detect marker and updates the drone_location
    detect();

    //mov.cpp and this function will be removed in later versions
    move3D(vx(), vy(), vz(), vr());

    //cin with a switch will be used to take commands during flight
    cin >> s;
    if(s==5)
    	cout << "cin worked :)" << endl;

    //check will be removed in later versions
    if(check()){
        landing();
    }

    tick++;
}

// --------------------------------------------------------------------------
//! @brief Initializes the Ar Drone and the Marker Detector
//! @param None
//! @return None
// --------------------------------------------------------------------------
void ArucoDrone::initAll(){
	//Initialize the AR Drone
	initialize_drone();
	//Initialize the Camera and the marker detect function
	initialize_detection();
	//Initialize PID clock
	pid_x.initClock();
	pid_y.initClock();
	pid_z.initClock();
}




// --------------------------------------------------------------------------
//! @brief returns the location of the drone according to the GPS
//! @param None
//! @return a 3D Point of the drones location
// --------------------------------------------------------------------------
cv::Point3d ArucoDrone::get_GPS_position(){
	double x,y,z;
	getPosition(&x,&y,&z);
	return cv::Point3d(x,y,z);
}


















