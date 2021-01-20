#ifndef _TALOS_CONFIG_
#define _TALOS_CONFIG_
#include "api.h"
//Overide Config
//default config
#define ROT_CONST sin(degreesToRadians(45))
#define WHEEL_C 0.3192
#define MAX_RPM 200
#define MAX_AUTON_RPM 200

#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)
#define radiansToDegrees(angleRad) (angleRad / (M_PI / 180.0))

extern pros::Motor fl;
extern pros::Motor bl;
extern pros::Motor fr;
extern pros::Motor br;
extern pros::Motor in;
extern pros::Imu gyro;
extern pros::Controller master;

extern const double kP;
extern const double kI;
extern const double kD;
extern const double deadband;
extern const double intdeadband;
extern const double integral_max;

extern const double rot_kP;
extern const double rot_kI;
extern const double rot_kD;
extern const double rot_deadband;
extern const double rot_intdeadband;
extern const double rot_integral_max;

#endif
