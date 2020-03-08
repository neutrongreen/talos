#include "sensors/sensors.h"

// define constructor
DriveMechOdo::DriveMechOdo(pros::Motor* lf, pros::Motor* lb, pros::Motor* rf, pros::Motor* rb, double a, double b){
  //set motor map
  d_motors["lf"] = lf;
  d_motors["lr"] = lb;
  d_motors["rf"] = rf;
  d_motors["rb"] = rb;
  //set prev value std::map<, value> map to zero;
  //zero all d_motors and set encoder units to rotation
  d_motors["lf"]->set_encoder_units(pros::E_MOTOR_ENCODER_ROTATIONS);
  d_motors["lr"]->set_encoder_units(pros::E_MOTOR_ENCODER_ROTATIONS);
  d_motors["rf"]->set_encoder_units(pros::E_MOTOR_ENCODER_ROTATIONS);
  d_motors["rb"]->set_encoder_units(pros::E_MOTOR_ENCODER_ROTATIONS);
  //reset motor positions
  reset_position();
}

//cacluate new postion using odometic functiosn found in Nagatani et a c2000
void DriveMechOdo::update_position(){
  //define drive motor change for last postion
  double d1 = d_motors["lf"]->get_position() - pre_rot["lf"];
  double d2 = d_motors["rf"]->get_position() - pre_rot["rf"];
  double d3 = d_motors["lb"]->get_position() - pre_rot["lb"];
  double d4 = d_motors["rb"]->get_position() - pre_rot["rb"];

  //define velocity vector
  Vector2D velocity = Vector2D();
  //caclualte x velcoty using equaiosn. aplha is a clerbrated value
  velocity.x = 1/4*(-d1+d2+d3-d4) * tan(alpha);
  //caclucate y velocity usiing equasion.
  velocity.y = 1/4*(d1+d2+d3+d4);
  //cacluate temparty heading
  double heading = (-d1+d2-d3+d4) * beta;
  //roate velocity to current robot refrencefram orientation
  velocity.rotate(position.h);
  //add velcotiy rotate to postion
  position.position += velocity;
  //add heading
  position.h += heading;
  //set previous postion to current postion.
  pre_rot["lf"] += d1;
  pre_rot["lr"] += d2;
  pre_rot["rf"] += d3;
  pre_rot["rb"] += d4;
}

void DriveMechOdo::reset_position(){
  position = Coord();
  pre_rot["lf"] = 0;
  pre_rot["lr"] = 0;
  pre_rot["rf"] = 0;
  pre_rot["rb"] = 0;
  d_motors["lf"]->tare_position();
  d_motors["lr"]->tare_position();
  d_motors["rf"]->tare_position();
  d_motors["rb"]->tare_position();
}
