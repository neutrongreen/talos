#include "main.h"
#include <math.h>
#define WHEEL_C 0.3192
#define MAX_RPM 190
#define MAX_AUTON_RPM 180

#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)
#define radiansToDegrees(angleRad) (angleRad / (M_PI / 180.0))
//pid constants
const double kP = 500;
const double kI = 1;
const double kD = 0.0001;
const double deadband = 0.02;
const double intdeadband = 0.02;
const double integral_max =20;

const double rot_kP = 100;
const double rot_kI = 10;
const double rot_kD = 0.0;
const double rot_deadband = degreesToRadians(0.1);
const double rot_intdeadband = degreesToRadians(0.1);
const double rot_integral_max = 40;
//define displacement variables
double dx = 0;
double dy = 0;
double dh = 0;
//define velocity varaibles
double vx = 0;
double vy = 0;
//deifne last time measured variable

//define preficals
pros::Motor fl(10);
pros::Motor bl(1);
pros::Motor fr(20, true);
pros::Motor br(11, true);
pros::Motor in(7, true);
pros::Imu gyro(6);
pros::Controller master(pros::E_CONTROLLER_MASTER);
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
//https://stackoverflow.com/questions/9323903/most-efficient-elegant-way-to-clip-a-number
 double clip(double n, double lower, double upper) {
   return std::max(lower, std::min(n, upper));
 }

//define update robot update_position
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
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

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
		ox = (ox * cos(-dh) - oy * sin(-dh));
		oy = (ox * sin(-dh) + oy * cos(-dh));
		pros::lcd::print(3, "OrX: %f", ox);
		pros::lcd::print(4, "OrY: %f", oy);

		//get angle of vector from the x axis of the robot
		double oh = atan2(oy, ox);
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
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
  gyro.reset();
  pros::delay(200);
  while(gyro.is_calibrating()){
    pros::delay(20);
    master.rumble("-");
  }
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  //test autonomous
  /*
  pros::lcd::print(7, "command: 1");
  move_to_position(0, 1);
  pros::lcd::print(7, "command: 2");
  move_to_position(1, 1);
  pros::lcd::print(7, "command: 2");
  move_to_position(0, 0);
  */
  //move_rotate(180);
  move_to_position(0, 0.20);
  in.move_velocity(200);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	//estimate horsiontail and vertical velocity of motors


	double time = ((double)pros::millis())/1000;
	while (true) {
		double newtime = ((double)pros::millis())/1000;
		double dt = newtime-time;
		time = newtime;
		//move init_motors
		int x = (int)(((double)master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X)/128)*MAX_RPM);
	  int y = ((int)(((double)master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)/128)*MAX_RPM));
	  int h = (int)(((double)master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)/128)*MAX_RPM);
    int intake = MAX_RPM*((master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) - master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)));
	  fl.move_velocity(y + h + x);
	  bl.move_velocity(y + h - x);
	  fr.move_velocity(y - h - x);
	  br.move_velocity(y - h + x);
    in.move_velocity(intake);
		//update postion
		update_position(dt);

		pros::delay(20);
	}
}
