//
//  move.hpp
//  ar_drone
//
//  Created by Niko Vertovec on 14/07/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#ifndef move_h
#define move_h

extern bool straight, backward, up, down, port, starbourd, clockwise, anticlockwise, land;
extern int int_straight, int_backward, int_up, int_down, int_port, int_starbourd, int_clockwise, int_anticlockwise;

void move_straight(int x);

void move_backward(int x);

void move_up(int x);

void move_down(int x);

void move_port(int x);

void move_starbourd(int x);

void move_clockwise(int x);

void move_anticlockwise(int x);

double vx();

double vy();

double vz();

double vr();

bool check();

#endif /* move_h */
