#include "talos.h"


double dx = 0;
double dy = 0;
double dh = 0;
//define velocity varaibles
double vx = 0;
double vy = 0;

double clip(double n, double lower, double upper) {
  return std::max(lower, std::min(n, upper));
}
/*
void update_position_x(double dt){
  //calculate robot velocity
  double v1 = ((fl.get_actual_velocity() + br.get_actual_velocity())/2) * ROT_CONST;
  double v2 = ((fr.get_actual_velocity() + bl.get_actual_velocity())/2) * ROT_CONST;
  vy =  v1 + v2;
  vx = v1 - v2;
  //convert to meters per seconds
  dh = gyro.get_heading();
  if (dh < 0){
    dh += 360;
  }
  dh = degreesToRadians(dh);
  vy = (vy/60)*WHEEL_C;
  vx = (vx/60)*WHEEL_C;
  dx += (vx * cos(-dh) - vy * sin(-dh)) * dt;
  dy += (vx * sin(-dh) + vy * cos(-dh)) * dt;
}
*/

//update the robots postion
void update_position(double dt){
  //calculate robot velocity
  vy = (fl.get_actual_velocity() + fr.get_actual_velocity() + bl.get_actual_velocity() + br.get_actual_velocity())/4;
  vx = -(-fl.get_actual_velocity() + fr.get_actual_velocity() + bl.get_actual_velocity() - br.get_actual_velocity())/4;
  //convert to meters per seconds
  dh = degreesToRadians(gyro.get_heading());

  vy = (vy/60)*WHEEL_C;
  vx = (vx/60)*WHEEL_C;
  dx += (vx * cos(-dh) - vy * sin(-dh)) * dt;
  dy += (vx * sin(-dh) + vy * cos(-dh)) * dt;
}

//move to postion of robot
void move_to_position(double tx, double ty){
	//t  for target
	bool at_target = false;
	//init pid values
	double integral = 0;
	double last_error = 0;
	double time = ((double)pros::millis())/1000;
	while(!at_target){
		//get delta time
		double newtime = ((double)pros::millis())/1000;
		double dt = newtime-time;
		time = newtime;
		//update update_position
		update_position(dt);

		//calculate offet vector from target
		double ox = tx-dx;
		double oy = ty-dy;
		pros::lcd::print(1, "OX: %f", ox);
		pros::lcd::print(2, "OY: %f", oy);
		//convert to robot point of refrience y rotating negative of the robots current angle
		pros::lcd::print(3, "OrX: %f", ox);
		pros::lcd::print(4, "OrY: %f", oy);

		//get angle of vector from the x axis of the robot
		double oh = atan2(oy, ox);
    if (oh < 0){
      oh += M_PI_2;
    }
    oh -= dh;
		pros::lcd::print(5, "OH: %f", oh);
		//pros::lcd::print(3, "oh: %.2f", oh);
		//calculate mangnatue needed through pid loop
		//calcualte error/magnatue of vector
		double error = sqrt((pow(ox, 2) + pow(oy, 2)));
		pros::lcd::print(6, "error: %f", error);
		//calculate integral
		integral += error*dt;
		//stop integral from overloading
		if(error < intdeadband){
			integral = 0;
		}
		if(error < deadband){
			at_target = true;
		}
		//limit max integral value
		integral = clip(integral, -integral_max, integral_max);
		double deriv = (error-last_error)/dt;
    last_error = error;
		double pid_mag = error*kP + integral*kI + deriv*kD;
		pid_mag = clip(pid_mag, -MAX_AUTON_RPM, MAX_AUTON_RPM);
		//tahnks https://seamonsters-2605.github.io/archive/mecanum/
		//speed scale of front left and back right
		int flbr_speed = (int)round(sin(oh + M_PI/4) * pid_mag);
		int frbl_speed = (int)round(sin(oh - M_PI/4) * pid_mag);
		//set speed of motors
		fl.move_velocity(flbr_speed);
		br.move_velocity(flbr_speed);
		fr.move_velocity(frbl_speed);
		bl.move_velocity(frbl_speed);
		//detemrien mangantue of speed
		pros::delay(20);
	}
	//stop all motors
	fl.move_velocity(0);
	br.move_velocity(0);
	fr.move_velocity(0);
	bl.move_velocity(0);
}


//rotate robot
void move_rotate(double th){
  //t  for target
  th = degreesToRadians(th);
  bool at_target = false;
  //init pid values
  double integral = 0;
  double last_error = 0;
  double time = ((double)pros::millis())/1000;
  while(!at_target){
    //get delta time
    double newtime = ((double)pros::millis())/1000;
    double dt = newtime-time;
    time = newtime;
    update_position(dt);

    double error = std::fmod((th-dh + 3*M_PI), (2*M_PI)) - M_PI;
    pros::lcd::print(1, "error: %f", radiansToDegrees(error));
    //calculate integral
    integral += error*dt;
    //stop integral from overloading
    if(error < rot_intdeadband && error > -rot_intdeadband){
      integral = 0;
    }
    if(error < rot_deadband && error > -rot_deadband){
      at_target = true;
    }
    //limit max integral value
    integral = clip(integral, -rot_integral_max, rot_integral_max);
    double deriv = (error-last_error)/dt;
    double pid_mag = error*rot_kP + integral*rot_kI + deriv*rot_kD;

    pid_mag = clip(pid_mag, -MAX_AUTON_RPM, MAX_AUTON_RPM);
    int speed = (int)round(pid_mag);
    pros::lcd::print(2, "speed: %d", speed);
    fl.move_velocity(speed);
    bl.move_velocity(speed);
    fr.move_velocity(-speed);
    br.move_velocity(-speed);

    pros::delay(20);
  }
  fl.move_velocity(0);
  br.move_velocity(0);
  fr.move_velocity(0);
  bl.move_velocity(0);
}
