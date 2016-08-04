//
//  main.cpp
//  LPS
//
//  Created by Niko Vertovec on 14/07/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#include <iostream>
#include "ar_drone/drone.h"
#include "detect_marker/location.h"
#include "arucodrone/arucodrone.h"

using namespace std;

// --------------------------------------------------------------------------
//! @brief this is the fist function that must be called
//! @param None
//! @return  0 if all was successful
// --------------------------------------------------------------------------
int main(int argc, char **argv){
	ArucoDrone drone;
	cv::Point3d point = drone.drone_location;

//	if(argc > 1){
//		if(strcmp(argv[1], "test_connection") == 0){
//			cout << "testing connection, drone will activate for 3 seconds" << endl;
//			testconnection();
//			cout << "finished!"<< endl;
//			return 0;
//		}
//	}

	while(1){
		drone.fly();
	}
    cout << "finished!"<< endl;
    return 0;
}
