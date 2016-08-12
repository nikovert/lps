//
//  main.cpp
//  LPS
//
//  Created by Niko Vertovec on 14/07/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#include <iostream>
#include "arucodrone/arucodrone.h"


using namespace std;



// --------------------------------------------------------------------------
//! @brief this is the fist function that must be called
//! @return  0 if all was successful
// --------------------------------------------------------------------------
int main(int argc, char **argv){
	cout << "starting Aruco Drone" << endl << "possible commands are: " << endl;
	cout << "\toff" << endl << "\thold" << endl << "\tland" << endl << "\ttakeoff" << endl << "\tflyto" << endl << "\tgetpos" << endl << "\tshowspeed" << endl << endl;
	ArucoDrone drone;
	drone.initAll();
	cout << "Initialization complete, ready to take commands" << endl;
	while(1){
		drone.fly();
	}
    cout << "finished!"<< endl;
    return 0;
}
