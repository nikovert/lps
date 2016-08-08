//
//  markerlocation.cpp
//  detect_marker
//
//  Created by Niko Vertovec on 14/07/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#include "arucodrone.h"
#include <math.h>

using namespace cv;

vector<Point3d> world_coords;


// --------------------------------------------------------------------------
//! @brief need to set width of the aruco board(length of X-aches)
//! @param the id of the Aruco marker
//! @return  2D coordinates of the location of the Marker on the Mat
// --------------------------------------------------------------------------
Point2d ArucoDrone::getWorldCoordsfromID(int id){
    int x = (id-1)%Matwidth;
    int y = (id-1)/Matwidth;
    Point2d pos(x, y);
    return pos;
}

// --------------------------------------------------------------------------
//! @brief converts 2D coordinates to 3D coordinates (z = 0)
//! @param the 2D coordinates
//! @return  3D coordinates with z = 0
// --------------------------------------------------------------------------
Point3d convertCoords(Point2d cor){
    return Point3d(cor.x, cor.y, 0);
}

// --------------------------------------------------------------------------
//! @brief set the pixel coordinates of a markers corners
//! @param the Aruco marker
//! @return a vector of the 2D coordinates of the marker corners
// --------------------------------------------------------------------------
vector<Point2d> ArucoDrone::setPixelCoords(aruco::Marker m){
    vector<Point2d> pixel_coords;
    pixel_coords.push_back (m[0]);
    pixel_coords.push_back (m[1]);
    pixel_coords.push_back (m[2]);
    pixel_coords.push_back (m[3]);
    return pixel_coords;
}


// --------------------------------------------------------------------------
//! @brief sets the world coordinates of a markers corners
//! @param the coordinates of the corners of the marker
//! @return a vector of the §D coordinates of the marker corners
// --------------------------------------------------------------------------
vector<Point3d> ArucoDrone::setWorldCoords(Point3d top_left, Point3d top_right, Point3d bottom_left, Point3d bottom_right){
    vector<Point3d> world_coords;
    world_coords.push_back (top_left);
    world_coords.push_back (top_right);
    world_coords.push_back (bottom_left);
    world_coords.push_back (bottom_right);
    return world_coords;
}


// --------------------------------------------------------------------------
//! @brief sets the world coordinates of a marker according to its id
//! @param the id of the aruco marker
//! @return  a vector of the 3D coordinates of the marker corners with z = 0
// --------------------------------------------------------------------------
vector<Point3d> ArucoDrone::setWorldCoords(int id){
    vector<Point3d> world_coords;
    if(id==50){
        return world_coords = setWorldCoords(Point3d(-8.89,12.7,0),Point3d(0,12.7,0),Point3d(0,3.81,0),Point3d(-8.89,3.81,0));
    }
    if(id==698){
        return world_coords = setWorldCoords(Point3d (0, 0, 0), Point3d (8.89, 0, 0), Point3d (8.89, -8.89, 0), Point3d (0, -8.89, 0));
    }
    Point2d topleft = getWorldCoordsfromID(id);
    double size = TheMarkerSize;
    world_coords.push_back (Point3d(topleft.x, topleft.y, 0));
    world_coords.push_back (Point3d(topleft.x+size, topleft.y, 0));
    world_coords.push_back (Point3d(topleft.x, topleft.y+size, 0));
    world_coords.push_back (Point3d (topleft.x+size, topleft.y+size, 0));
    return world_coords;
}

// --------------------------------------------------------------------------
//! @brief calculates the distance to a marker accrording to its known position (if set with id)
//! @param the id of the aruco marker
//! @return the distance of the marker to the drone (in cm)
// --------------------------------------------------------------------------
double ArucoDrone::distance(int id){
    Point3d marker = convertCoords(getWorldCoordsfromID(id));
    return sqrt(pow(marker.x - drone_location.x, 2) + pow(marker.y - drone_location.y, 2) + pow(marker.z - drone_location.z, 2));
}


// --------------------------------------------------------------------------
//! @brief sets up a markers coordinates, needed if a marker is placed outside of its id coordinates
//! @param the id of the aruco marker
//! @return a vector of the §D coordinates of the marker corners
// --------------------------------------------------------------------------
vector<Point3d> ArucoDrone::setup(int id){
    if(id==50){
        world_coords = setWorldCoords(Point3d(-8.89,12.7,0),Point3d(0,12.7,0),Point3d(0,3.81,0),Point3d(-8.89,3.81,0));
    }
    if(id==698){
        world_coords = setWorldCoords(Point3d (0, 0, 0), Point3d (8.89, 0, 0), Point3d (8.89, -8.89, 0), Point3d (0, -8.89, 0));
    }
    return world_coords;
}
