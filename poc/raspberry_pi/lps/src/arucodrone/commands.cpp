/*
 * commands.cpp
 *
 *  Created on: Aug 3, 2016
 *      Author: nikovertovec
 */

#include "arucodrone.h"
#include <thread>

bool getinput;

// --------------------------------------------------------------------------
//! @brief parses the input from the terminal
//! @return  an integer representing the command
// --------------------------------------------------------------------------
int parseinput(string terminal_input, cv::Point3d *holdpos, cv::Point3d *drone_location, int *command, cv::Point3d *speed){
	if(terminal_input.compare("off") == 0) return -2;
	if(terminal_input.compare("hold") == 0) return -1;
	if(terminal_input.compare("land") == 0) return 0;
	if(terminal_input.compare("takeoff") == 0) return 1;
	if(terminal_input.compare("start") == 0) return 1;
	if(terminal_input.compare("getpos") == 0) {
		std::cout << "Drone is currently at: " << *drone_location << std::endl;
		return *command;
	}
	if(terminal_input.compare("showspeed") == 0) {
			std::cout << "Drone speed currently: " << speed << std::endl;
			return *command;
		}
	if(terminal_input.compare("flyto") == 0){
		cv::Point3d point;
		std::cout << std::endl << "Please enter the x coordinates: ";
		std::cin >> point.x;
		std::cout << std::endl << "Please enter the y coordinates: ";
		std::cin >> point.y;
		std::cout << std::endl << "Please enter the z coordinates: ";
		std::cin >> point.z;
		*holdpos = point;
		return -1;
	}
	return *command;
}

// --------------------------------------------------------------------------
//! @brief waits for input from user, used by separate thread
//! @return  None
// --------------------------------------------------------------------------
void input(int *command, cv::Point3d *holdpos, cv::Point3d *drone_location, int *prev_command, cv::Point3d *speed){
	string terminal_input;
	while(getinput){
		std::getline(std::cin, terminal_input);
		*prev_command = *command;
		*command = parseinput(terminal_input, holdpos, drone_location, command, speed); //must be checked if it works
		if(*prev_command != *command)
			cout << "command changed" << endl;;
	}
}

// --------------------------------------------------------------------------
//! @brief initilizes a new thread that waits for a user input
//! @return  None
// --------------------------------------------------------------------------
void ArucoDrone::initialize_thread(){
	getinput = true;
	std::thread t1(input, &command, &holdpos, &drone_location, &prev_command, &speed);
	t1.detach();
}




