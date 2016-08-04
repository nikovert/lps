/*
 * commands.cpp
 *
 *  Created on: Aug 3, 2016
 *      Author: nikovertovec
 */

#include "arucodrone.h"
#include <thread>

string terminal_input;
bool getinput;

// --------------------------------------------------------------------------
//! @brief waits for input from user, used by separate thread
//! @param None
//! @return  None
// --------------------------------------------------------------------------
void input(){
	while(getinput)
		std::getline(std::cin, terminal_input);
}

// --------------------------------------------------------------------------
//! @brief initilizes a new thread that waits for a user input
//! @param None
//! @return  None
// --------------------------------------------------------------------------
void ArucoDrone::initialize_thread(){
	std::thread t1(input);
}



