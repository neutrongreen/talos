#ifndef ODOMENTRYH

//include main
#include "main.h"
#include "talos/datatypes.h"
#include <map>
#include <vector>
#include <math.h>
#include <chrono>
#define G 9.82
//define odomentry classes
class OdomentryBaseClass{
public:
  //define cooords for predicted positions
  Coord pose = Coord(0, 0 ,0);
    //define update drive function returns pose
  Coord update_position();
protected:
    //last update time
  unsigned int last_update_t;
    //map of linked motors
    //last rotation of motors the map will be equal to the keys of the upper ma
};

//define base class for holomic drive for odementry
class HolonomicDriveOdo : public OdomentryBaseClass{
  public:
      //define public constructor
    HolonomicDriveOdo(std::map<std::string, pros::Motor*>* motors, std::vector<std::string> keys = {"fl", "fr", "bl", "br"});
  protected:
    //define config
    std::map<std::string, pros::Motor*> robot_mtrs;
    //define array of drive lablels used in equasion coner names are from lef tot right top to botttom to allow for interchnaabiltiy whythout havingt o modify everything
    std::vector<std::string> drive_keys = {"fl", "fr", "bl", "br"};
    //define last rot values
    std::map<std::string, double> last_rot;
    //get last rotation
    double get_rot_change(std::string motor);
};

//fuction overide
class MeccanumDriveOdo : public HolonomicDriveOdo{
  public:
    //define fuction overide
    MeccanumDriveOdo(std::map<std::string, pros::Motor*>* motors, int wheel_separation_width, int wheel_separation_height, double wheel_radius, std::vector<std::string> keys = {"fl", "fr", "bl", "br"});
    Coord update_position();
  protected:
    //define wheel cosntants
    int separation_width = 0;
    int separation_height = 0;
    double radius = 0;

};

class InvOdomentry : public OdomentryBaseClass{
  public:
    InvOdomentry(pros::Imu* interal_unit);
    Coord update_position();
    void reset_sensor();
  protected:
    pros::Imu* navunit;
    std::chrono::system_clock::time_point lasttime = std::chrono::system_clock::time_point();
    Vector2D velocity = Vector2D(0 ,0);
};
#endif