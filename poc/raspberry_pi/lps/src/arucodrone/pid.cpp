/*
 * pid.cpp
 *
 *  Created on: Aug 2, 2016
 *      Author: nikovertovec
 */

#include "pid.h"
#include <cmath>
#include <iostream>

// for readability
using hr_clock = std::chrono::high_resolution_clock;
using hr_time_point = hr_clock::time_point;
using hr_duration = hr_clock::duration;
using millisec = std::chrono::duration<double, std::milli>;

// --------------------------------------------------------------------------
//! @brief   Constructor of the PID controller class
//! @return  values of the PID controller
// --------------------------------------------------------------------------
double PID::kp(){
	return _kp;
}

double PID::ki(){
	return _ki;
}

double PID::kd(){
	return _kd;
}
// --------------------------------------------------------------------------
//! @brief   Constructor of the PID controller class
//! @return  None
// --------------------------------------------------------------------------
PID::PID(double kp, double ki, double kd):
	_kp(kp),
	_ki(ki),
	_kd(kd),
	_dt(0),
	_max(5.0), //the maximum speed
	_min(-5.0), //the minimum speed (opposite direction)
	_integral(0), //the integral starts at 0
	_pre_error(0)
	{ }

// --------------------------------------------------------------------------
//! @brief   Destructor of the PID controller class
//! @return  None
// --------------------------------------------------------------------------
PID::~PID() { }

// --------------------------------------------------------------------------
//! @brief calculates the time since the the function was last called
//! @return time since the last person was called
// --------------------------------------------------------------------------
millisec PID::timediff(){
    // save time between function call
    previous = hr_clock::now();

    // get current time
    hr_time_point current = hr_clock::now();

    // get the time difference
    millisec ms = std::chrono::duration_cast<millisec>(current - previous);

    // store current time for next call
    previous = current;

    // return elapsed time in milliseconds
    return ms;
}

// --------------------------------------------------------------------------
//! @brief Initializes the Clock
//! @return  None
// --------------------------------------------------------------------------
void PID::initClock(){
	//SETUP HEARTBEAT TIMER
	//clock_gettime(CLOCK_REALTIME, &gettime_now);
	//last_heartbeat = gettime_now.tv_nsec;
	//clock_gettime(CLOCK_REALTIME, &last);

	timediff();
	//prog_start = hr_clock::now();
}

void PID::set(double kp, double ki, double kd){
	_kp = kp;
	_ki = ki;
	_kd = kd;
}

// --------------------------------------------------------------------------
//! @brief calculates the speed according to the distance to fly
//! @param the distance to fly
//! @return the speed from -5 (backwards) to 5 (forwards)
// --------------------------------------------------------------------------
double PID::refresh(double error){
	//calculate time difference

	// find the duration
	//d = hr_clock::now() - prog_start;

	// cast the duration to milliseconds
	_dt =  timediff().count();

	//std::cout << "dt: " << _dt << std::endl;
//	clock_gettime(CLOCK_REALTIME, &now);
//	clock_gettime(CLOCK_REALTIME, &gettime_now);
//	std::cout << "now.tv_sec: " << now.tv_sec  << std::endl;
//	std::cout << "last.tv_sec: " << last.tv_sec << std::endl;
//	_dt = ((now.tv_sec - last.tv_sec) * BILLION) + (now.tv_nsec - last.tv_nsec);
//
//	//_dt = (gettime_now.tv_nsec - last_heartbeat);  // 1000000000
//	last_heartbeat = gettime_now.tv_nsec;
//	std::cout << "caluculated time: " << _dt << std::endl;

	//Proportional
	double p = error * _kp; //accounts for present values of the error.

	//Integral
	_integral = _integral + error;
	double i = _integral * _ki; //accounts for past values of the error.

	//Derivative
	double derivative = (error - _pre_error) / _dt;
	double d = derivative * _kd; //accounts for possible future values of the error, based on its current rate of change.
	if(std::isnan(d)){
		std::cerr << "Error in PID::refresh, cannot except d, NaN" << std::endl;
		d  = 0;
	}

	//calculate output;
	double output = p + i + d;

	//saves current error as previous error
	_pre_error = error;
	
	//clock_gettime(CLOCK_REALTIME, &last);
	//prog_start = hr_clock::now();

	//limit to min or max
	if(output > _max)
		return _max;
	if(output < _min)
		return _min;

	return output;
}
