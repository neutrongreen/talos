//#define _OV_CONFIG_
#include "main.h"
#include <math.h>
#include "talos.h"
#include "auton_select.h"
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
	init();
	gui_init(0);
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
	auton_id = 2;
	int side = 0;
	if (auton_side == 0){
		side = 1;
	}
	else {
		side = -1;
	}
  /*
  pros::lcd::print(7, "command: 1");
  move_to_position(0, 1);
  pros::lcd::print(7, "command: 2");
  move_to_position(1, 1);
  pros::lcd::print(7, "command: 2");
  move_to_position(0, 0);
  */
  //move_rotate(180);
	//test auton

	switch (auton_id) {
		case 1:
			roll1.move_velocity(-200);
			roll1.move_velocity(-600);
		case 2:
			move_to_position(0.0, -0.4);
			move_to_position(side * -0.8, -0.4);

			move_to_position(side * 0.6, -0.4);
			move_rotate(side * 45);
			move_to_position(side * 0.82, -0.22);
	}

	//move_to_position(1, 0);
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
		#ifdef _OV_CONFIG_
		h = -h;
		#endif

    int left = master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) - master.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
		int right = master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) - master.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
		int overide = master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT) - master.get_digital(pros::E_CONTROLLER_DIGITAL_Y);


	  fl.move_velocity(y + h + x);
	  bl.move_velocity(y + h - x);
	  fr.move_velocity(y - h - x);
	  br.move_velocity(y - h + x);
    //in.move_velocity(intake);
		//update
		#ifdef _OV_CONFIG_
		roll1.move_velocity(200*(left ? left : overide));
		roll2.move_velocity(600*(left ? left : -abs(overide)));

		in1.move_velocity(200*(right ? right : overide));
		in2.move_velocity(200*(right ? right : overide));
		#else
		roll1.move_velocity(200*(-right));
		int bw = master.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
		in1.move_velocity((1 - bw*0.7)*200*(left));
		in2.move_velocity((1 - bw*0.7)*200*(-left));
		#endif
		update_position(dt);
		pros::lcd::print(1, "DX: %f", dx);
		pros::lcd::print(2, "DY: %f", dy);
		//convert to robot point of refrience y rotating negative of the robots current angle
		pros::lcd::print(3, "VX: %f", vx);
		pros::lcd::print(4, "VY: %f", vy);
		pros::lcd::print(2, "DH: %f",  radiansToDegrees(dh));
		pros::delay(20);
	}
}
