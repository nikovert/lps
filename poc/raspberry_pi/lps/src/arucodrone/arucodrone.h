/*
 * arucodrone.h
 *
 *  Created on: Aug 3, 2016
 *      Author: nikovertovec
 */

#ifndef ARUCODRONE_H_
#define ARUCODRONE_H_

#include "../ar_drone/ardrone/ardrone.h"

class ArucoDrone: public ARDrone {
public:
	ArucoDrone();
	virtual ~ArucoDrone();
	void fly();

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
	Point2d getWorldCoordsfromID(int id);
	vector<Point2d> setPixelCoords(aruco::Marker m);
	vector<Point3d> setWorldCoords(Point3d top_left, Point3d top_right, Point3d bottom_left, Point3d bottom_right);
	vector<Point3d> setWorldCoords(int id);
	double distance(int id);

	//flyto
	double distancetofly(Point3d point);
	Point3d vectortofly(Point3d point);
	double getspeed(int x);
	void flyto(Point3d vector);

	//cameralocation
	Mat getLocation(Marker m, CameraParameters TheCameraParameters, bool print);

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
