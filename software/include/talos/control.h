#ifndef _ROBOT_CONTROL
#include "main.h"
#include <map>
#include <vector>
#include "talos/datatypes.h"

#define _ROBOT_CONTROL

class DriveControlBase{
  public:
    DriveControlBase(std::map<std::string, pros::Motor*> motors, pros::Controller* controller, std::map<std::string, pros::controller_analog_e_t> channels,
      std::vector<std::string> keys, double max_rpm = 200);
    void stop_motors();
    void update_speed();
  protected:
    std::vector<std::string> drive_keys;
    std::map<std::string, pros::Motor*> drive_motors;
    pros::Controller* drive_controller;
    std::map<std::string, pros::controller_analog_e_t> drive_channels;
    double max_drive_rpm;
};

class HolonomicDriveControl : public DriveControlBase
{
  public:
    //macure ure motos are speciped. joysick order can be in any order
    void update_speed();
    HolonomicDriveControl(std::map<std::string, pros::Motor*> motors, pros::Controller* controller, std::map<std::string, pros::controller_analog_e_t> channels,
      std::vector<std::string> keys, double max_rpm = 200, int forward_scale = 1) : DriveControlBase(motors, controller, channels, keys, max_rpm) {scale = forward_scale;};
  protected:
    int scale;
};

class DifDriveControl : public DriveControlBase
{
  public:
    //macure ure motos are speciped. joysick order can be in any order
    void update_speed();
    DifDriveControl(std::map<std::string, pros::Motor*> motors, pros::Controller* controller, std::map<std::string, pros::controller_analog_e_t> channels,
      std::vector<std::string> keys, double max_rpm = 200) : DriveControlBase(motors, controller, channels, keys, max_rpm) {};
};
#endif
