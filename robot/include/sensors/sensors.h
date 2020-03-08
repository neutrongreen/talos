#ifndef _SENSORS_OBJ
#include "main.h"
#include "datatypes/datatypes.h"
#include <map>
//define base class for all coord sensors
class PosSensor{
  public:
    PosSensor();
    Coord position;
    void update_position();
    void reset_position();
};

//odomitry for meccaium drive
class DriveMechOdo : public PosSensor{
  public:
    //define intilaziation function with mtor inputs and callerbraiton values alpha and beta, with vary depring on surface
    DriveMechOdo(pros::Motor* lf, pros::Motor* lb, pros::Motor* rf, pros::Motor* rb, double a, double b);
    void update_position();
    void reset_position();
    //dictony contiang previous rotation of each motors and an arrya of mtors by same refines. futre opimization could be to use a strut
  protected:
    std::map<const char, pros::Motor*> d_motors;
    std::map<const char,double> pre_rot;
    double alpha = 0;
    double beta = 0;
};

//odomnitry fo diffrental drives
class DriveDifOdo : public PosSensor{
  public:
    //construcr
    DriveDifOdo(pros::Motor* l_motors_n[], pros::Motor* r_motors_n[]);
    //define motors refrence array
    pros::Motor* l_motors[8];
    pros::Motor* r_motors[8];
    //define previous value arrays for mtors
    double l_motors_p[8];
    double r_motors_p[8];
};
//inu postion cacluation class

#endif /* end of include guard:  */
