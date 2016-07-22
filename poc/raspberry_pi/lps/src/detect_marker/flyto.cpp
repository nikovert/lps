//
//  flyto.cpp
//  lbs
//
//  Created by Niko Vertovec on 16/07/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#include "location.h"
#include <math.h>

using namespace cv;

int distancetofly(Point3d point){
    return sqrt(pow(point.x - drone_location.x, 2) + pow(point.y - drone_location.y, 2) + pow(point.z - drone_location.z, 2));
}

Point3d vectortofly(Point3d point){
    return Point3d((point.x - drone_location.x), (point.y - drone_location.y), (point.z - drone_location.z));
}