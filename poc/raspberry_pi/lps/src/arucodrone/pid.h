/*
 * pid.h
 *
 *  Created on: Aug 2, 2016
 *      Author: nikovertovec
 */

#ifndef PID_H_
#define PID_H_

#include <time.h>

class PID {
public:
	PID(double kp, double ki, double kd);
	virtual ~PID();
	double refresh(double error);
	void initClock();
private:
	double _kp; // -  proportional gain
	double _ki; // -  Integral gain
	double _kd; // -  derivative gain
	double _dt; // -  loop interval time in seconds
	double _max; // - maximum value of manipulated variable
	double _min; // - minimum value of manipulated variable
    double _pre_error; //the previous error.
    double _integral; //the integral error.

    //used to clock loop
    struct timespec gettime_now;
    long int last_heartbeat;
};

#endif /* PID_H_ */
