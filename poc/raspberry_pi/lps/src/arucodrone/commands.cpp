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
int parseinput(string terminal_input){
	if(terminal_input.compare("hold") == 0) return -1;
	if(terminal_input.compare("land") == 0) return 0;
	if(terminal_input.compare("takeoff") == 0) return 1;
	return -1;
}

// --------------------------------------------------------------------------
//! @brief waits for input from user, used by separate thread
//! @return  None
// --------------------------------------------------------------------------
void input(int &command){
	string terminal_input;
	while(getinput){
		std::getline(std::cin, terminal_input);
		*command = parseinput(terminal_input); //must be checked if it works
	}
}

// --------------------------------------------------------------------------
//! @brief initilizes a new thread that waits for a user input
//! @return  None
// --------------------------------------------------------------------------
void ArucoDrone::initialize_thread(){
	getinput = true;
	command(-1);
	std::thread t1(input, &command);
}




