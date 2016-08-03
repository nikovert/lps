/*
 * arucodrone.cpp
 *
 *  Created on: Aug 3, 2016
 *      Author: nikovertovec
 */

#include "arucodrone.h"

using namespace std;

// --------------------------------------------------------------------------
//! @brief   Initialisizes Drone, first time only
//! @param None
//! @return  None
// --------------------------------------------------------------------------
ArucoDrone::ArucoDrone(){
    tick= 0;
    land = false;
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

    // PID controllers for X,Y and Z direction
    PID pid_x(0,0,0,0);
    PID pid_y(0,0,0,0);
   	PID pid_z(0,0,0,0);

   	// Initialize the Camera and the detection
	initialize_detection();

	Point3d drone_location;
}

ArucoDrone::~ArucoDrone() {
	close();
}

void ArucoDrone::fly(){

    // Take off
    if (onGround()) takeoff();

    // detect marker and updates the drone_location
    detect();

    move3D(vx(), vy(), vz(), vr());

    if(check()){
        landing();
    }

    // optional altitude check
    if(altitude) cout << "Altitude: " << ardrone.getAltitude() << endl;

    tick++;
}

// --------------------------------------------------------------------------
//! @brief returns the location of the drone according to the GPS
//! @param None
//! @return a 3D Point of the drones location
// --------------------------------------------------------------------------
cv::Point3d ArucoDrone::get_GPS_position(){
	double x,y,z;
	ardrone.getPosition(&x,&y,&z);
	return cv::Point3d(x,y,z);
}

