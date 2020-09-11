#include "talos/control.h"

DriveControlBase::DriveControlBase(std::map<std::string, pros::Motor*> motors, pros::Controller* controller, std::map<std::string, pros::controller_analog_e_t> channels,
  std::vector<std::string> keys, double max_rpm){
  //define drive drive_keys
  drive_keys = keys;
  drive_controller = controller;
  drive_channels = channels;
  //itterate over vector list
  for(std::string i : drive_keys){
    //get pointer to motor
    drive_motors[i] = motors[i];
  }
  max_drive_rpm = max_rpm;
}

//define stopmotor fuctions
void DriveControlBase::stop_motors(){
  for(std::string i : drive_keys){
    drive_motors[i]->move_velocity(0);
  }
}

void HolonomicDriveControl::update_speed(){
  int x = (int)(((double)drive_controller->get_analog(drive_channels["lx"])/128)*max_drive_rpm);
  int y = ((int)(((double)drive_controller->get_analog(drive_channels["ly"])/128)*max_drive_rpm));
  int h = (int)(((double)drive_controller->get_analog(drive_channels["rx"])/128)*max_drive_rpm);
  drive_motors["fl"]->move_velocity((scale*y) + h + x);
  drive_motors["bl"]->move_velocity(y + h - x);
  drive_motors["fr"]->move_velocity((scale*y) - h - x);
  drive_motors["br"]->move_velocity(y - h + x);
}

void DifDriveControl::update_speed(){
  double left = ((double)drive_controller->get_analog(drive_channels["ly"])/128)*max_drive_rpm;
  double right = ((double)drive_controller->get_analog(drive_channels["ry"])/128)*max_drive_rpm;

  drive_motors["fl"]->move_velocity(left);
  drive_motors["bl"]->move_velocity(left);
  drive_motors["fr"]->move_velocity(right);
  drive_motors["br"]->move_velocity(right);
}
