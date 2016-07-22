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

int main(int argc, char **argv){
    while(1){
        fly();
    }
    cout << "finished"<< endl;
    return 0;
}
