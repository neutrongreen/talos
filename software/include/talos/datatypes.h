#ifndef _DATA_TYPES
//include cmath
#include <cmath>
#define _DATA_TYPES

/* end of include guard:  */


class Vector2D{
  public:
    //constructor
    Vector2D(double nx = 0, double ny = 0);
    double x;
    double y;
    //overding vector math operators
    // * and / are scalars becuase * and / operators dont work with vector to vecotr math
    Vector2D operator+(const Vector2D vec);
    Vector2D operator-(const Vector2D vec);
    Vector2D operator*(const double scalar);
    Vector2D operator/(const double scalar);
    //overloading set math operators
    // * and / are scalars becuase * and / operators dont work with vector to vecotr math
    void operator+=(const Vector2D vec);
    void operator-=(const Vector2D vec);
    void operator*=(const double scalar);
    void operator/=(const double scalar);
    //overloading vector logic operators
    bool operator==(const Vector2D vec);
    bool operator!=(const Vector2D vec);
    //define normalize vecotor function
    //will add more functions as needed
    Vector2D normalized();
    //add rotation function that will rotoa vector by angle
    Vector2D rotate(double angle);
};

//class for location + heaidng
class Coord{
 public:
    //constructor
    Coord(double nx = 0, double ny = 0, double nh = 0);
    //changed x and y to vetor for utility
    Vector2D position;
    double h;
    //overding Polar  math operators
    // * and / are scalars becuase * and / operators dont work with vector to vecotr math

    //overloading Polar logic operators
    bool operator==(const Coord coord);
    bool operator!=(const Coord coord);

};

#endif
