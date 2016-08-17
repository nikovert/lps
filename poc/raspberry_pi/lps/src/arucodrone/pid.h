/*
 * pid.h
 *
 *  Created on: Aug 2, 2016
 *      Author: nikovertovec
 */

#ifndef PID_H_
#define PID_H_

#include <time.h>
#include <chrono>

class PID {
public:
	PID(double kp, double ki, double kd);
	virtual ~PID();
	double refresh(double error);
	void initClock();
	void set(double _kp, double _ki, double _kd);
	double kp();
	double ki();
	double kd();
	void reset();
private:
	double _kp; 		// -  proportional gain
	double _ki; 		// -  Integral gain
	double _kd; 		// -  derivative gain
	double _dt; 		// -  loop interval time in milliseconds
	double _max; 		// - maximum value of manipulated variable
	double _min; 		// - minimum value of manipulated variable
    double _pre_error; 	//the previous error.
    double _integral; 	//the integral error.

    std::chrono::high_resolution_clock::time_point previous;
    std::chrono::duration<double, std::milli> timediff();


    //used to clock loop
    //hr_time_point prog_start;
	//hr_duration d;

    //struct timespec now;
    //struct timespec last;
    //struct timespec gettime_now;
    //long int last_heartbeat;
};

#endif /* PID_H_ */
