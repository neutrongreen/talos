
//this is the config file defining diffrent robot configs

//defines a map of motors in a enumerator that can be accese

//define config
#ifndef ROBOT_CONFIGH
#define ROBOT_CONFIGH
#include "main.h"
#include <map>
#include "talos/config/config.h"
#include "talos/control.h"
#include "talos/odomentry.h"
//define motors
std::map<std::string, pros::Motor*> motors =
{
  {
    "fl", new pros::Motor(11, false)
  },
  {
    "fr", new pros::Motor(1, true)
  },
  {
    "bl", new pros::Motor(20, false)
  },
  {
    "br", new pros::Motor(10, true)
  },
  {
    "belt_1", new pros::Motor(7, true)
  },
  {
    "belt_2", new pros::Motor(8, true)
  },
  {
    "intake_1", new pros::Motor(9, false)
  },
  {
    "intake_2", new pros::Motor(19, true)
  }
};

//channels
std::map<std::string, pros::controller_analog_e_t> channels =
{
  {
    "lx", pros::E_CONTROLLER_ANALOG_LEFT_X
  },
  {
    "ly", pros::E_CONTROLLER_ANALOG_LEFT_Y
  },
  {
    "rx", pros::E_CONTROLLER_ANALOG_RIGHT_X
  },
  {
    "ry", pros::E_CONTROLLER_ANALOG_RIGHT_Y
  }
};


//controller pointer
pros::Controller* master = new pros::Controller(pros::E_CONTROLLER_MASTER);
//define imu
std::vector<std::string> keys = {"fl", "fr", "bl", "br"};
HolonomicDriveControl drive = HolonomicDriveControl(motors, master, channels, keys, 200);

pros::Imu* navunit = new pros::Imu(2);
InvOdomentry* ImuPositing = new InvOdomentry(navunit);
#endif
