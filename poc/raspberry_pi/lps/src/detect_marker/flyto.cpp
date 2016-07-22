//
//  flyto.cpp
//  lbs
//
//  Created by Niko Vertovec on 16/07/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#include "location.h"
#include <math.h>
#include "../ar_drone/move.h"

using namespace cv;

//calculate distance to point
int distancetofly(Point3d point){
    return sqrt(pow(point.x - drone_location.x, 2) + pow(point.y - drone_location.y, 2) + pow(point.z - drone_location.z, 2));
}

//calculate vector to point
Point3d vectortofly(Point3d point){
    return Point3d((point.x - drone_location.x), (point.y - drone_location.y), (point.z - drone_location.z));
}


//fly along a vector
void flyto(Point3d vector){
    if(vector.x>0){
        int_straight = vector.x;
        straight = true;
    }
    
    if(vector.x<0){
        int_backward = vector.x * -1;
        backward = true;
    }
    
    if(vector.y>0){
        int_port = vector.y;
        port = true;
    }
    
    if(vector.y<0){
        int_starbourd = vector.y * -1;
        starbourd = true;
    }
    
    if(vector.z>0){
        int_up = vector.z;
        up = true;
    }
    
    if(vector.z<0){
        int_down = vector.z * -1;
        down = true;
    }
}
