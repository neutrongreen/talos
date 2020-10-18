#include "main.h"
#include "talos/control.h"
#include "talos/config/talos2_config.h"
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
void initialize(){
	pros::lcd::initialize();
	pros::lcd::register_btn1_cb(on_center_button);
	init_motors(motors);
	//ImuPositing->calibrate();
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
	//move foward
	motors["fl"]->move_absolute(1, 100);
	motors["bl"]->move_absolute(1, 100);
	motors["fr"]->move_absolute(1, 100);
	motors["br"]->move_absolute(1, 100);
	pros::delay(2000);
	motors["belt_1"]->move_velocity(600);
	motors["belt_2"]->move_velocity(600);

}

/*
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
	while (true) {
		drive.update_speed();
		motors["belt_1"]->move_velocity(200*(master->get_digital(pros::E_CONTROLLER_DIGITAL_L1) - master->get_digital(pros::E_CONTROLLER_DIGITAL_L2)));
		motors["belt_2"]->move_velocity(200*(master->get_digital(pros::E_CONTROLLER_DIGITAL_R1) - master->get_digital(pros::E_CONTROLLER_DIGITAL_R2)));
		int intake = 200*((master->get_digital(pros::E_CONTROLLER_DIGITAL_A) - master->get_digital(pros::E_CONTROLLER_DIGITAL_B)));
		motors["intake_1"]->move_velocity(intake);
		motors["intake_2"]->move_velocity(intake);
		//ImuPositing->update_position();

		//pros::lcd::print(1, "X: %.2f", ImuPositing->velocity.x);
		//pros::lcd::print(2, "Y: %.2f", ImuPositing->velocity.y);
		//pros::lcd::print(3, "H: %.2f", ImuPositing->pose.h);
		pros::delay(50);
	}
}
