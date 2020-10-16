#include "main.h"
#include <math.h>
#define G 9.8
#define FCONST 0.2
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */

//imu

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
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Imu sensor(2);

	sensor.reset();
	pros::delay(200);
	double x = 0;
	double y = 0;
	double vx = 0;
	double vy = 0;

	while(sensor.is_calibrating()){
		pros::delay(200);
	}

	double time = ((double)pros::millis())/1000;

	while (true) {
		//get accelration
		pros::c::imu_accel_s_t accel = sensor.get_accel();


		//get delta time
		double newtime = ((double)pros::millis())/1000;
		double dt = newtime-time;
		time = newtime;
		double heading = sensor.get_heading();
		//convert to mps
		double ax = round(accel.x*G*10)/10;
		double ay = round(accel.y*G*10)/10;

		//cacluate new displacement from 0
		x += (vx * dt) + (0.5 * (ax) * (dt * dt));
		y += (vy * dt) + (0.5 * (ay) * (dt * dt));

		//calculate new velocity
		vx = (ax)*dt + vx;
		vy = (ay)*dt + vy;

		if(vx < 0){
			vx -= fmaxf(-FCONST, vx);
		}
		else{
			vx -= fminf(FCONST, vx);
		}

		if(vy < 0){
			vy -= fmaxf(-FCONST, vy);
		}
		else{
			vy -= fminf(FCONST, vy);
		}

		pros::lcd::print(0, "AX: %f", ax);
		pros::lcd::print(1, "AY: %f", ay);
		pros::lcd::print(2, "VX: %f", vx);
		pros::lcd::print(3, "VY: %f", vy);
		pros::lcd::print(4, "DX: %f", x);
		pros::lcd::print(5, "DY: %f", y);
		pros::lcd::print(6, "H: %f", heading);

		pros::delay(50);
	}
}
