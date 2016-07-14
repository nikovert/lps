//
//  markerlocation.cpp
//  detect_marker
//
//  Created by Niko Vertovec on 14/07/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#include "location.h"

using namespace cv;

vector<Point3d> world_coords;

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

vector<Point3d> setup(int id){
    if(id==50){
        world_coords = setWorldCoords(Point3d(-8.89,12.7,0),Point3d(0,12.7,0),Point3d(0,3.81,0),Point3d(-8.89,3.81,0));
    }
    if(id==698){
        world_coords = setWorldCoords(Point3d (0, 0, 0), Point3d (8.89, 0, 0), Point3d (8.89, -8.89, 0), Point3d (0, -8.89, 0));
    }
    return world_coords;
}