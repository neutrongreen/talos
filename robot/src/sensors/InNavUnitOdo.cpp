//include snsor file
#include "sensors/sensors.h"

//define construct for inu
InNavUnitOdo::InNavUnitOdo(pros::Imu* sensor){
  //set the interal navagtion unti to the poitner to sensor
  inu = sensor;
  //reset sensors
  reset_position();
}

//rest nav unit
void InNavUnitOdo::reset_position(){
  //set postion to zero
  position = Coord();
  velocity = Vector2D();
  //reset calabration
  inu->reset();

  //wait uintill calerbration finshes. should take about 2 seconds
  while(inu->is_calibrating()){
    pros::delay(10);
  }
}


//update postions
void InNavUnitOdo::update_position(int deltatime){
  //get accelromiter values
  pros::c::imu_gyro_s_t accel = inu->get_accel();
  //convert and add acllertomer values to velocity, calcauting the net current speed. times by g to convert to m/s^2 and dived by deltatime
  //to get actual velcoity increase
  //need to add deadbanding
  velocity += Vector2D((accel.x*G)/deltatime, (accel.y*G)/deltatime);
  //update coord.
  //add velocity to postion rotated by current heading
  position.position += velocity.rotate(inu->get_yaw());
  //set current heading to mearured eaidng\.
  position.h = inu->get_yaw();
}
