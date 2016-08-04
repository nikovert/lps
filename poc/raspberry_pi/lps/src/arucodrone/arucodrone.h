/*
 * arucodrone.h
 *
 *  Created on: Aug 3, 2016
 *      Author: nikovertovec
 */

#ifndef ARUCODRONE_H_
#define ARUCODRONE_H_

#include "../ar_drone/ardrone/ardrone.h"
#include "pid.h"
#include <aruco/aruco.h>
#include <aruco/cvdrawingutils.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <raspicam/raspicam_cv.h>
#include <string.h>

class ArucoDrone: public ARDrone {
public:
	ArucoDrone();
	virtual ~ArucoDrone();
	void fly();
	void initialize_drone();
	void initAll();
	cv::Point3d get_GPS_position();

	//detect
	void initialize_detection();
	void detect();

	//move
	void move_straight(int x);
	void move_backward(int x);
	void move_up(int x);
	void move_down(int x);
	void move_port(int x);
	void move_starbourd(int x);
	void move_clockwise(int x);
	void move_anticlockwise(int x);

	bool straight, backward, up, down, port, starbourd, clockwise, anticlockwise, land;
	int int_straight, int_backward, int_up, int_down, int_port, int_starbourd, int_clockwise, int_anticlockwise;

	//pid
	PID pid_x;
	PID pid_y;
	PID pid_z;


	cv::Point3d drone_location;
	double TheMarkerSize;

	//markerlocation
	cv::Point2d getWorldCoordsfromID(int id);
	vector<cv::Point2d> setPixelCoords(aruco::Marker m);
	vector<cv::Point3d> setWorldCoords(cv::Point3d top_left, cv::Point3d top_right, cv::Point3d bottom_left, cv::Point3d bottom_right);
	vector<cv::Point3d> setWorldCoords(int id);
	double distance(int id);
	//to be removed in later versions
	vector<cv::Point3d> setup(int id);

	//flyto
	double distancetofly(cv::Point3d point);
	cv::Point3d vectortofly(cv::Point3d point);
	double getspeed(int x);
	void flyto(cv::Point3d vector);

	//cameralocation
	cv::Mat getLocation(aruco::Marker m, aruco::CameraParameters TheCameraParameters, bool print);

	//commands
	string terminal_input;
	bool getinput;
	void initialize_thread();

private:
	//move
	double vx();
	double vy();
	double vz();
	double vr();
	bool check();
	int tick;
};

#endif /* ARUCODRONE_H_ */
