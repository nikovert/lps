//
//  markerlocation.cpp
//  detect_marker
//
//  Created by Niko Vertovec on 14/07/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#include "location.h"
#include <math.h>

using namespace cv;

vector<Point3d> world_coords;

//need to set width (length of X-aches)
int width = 8; //we choose 8 for now
Point2d getWorldCoordsfromID(int id){
    int x = id%width;
    int y = id/8;
    Point2d pos(x, y);
    return pos;
}

Point3d convertCoords(Point2d cor){
    return Point3d(cor.x, cor.y, 0);
}

vector<Point2d> setPixelCoords(aruco::Marker m){
    vector<Point2d> pixel_coords;
    pixel_coords.push_back (m[0]);
    pixel_coords.push_back (m[1]);
    pixel_coords.push_back (m[2]);
    pixel_coords.push_back (m[3]);
    return pixel_coords;
}

vector<Point3d> setWorldCoords(Point3d top_left, Point3d top_right, Point3d bottom_left, Point3d bottom_right){
    vector<Point3d> world_coords;
    world_coords.push_back (top_left);
    world_coords.push_back (top_right);
    world_coords.push_back (bottom_left);
    world_coords.push_back (bottom_right);
    return world_coords;
}

vector<Point3d> setWorldCoords(int id){
    vector<Point3d> world_coords;
    if(id==50){
        return world_coords = setWorldCoords(Point3d(-8.89,12.7,0),Point3d(0,12.7,0),Point3d(0,3.81,0),Point3d(-8.89,3.81,0));
    }
    if(id==698){
        return world_coords = setWorldCoords(Point3d (0, 0, 0), Point3d (8.89, 0, 0), Point3d (8.89, -8.89, 0), Point3d (0, -8.89, 0));
    }
    Point2d center = getWorldCoordsfromID(id);
    double size = TheMarkerSize/2;
    world_coords.push_back (Point3d(center.x-size, center.y-size, 0));
    world_coords.push_back (Point3d(center.x+size, center.y-size, 0));
    world_coords.push_back (Point3d(center.x-size, center.y+size, 0));
    world_coords.push_back (Point3d (center.x+size, center.y+size, 0));
    return world_coords;
}


Point3d MatPoint(Mat mat){
    if(mat.rows==3){
        return Point3d(mat.at<double>(0, 0), mat.at<double>(1, 0), mat.at<double>(2, 0));
    }
    if(mat.cols==3){
        return Point3d(mat.at<double>(0, 0), mat.at<double>(1, 0), mat.at<double>(2, 0));
    }
    cerr << "Mat size is wrong!";
    return Point3d(0,0,0);
}

double distance(int id){
    Point3d marker = convertCoords(getWorldCoordsfromID(id));
    Point3d drone = MatPoint(drone_location);
    return sqrt(pow(marker.x - drone.x, 2) + pow(marker.y - drone.y, 2) + pow(marker.z - drone.z, 2));
}

vector<Point3d> setup(int id){
    if(id==50){
        world_coords = setWorldCoords(Point3d(-8.89,12.7,0),Point3d(0,12.7,0),Point3d(0,3.81,0),Point3d(-8.89,3.81,0));
    }
    if(id==698){
        world_coords = setWorldCoords(Point3d (0, 0, 0), Point3d (8.89, 0, 0), Point3d (8.89, -8.89, 0), Point3d (0, -8.89, 0));
    }
    return world_coords;
}