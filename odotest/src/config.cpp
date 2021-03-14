#include "config.h"
#ifdef _OV_CONFIG_
//2918e
pros::Motor fl = pros::Motor(10, true);
pros::Motor bl = pros::Motor(1, true);
pros::Motor fr = pros::Motor(20);
pros::Motor br = pros::Motor(11);

pros::Motor roll1 = pros::Motor(15);
pros::Motor roll2 = pros::Motor(16);

pros::Motor in1 = pros::Motor(5);
pros::Motor in2 =  pros::Motor(6, true);

pros::Imu gyro = pros::Imu(2);
pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);

const double kP = 500;
const double kI = 1;
const double kD = 0.0001;
const double deadband = 0.01;
const double intdeadband = 0.01;
const double integral_max =20;

const double rot_kP = 5;
const double rot_kI = 4;
const double rot_kD = 3;
const double rot_deadband = degreesToRadians(0.2);
const double rot_intdeadband = degreesToRadians(0.2);
const double rot_integral_max = 100;

const int drive_type = 1;
const double WHEEL_C = 0.23340465;
//2018g
#else
pros::Motor fl = pros::Motor(10);
pros::Motor bl = pros::Motor(1);
pros::Motor fr = pros::Motor(20, true);
pros::Motor br = pros::Motor(12, true);

pros::Motor roll1 = pros::Motor(2);
pros::Motor roll2 = pros::Motor(9);

pros::Motor in1 = pros::Motor(7);
pros::Motor in2 =  pros::Motor(4);

pros::Imu gyro = pros::Imu(6);
pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);

const double kP = 600;
const double kI = 1;
const double kD = 0.0001;
const double deadband = 0.01;
const double intdeadband = 0.01;
const double integral_max =20;

const double rot_kP = 5;
const double rot_kI = 4;
const double rot_kD = 3;
const double rot_deadband = degreesToRadians(0.2);
const double rot_intdeadband = degreesToRadians(0.2);
const double rot_integral_max = 100;

const int drive_type = 0;

const double WHEEL_C = 0.3192;
#endif
