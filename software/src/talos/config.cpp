#include "talos/config/config.h"

//definine intilaizte motors fuction
void init_motors(std::map<std::string, pros::Motor*> motors){

  //get itterator

  //itterate over itterotor
  for (auto& [key, i] : motors){
    i->tare_position();
    i->set_voltage_limit(10000);
    i->set_encoder_units(pros::E_MOTOR_ENCODER_ROTATIONS);
  }
    //set motor values

    //itterate up one spac
}
