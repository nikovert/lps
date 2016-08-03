/*
 * pid.h
 *
 *  Created on: Aug 2, 2016
 *      Author: nikovertovec
 */

#ifndef PID_H_
#define PID_H_

class PID {
public:
	PID(double kp, double ki, double kd,  double dt);
	virtual ~PID();
	double refresh(double dronelocation, double destination);

private:
	double _kp; // -  proportional gain
	double _ki; // -  Integral gain
	double _kd; // -  derivative gain
	double _dt; // -  loop interval time
	double _max; // - maximum value of manipulated variable
	double _min; // - minimum value of manipulated variable
    double _pre_error; //the previous error.
    double _integral; //the integral error.
};

#endif /* PID_H_ */
