//
//  main.cpp
//  LPS
//
//  Created by Niko Vertovec on 14/07/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#include <iostream>
#include <thread>
#include "ar_drone/drone.h"
#include "detect_marker/location.h"
#include "arucodrone/arucodrone.h"

using namespace std;

bool boolexit;
// --------------------------------------------------------------------------
//! @brief waits for input from user, used by separate thread
//! @param None
//! @return  None
// --------------------------------------------------------------------------
void input(){
	int s;
	cin >> s;
	if(s==5){
		cout << "s is 5" <<endl;
		boolexit = true;
	}
	else
		cout << "s is not 5" <<endl;
}

// --------------------------------------------------------------------------
//! @brief this is the fist function that must be called
//! @param None
//! @return  0 if all was successful
// --------------------------------------------------------------------------
int main(int argc, char **argv){
	boolexit = false;
	thread t1(input);
	int tick = 0;
	while(!boolexit){
	if(tick == 0) cout << "looping" << endl;
		tick++;
	}

	ArucoDrone drone;
	drone.initAll();

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
