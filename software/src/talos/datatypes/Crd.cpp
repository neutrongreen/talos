#include "talos/datatypes.h"

//basic constructor
Crd::Crd(double nx, double ny, double nh){
  position.x = nx;
  position.y = ny;
  h = nh;
}

//logic operator overloads

bool Crd::operator==(const Crd crd){
  //check if both Crdtors x and y are the same adn heading h.
  return (position == crd.position && h == crd.h);
}

bool Crd::operator!=(const Crd crd){
  //check if both Crdtors x, y and heading h are the same and then get the inverse
  return !(position ==crd.position && h == crd.h);
}
//no need to have => <= or < and > as Crdtors ar repsesnted by mtuable values rather than one sligre greater value
//meanign that we cannot chose the vlaue to check where one is greather than the other


//vctor functions
//will add more as needed

//normale Crdtor by finding magnatued and divitng x and y bi it, resting the toal magnatued to 1.
