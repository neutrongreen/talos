#include "talos/datatypes.h"

//basic constructor
Coord::Coord(double nx, double ny, double nh){
  position.x = nx;
  position.y = ny;
  h = nh;
}

//logic operator overloads

bool Coord::operator==(const Coord coord){
  //check if both coordtors x and y are the same adn heading h.
  return (position == coord.position && h == coord.h);
}

bool Coord::operator!=(const Coord coord){
  //check if both coordtors x, y and heading h are the same and then get the inverse
  return !(position == coord.position && h == coord.h);
}
//no need to have => <= or < and > as coordtors ar repsesnted by mtuable values rather than one sligre greater value
//meanign that we cannot chose the vlaue to check where one is greather than the other


//vctor functions
//will add more as needed

//normale coordtor by finding magnatued and divitng x and y bi it, resting the toal magnatued to 1.
