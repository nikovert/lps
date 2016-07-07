//
//  lps_marker.cpp
//  detect_marker
//
//  Created by Niko Vertovec on 27/06/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#include <stdio.h>
#include "aruco.h"
#include <math.h>

using namespace cv;
using namespace aruco;

class lps_marker{
    public:
    Marker m;
    vector<Point3d>     world_coords;
    vector<Point2d>     pixel_coords;
    
    void marker(Marker marker){
        m = marker;
        setPixelCoords();
    }
    
    void cast(Marker marker, Point3d top_left, Point3d top_right, Point3d bottom_left, Point3d bottom_right){
        m = marker;
        setPixelCoords();
        setWorldCoords(top_left, top_right, bottom_left, bottom_right);
    }
    
    void setWorldCoords(Point3d top_left, Point3d top_right, Point3d bottom_left, Point3d bottom_right){
        world_coords.push_back (top_left);
        world_coords.push_back (top_right);
        world_coords.push_back (bottom_left);
        world_coords.push_back (bottom_right);
    }
    
    void setPixelCoords(){
        int a = sqrt(m.getArea())/2;
        pixel_coords.push_back (Point2d (m.getCenter().x + a, m.getCenter().y + a));
        pixel_coords.push_back (Point2d (m.getCenter().x + a, m.getCenter().y - a));
        pixel_coords.push_back (Point2d (m.getCenter().x - a, m.getCenter().y + a));
        pixel_coords.push_back (Point2d (m.getCenter().x - a, m.getCenter().y - a));
    }
};

