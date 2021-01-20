#include "config.h"
#ifdef _OV_CONFIG_

pros::Motor fl = pros::Motor(10);
pros::Motor bl = pros::Motor(1);
pros::Motor fr = pros::Motor(20, true);
pros::Motor br = pros::Motor(11, true);
pros::Motor in = pros::Motor(7, true);
pros::Imu gyro = pros::Imu(6);
pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);

const double kP = 500;
const double kI = 1;
const double kD = 0.0001;
const double deadband = 0.02;
const double intdeadband = 0.02;
const double integral_max =20;

const double rot_kP = 5;
const double rot_kI = 4;
const double rot_kD = 3;
const double rot_deadband = degreesToRadians(0.2);
const double rot_intdeadband = degreesToRadians(0.2);
const double rot_integral_max = 100;

#else
pros::Motor fl = pros::Motor(10);
pros::Motor bl = pros::Motor(1);
pros::Motor fr = pros::Motor(20, true);
pros::Motor br = pros::Motor(11, true);
pros::Motor in = pros::Motor(7, true);
pros::Imu gyro = pros::Imu(6);
pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);

const double kP = 500;
const double kI = 1;
const double kD = 0.0001;
const double deadband = 0.02;
const double intdeadband = 0.02;
const double integral_max =20;

const double rot_kP = 5;
const double rot_kI = 4;
const double rot_kD = 3;
const double rot_deadband = degreesToRadians(0.2);
const double rot_intdeadband = degreesToRadians(0.2);
const double rot_integral_max = 100;

#endif
