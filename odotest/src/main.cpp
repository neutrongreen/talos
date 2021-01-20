//#define _OV_CONFIG_
#include "main.h"
#include <math.h>
#include "talos.h"
//deifne last time measured variable

//define preficals

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
//https://stackoverflow.com/questions/9323903/most-efficient-elegant-way-to-clip-a-number
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
  move_to_position(0, -0.44);
  move_to_position(-0.74, -0.44);
  move_to_position(-0.74, -0.24);
  in.move_velocity(200);
  pros::delay(1000);
  in.move_velocity(0);
  move_to_position(-0.74, -0.44);
  move_to_position(0.74, -0.44);
  move_rotate(45);
  move_to_position(0.80, -0.34);
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
