//
//  main.cpp
//  ar_drone
//
//  Created by Niko Vertovec on 28/06/16.
//  Using the AR Drone SDK
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#include <iostream>

//ARDroneLib
#include </Users/nikovertovec/Documents/lps/lps/poc/ar_drone/ARDroneLib/Soft/Lib/ardrone_tool/ardrone_tool.c>

//Common
#include <config.h>
#include <ardrone_tool/ardrone_api.c>

int ardrone_tool_main(int argc, char **argv);

int test(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    char **test;
    return ardrone_tool_main(argc, test);
}
