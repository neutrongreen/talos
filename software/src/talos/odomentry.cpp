#include "talos/odomentry.h"

//define constructor for holonomic drive odomeerty default class
HolonomicDriveOdo::HolonomicDriveOdo(std::map<std::string, pros::Motor*> motors, std::vector<std::string> keys){
  //define drive drive_keys
  drive_keys = keys;
  //itterate over vector list
  for(std::string i : drive_keys){
    //get pointer to motor
    robot_mtrs[i] = motors[i];
    //create lastrot entry with current wheel positon should be zero but adds security
    last_rot[i] = robot_mtrs[i]->get_position();
  }
}
//define get rotation changed
double HolonomicDriveOdo::get_rot_change(std::string motor){
  double mtr_rot = robot_mtrs[motor]->get_position();
  double rotation = M_PI*2*(mtr_rot - last_rot[motor]);
  last_rot[motor] = mtr_rot;
  return rotation;
}

//very very long fuction definition calling base bonstruction and setting a few custom values
MeccanumDriveOdo::MeccanumDriveOdo(std::map<std::string, pros::Motor*> motors, int wheel_separation_width, int wheel_separation_height, double wheel_radius, std::vector<std::string> keys) : HolonomicDriveOdo(motors, keys){
  separation_width = wheel_separation_width;
  separation_height = wheel_separation_height;
  radius = wheel_radius;
}

//usign equasions found http://robotsforroboticists.com/drive-kinematics/
Crd MeccanumDriveOdo::update_position(){
  lasttime = pros::millis();
  Vector2D temploc = Vector2D(0, 0);
  //cacluate templocation
  temploc.x = (get_rot_change("fl") + get_rot_change("fr") + get_rot_change("bl") + get_rot_change("br")) * (radius/4);
  temploc.y = (-get_rot_change("fl") + get_rot_change("fr") + get_rot_change("bl") - get_rot_change("br")) * (radius/4);
  //calcualte roation
  double rot = (-get_rot_change("fl") + get_rot_change("fr") - get_rot_change("bl") + get_rot_change("br")) * (radius/(4 * (separation_width + separation_height)));
  //rotate by refrence frame
  pose.position += temploc.rotate(-pose.h);
  //change refrence frame
  pose.h += rot;
  //return current posrion
  return pose;
}

InvOdomentry::InvOdomentry(pros::Imu* interal_unit){
    //store rCefrence to sensor
    navunit = interal_unit;

}
void InvOdomentry::calibrate(){
  //reset navunit
  navunit->reset();
  //wait unit done reseting
  while(navunit->is_calibrating()){
    pros::delay(200);
  }

  double sumx = 0;
  double sumy = 0;
  //wait unill below infinty
  while(navunit->get_accel().x > 4 || navunit->get_accel().x < -4){
    pros::delay(20);
  }
  //get average value
  for(int i = 0; i < 20; i++){
    pros::c::imu_gyro_s_t accel = navunit->get_accel();

    sumx += accel.x;
    sumy += accel.y;
    pros::lcd::print(1, "X: %.2f", accel.x);
    pros::lcd::print(2, "Y: %.2f", accel.y);
    pros::delay(50);
  }
  offsetx = sumx/20;
  offsety = sumy/20;
}


Crd InvOdomentry::update_position(){
  //get delta time

  u_int32_t currenttime = pros::millis();
  u_int32_t deltatime = currenttime-lasttime;
  double deltatimef = (double)(deltatime)/1000;
  //get rotation
  double rot = navunit->get_yaw();
  //get accelromiter values
  pros::c::imu_gyro_s_t accel = navunit->get_accel();
  //convert and add acllertomer values to velocity, calcauting the net current speed. times by g to convert to m/s^2 and dived by deltatime
  //to get actual velcoity increase
  //subtract clareated average offset
  Vector2D accelvec = Vector2D(accel.x-offsetx, accel.y-offsety)*G;
  //insert at start of array
  //buf.insert(buf.begin(), accelvec);
  //buf.resize(bufsize);
  //shrink buffer to cosnatn bufsize
  //get average
  //Vector2D sumvec = Vector2D(0, 0);
  //for (Vector2D i : buf){
    //sumvec += i;
  //}
  //accelvec = sumvec/bufsize;
  //send mps value through kalman filteredControllerInput


  //update Crd.
  //rotate accelration to starting refrence
  //convert to displacemd

  pose.position = velocity*deltatimef + (accelvec*0.5)*(deltatimef*deltatimef);
  //set current heading to mearured eaidng\
  //set velocty to accleration times deltatime
  velocity = accelvec*deltatimef + velocity;
  //set pose rot
  pose.h = rot;
  //set lastitme to current time
  lasttime = currenttime;
  //return pose

  acceleration = accelvec;

  return pose;
}

LogOdo::LogOdo(std::map<std::string, OdomentryBaseClass*> odomentry){
  sensors = odomentry;
}

void LogOdo::log_data(){
  //if file not open
  logfile = fopen("/usd/data.txt", "a");
  if(logfile){
    for(auto [name, i] : sensors){
      fprintf(logfile, "%s, %f, %f, %f, %f,\n", name.c_str(), (double)(i->lasttime)/1000, i->pose.h, i->pose.position.x, i->pose.position.y);
    }
  }
  fclose(logfile);
}
