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


// --------------------------------------------------------------------------
//! @brief calculate distance to point
//! @param a point in the realworld
//! @return the distance to the point
// --------------------------------------------------------------------------
double ArucoDrone::distancetofly(Point3d point){
    return sqrt(pow(point.x - drone_location.x, 2) + pow(point.y - drone_location.y, 2) + pow(point.z - drone_location.z, 2));
}


// --------------------------------------------------------------------------
//! @brief calculate vector to point
//! @param a point in the realworld
//! @return a vector that points to the point
// --------------------------------------------------------------------------
Point3d ArucoDrone::vectortofly(Point3d point){
    return Point3d((point.x - drone_location.x), (point.y - drone_location.y), (point.z - drone_location.z));
}

// --------------------------------------------------------------------------
//! @brief calculate required speed to marker
//! @param which aches speed is wanted (0=x, 1=y, 2=z, 3=r)
//! @return the speed at which to fly to the marker
// --------------------------------------------------------------------------
double ArucoDrone::getspeed(int x){
	switch(x){
	case 0:
		return 5;
	case 1:
		return 5;
	case 2:
		return 5;
	case 3:
		return 5;
	}
}



// --------------------------------------------------------------------------
//! @brief fly along a vector
//! @param a vector that should be flown
//! @return  None
// --------------------------------------------------------------------------
void ArucoDrone::flyto(Point3d vector){
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
