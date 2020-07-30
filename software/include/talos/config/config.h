//this is the config file defining diffrent robot motor intilastion

//define motor initlization fuction
#ifndef MOTOR_CONFIGH
#include "main.h"
#include <map>

void init_motors(std::map<std::string, pros::Motor*>* motors);
#endif
