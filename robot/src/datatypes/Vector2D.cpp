#include "datatypes/datatypes.h"

//construction script
Vector2D::Vector2D(double nx, double ny){
  //nx and ny are newx and newy
  //set x and y to newx and newy. by default newx and newy are zero.
  x = nx;
  y = ny;
}


//opertor overloads

//basic math
// add one vector to another
Vector2D Vector2D::operator+(const Vector2D vec){
  return Vector2D(x + vec.x, y+vec.y);
}

//minus operator overload sburacts one vecotr from another
Vector2D Vector2D::operator-(const Vector2D vec){
  return Vector2D(x - vec.x, y - vec.y);
}
//mutply and divide can only be done by sclars, rather than another vector as vecotrs have to be mutiplyed by other functions

//mutply each indivraul value of vector to scale it up
Vector2D Vector2D::operator*(const double scalar){
  return Vector2D(x * scalar, y * scalar);
}

//divide each indvidual value of vector to scale it down
Vector2D Vector2D::operator/(const double scalar){
  return Vector2D(x / scalar, y / scalar);
}

//plusequals operator overload for vector.
void Vector2D::operator+=(const Vector2D vec){
  //using += for both x and y
  x += vec.x;
  y += vec.y;
}

//minusequals operaor overload for vector
void Vector2D::operator-=(const Vector2D vec){
  //using -= for both vector x and y
  x -= vec.x;
  y -= vec.y;
}

//mutplcial and divison /= and *= are done by scalars because of same issue listed above

//mutply each indivraul value of vector to scale it up. *= overload
void Vector2D::operator*=(const double scalar){
  //using *= as operator
  x *= scalar;
  y *= scalar;
}

//divide each indvidual value of vector to scale it down. /= overload
void Vector2D::operator/=(const double scalar){
  //using /= as fuaction
  x /= scalar;
  y /= scalar;
}

//logic operator overloads

bool Vector2D::operator==(const Vector2D vec){
  //check if both vectors x and y are the same
  return (x == vec.x && y == vec.y);
}

bool Vector2D::operator!=(const Vector2D vec){
  //check if both vectors x and y are the same and then get the inverse
  return !(x == vec.x && y == vec.y);
}
//no need to have => <= or < and > as vectors ar repsesnted by mtuable values rather than one sligre greater value
//meanign that we cannot chose the vlaue to check where one is greather than the other


//vctor functions
//will add more as needed

//normale vector by finding magnatued and divitng x and y bi it, resting the toal magnatued to 1.
Vector2D Vector2D::normalized(){
  double m = std::sqrt(x*x + y*y);
  return Vector2D(x/m, y/m);
}

//add rotation functions
Vector2D Vector2D::rotate(double angle){
  //set old xy values
  double oldx = x;
  double oldy = y;
  //cacluate rotated vector
  double nx = oldx*cos(angle)-oldy*sin(angle);
  double ny = oldx*sin(angle)+oldy*cos(angle);
  //return rotated vector
  return Vector2D(nx, ny);
}
