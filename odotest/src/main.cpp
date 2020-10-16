#include "main.h"
#include <math.h>
#define WHEEL_C 0.3192
#define MAX_RPM 190
#define PI 3.14159265

#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
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
void autonomous() {}

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
	double dx = 0;
	double dy = 0;
	double dh = 0;

	double vx = 0;
	double vy = 0;

	pros::Motor fl(10);
	pros::Motor bl(20);
	pros::Motor fr(1, true);
	pros::Motor br(11, true);
	pros::Imu gyro(6);
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	gyro.reset();
	pros::delay(200);
	while(gyro.is_calibrating()){
		pros::delay(20);
		master.rumble("-");
	}
	double time = ((double)pros::millis())/1000;
	while (true) {
		//move init_motors
		int x = (int)(((double)master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X)/128)*MAX_RPM);
	  int y = ((int)(((double)master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)/128)*MAX_RPM));
	  int h = (int)(((double)master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)/128)*MAX_RPM);
	  fl.move_velocity(x + h + y);
	  bl.move_velocity(x + h - y);
	  fr.move_velocity(x - h - y);
	  br.move_velocity(x - h + y);
		//update postion
		double newtime = ((double)pros::millis())/1000;
		double dt = newtime-time;
		time = newtime;
		//calculate robot velocity
		vy = (fl.get_actual_velocity() + fr.get_actual_velocity() + bl.get_actual_velocity() + br.get_actual_velocity())/4;
		vx = (-fl.get_actual_velocity() + fr.get_actual_velocity() + bl.get_actual_velocity() - br.get_actual_velocity())/4;
		//convert to meters per seconds
		dh = degreesToRadians(gyro.get_heading());

		vy = (vy/60)*WHEEL_C;
		vx = (vx/60)*WHEEL_C;
		dx += (vx * cos(-dh) - vy * sin(-dh)) * dt;
		dy += (vx * sin(-dh) + vy * cos(-dh)) * dt;

		pros::lcd::print(1, "X: %.2f", dx);
		pros::lcd::print(2, "y: %.2f", dy);
		pros::lcd::print(3, "h: %.2f", dh);
		pros::delay(20);
	}
}
