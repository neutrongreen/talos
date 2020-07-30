
//this is the config file defining diffrent robot configs

//defines a map of motors in a enumerator that can be accese

//define config
#ifndef ROBOT_CONFIGH

#include "main.h"
#include <map>
#include "talos/config/config.h"

//define motors
std::map<std::string, pros::Motor*> motors =
{
  {
    "fl", new pros::Motor(1, true)
  },
  {
    "fr", new pros::Motor(10, true)
  },
  {
    "bl", new pros::Motor(11, true)
  },
  {
    "bl", new pros::Motor(20, true)
  }
};

//define imu
pros::Imu* navunit = new pros::Imu(5);
#endif
