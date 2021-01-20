#ifndef _TALOS_CONTROL_
#define _TALOS_CONTROL_

#include "api.h"
#include "config.h"
//define postion variables
extern double dx;
extern double dy;
extern double dh;
//define velocity varaibles
extern double vx;
extern double vy;

//define fuctions
void move_to_position(double tx, double ty);
void move_rotate(double th);
void update_position(double dt);

#endif