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
	cout << "starting Aruco Drone" << endl << "possible commands are: " << endl;
	cout << "hold" << endl << "land" << endl << "takeoff" << endl << "flyto" << endl << "getpos" << endl;
	ArucoDrone drone;
	drone.initAll();
	while(1){
		drone.fly();
	}
    cout << "finished!"<< endl;
    return 0;
}
