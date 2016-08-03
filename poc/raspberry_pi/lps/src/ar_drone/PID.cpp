/*
 * PID.cpp
 *
 *  Created on: Aug 2, 2016
 *      Author: nikovertovec
 */

#include "PID.h"

/**
 * Implementation
 */
PID::PID(double kp, double ki, double kd,  double dt) {
	_kp(kp);
	_ki(ki);
	_kd(kd);
	_dt(dt);
	_max(5); //the maximum speed
	_min(0); //the minimum speed
	_integral(0); //the integral starts at 0
}

/**
 * destructor
 */
PID::~PID() {
	// TODO Auto-generated destructor stub
}

/**
 * calculates the speed
 */
double PID::refresh(double dronelocation, double destination){

	//calculate current error
	double error = dronelocation-destination;

	//Proportional
	double p = error * _kp; //accounts for present values of the error.

	//Integral
	_integral = _integral+error;
	double i = _integral*_ki; //accounts for past values of the error.

	//Derivative
	double derivative = (error - _pre_error) / _dt;
	double d = derivative * _kd; //accounts for possible future values of the error, based on its current rate of change.

	//calculate output;
	double output = p + i + d;

	//limit to min or max
	if(output > _max)
		return _max;
	if(output < _min)
		return _min;

	return output;










}
