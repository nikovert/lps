//
//  move.cpp
//  ar_drone
//
//  Created by Niko Vertovec on 14/07/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#include "arucodrone.h"
#include <iostream>

using namespace std;

//bool straight, backward, up, down, port, starbourd, clockwise, anticlockwise, land;
//int int_straight, int_backward, int_up, int_down, int_port, int_starbourd, int_clockwise, int_anticlockwise;

bool output_movement = false;


// --------------------------------------------------------------------------
//! @brief these functions prepare the drone to be moved in a particular direction
//! @param how long the drone should move in the required direction
//! @return None
// --------------------------------------------------------------------------

void ArucoDrone::move_straight(int x){
    if(output_movement)cout << "moving " << "forward" << endl;
    int_straight = x;
    straight = true;
}

void ArucoDrone::move_backward(int x){
    if(output_movement)cout << "moving " << "backward" << endl;
    int_backward = x;
    backward = true;
}

void ArucoDrone::move_up(int x){
    if(output_movement)cout << "moving " << "up" << endl;
    int_up = x;
    up = true;
}

void ArucoDrone::move_down(int x){
    if(output_movement)cout << "moving " << "down" << endl;
    int_down = x;
    down = true;
}

void ArucoDrone::move_port(int x){
    if(output_movement)cout << "moving " << "left" << endl;
    int_up = x;
    port = true;
}

void ArucoDrone::move_starbourd(int x){
    if(output_movement)cout << "moving " << "right" << endl;
    int_up = x;
    starbourd = true;
}

void ArucoDrone::move_clockwise(int x){
    if(output_movement)cout << "turning " << "clockwise" << endl;
    int_clockwise = x;
    clockwise = true;
}

void ArucoDrone::move_anticlockwise(int x){
    if(output_movement)cout << "turning " << "anticlockwise" << endl;
    int_anticlockwise = x;
    anticlockwise = true;
}

// --------------------------------------------------------------------------
//! @brief checks if the drone needs to move forward or backward and returns by how much
//! @param None
//! @return how fast the drone should move on x-aches
// --------------------------------------------------------------------------
double ArucoDrone::vx(){
    double vx = 0;
    double speed = getspeed(0);
    if(straight){
        vx = speed;
        int_straight--;
    }
    
    if(backward){
        vx = -speed;
        int_backward--;
    }
    return vx;
}

// --------------------------------------------------------------------------
//! @brief checks if the drone needs to move left or right and returns by how much
//! @param None
//! @return how fast the drone should move on y-aches
// --------------------------------------------------------------------------
double ArucoDrone::vy(){
    double vy = 0;
    double speed = getspeed(1);
    if(port){
        vy = speed;
        int_port--;
    }
    
    if(starbourd){
        vy = -speed;
        int_starbourd--;
    }
    return vy;
}

// --------------------------------------------------------------------------
//! @brief checks if the drone needs to move up or down and returns by how much
//! @param None
//! @return how fast the drone should move on z-aches
// --------------------------------------------------------------------------
double ArucoDrone::vz(){
    double vz = 0;
    double speed = getspeed(2);
    if(up){
        vz = speed;
        int_up--;
    }
    
    if(down){
        vz = -speed;
        int_down--;
    }
    return vz;
}

// --------------------------------------------------------------------------
//! @brief checks if the drone needs to be turned and returns by how much
//! @param None
//! @return how fast to rotate
// --------------------------------------------------------------------------
double ArucoDrone::vr(){
    double vr = 0;
    double speed = getspeed(3);
    if(clockwise){
        vr = speed;
        int_clockwise--;
    }
    
    if(anticlockwise){
        vr = -speed;
        int_anticlockwise--;
    }
    return vr;
}


// --------------------------------------------------------------------------
//! @brief checks if the drone still needs to be moved
//! @param None
//! @return if the drone should land
// --------------------------------------------------------------------------
bool ArucoDrone::check(){
    if(int_straight == 0){
        straight = false;
    }

    if(int_backward == 0){
        backward = false;
    }

    if(int_up == 0){
        up = false;
    }

    if(int_down == 0){
        down = false;
    }

    if(port == 0){
        int_port = false;
    }

    if(int_starbourd == 0){
        starbourd = false;
    }

    if(int_clockwise == 0){
        clockwise = false;
    }

    if(int_anticlockwise == 0){
        anticlockwise = false;
    }
    
    return false;
}
