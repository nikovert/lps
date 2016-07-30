//
//  main.cpp
//  LPS
//
//  Created by Niko Vertovec on 14/07/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#include <iostream>
#include "ar_drone/drone.h"
#include "detect_marker/location.h"

using namespace std;

// --------------------------------------------------------------------------
//! @brief this is the fist function that must be called
//! @param None
//! @return  0 if all was successful
// --------------------------------------------------------------------------
int main(int argc, char **argv){
    while(1){
        fly();
    }
    cout << "finished"<< endl;
    return 0;
}
