#include "main.h"
#include "sensors/sensors.h"

#define MAX_SPEED 200

//define motor objects
pros::Motor fl_motor(20, false);
pros::Motor fr_motor(11, true);
pros::Motor bl_motor(10, false);
pros::Motor br_motor(1, true);

pros::Motor inl_motor(5, true);
pros::Motor inr_motor(6, false);
pros::Motor belt_motor(7, false);
pros::Motor flywheel_motor(8, false);
//define mater CONTROLLER_MASTER
pros::Controller master(CONTROLLER_MASTER);

pros::Motor* motors[8] = {&fl_motor, &fr_motor, &bl_motor, &br_motor,  &inl_motor,  &inr_motor, &belt_motor, &flywheel_motor};

//define program states
enum  program_states{
  E_DRIVE = 0,
  E_CALIBRATE = 1
};

program_states state = E_DRIVE;
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */


 //define rotationvalue
 int autonrot = 5;
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
  motors[7]->set_gearing(pros::E_MOTOR_GEARSET_06);
  for(int i = 0; i < 8; i++){
    motors[i]->set_encoder_units(pros::E_MOTOR_ENCODER_ROTATIONS);
    motors[i]->tare_position();
    motors[i]->set_voltage_limit(10800);
    motors[i]->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
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
  flywheel_motor.move_velocity(190*3);
  fl_motor.move_relative(1, 100);
  fr_motor.move_relative(1, 100);
  bl_motor.move_relative(1, 100);
  br_motor.move_relative(1, 100);
  pros::delay(3000);
  fl_motor.move_relative(-0.2, 100);
  fr_motor.move_relative(-0.2, 100);
  bl_motor.move_relative(-0.2, 100);
  br_motor.move_relative(-0.2, 100);
  pros::delay(1000);
  belt_motor.move_velocity(190);

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
//program scripts
void calabration(){
  //move foward rotationvalue
  fl_motor.move_relative(autonrot, 100);
  fr_motor.move_relative(autonrot, 100);
  bl_motor.move_relative(autonrot, 100);
  br_motor.move_relative(autonrot, 100);
  //wait 5 seconds
  pros::delay(5000);
  //move sideways
  fl_motor.move_relative(autonrot, 100);
  bl_motor.move_relative(-autonrot, 100);
  fr_motor.move_relative(-autonrot, 100);
  br_motor.move_relative(autonrot, 100);
  //wait 5 seconds
  pros::delay(5000);
  //turn
  fl_motor.move_relative(autonrot/2, 100);
  bl_motor.move_relative(-autonrot/2, 100);
  fr_motor.move_relative(-autonrot/2, 100);
  br_motor.move_relative(autonrot/2, 100);
}
void drive(){
  bool beltstate = false;
  while(true){
    int x = (int)(((float)master.get_analog(ANALOG_LEFT_X)/128)*MAX_SPEED);
    int y = (int)(((float)master.get_analog(ANALOG_LEFT_Y)/128)*MAX_SPEED);
    int turn = (int)(((float)master.get_analog(ANALOG_RIGHT_X)/128)*MAX_SPEED);

    int intake = (master.get_digital(DIGITAL_L1)-master.get_digital(DIGITAL_L2))*MAX_SPEED;
    int flywheel = (master.get_digital(DIGITAL_R1)-master.get_digital(DIGITAL_R2))*MAX_SPEED;
    if(master.get_digital_new_press(DIGITAL_A)){
      beltstate = !beltstate;
    }

    fl_motor.move_velocity(y + turn - x);
    bl_motor.move_velocity(y + turn + x);
    fr_motor.move_velocity(y - turn + x);
    br_motor.move_velocity(y - turn - x);
    //set motor speeds
    inl_motor.move_velocity(intake);
    inr_motor.move_velocity(intake);
    /*
    if(beltstate){
        belt_motor.move_velocity(MAX_SPEED);
    }
    else{
      belt_motor.move_velocity(0);
    }
    flywheel_motor.move_velocity(flywheel);
    */
    belt_motor.move_velocity(flywheel);




    //delay
    pros::delay(20);
  }
}

void opcontrol() {
  //choose main function based up on program state
  switch(state){
    case E_DRIVE:
      drive();
      break;
    case E_CALIBRATE:
      calabration();
      break;
  }
  drive();
}
