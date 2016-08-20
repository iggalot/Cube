/////////////////////////////////////////////////////////////////////////////
// Name:        geometry.h
/////////////////////////////////////////////////////////////////////////////



#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <iostream>

class Point {
public:
	Point(int x_pt=0, int y_pt=0, int z_pt=0) {
		x = x_pt;
		y = y_pt;
		z = z_pt;
	}

	int getX() const {return x;}
	int getY() const {return y;}
	int getZ() const {return z;}

	void setX(int x_pt) {x = x_pt; return;}
	void setY(int y_pt) {y = y_pt; return;}
	void setZ(int z_pt) {z = z_pt; return;}

	// overloaded operators
  	Point operator=(const Point &rhs) {
    // Check for self-assignment!
    if (this == &rhs)      // Same object?
      return *this;        // Yes, so skip assignment, and just return *this.

  	this->x += rhs.getX();
  	this->y += rhs.getY();
  	this->z += rhs.getZ();

    return *this;
  }

	Point operator+(const Point& b) {
		int x_temp = getX() + b.getX();
		int y_temp = getY() + b.getY();
		int z_temp = getZ() + b.getZ();
		return Point(x_temp, y_temp, z_temp);
	}

	Point operator-(const Point& b) {
		int x_temp = getX() - b.getX();
		int y_temp = getY() - b.getY();
		int z_temp = getZ() - b.getZ();
		return Point(x_temp, y_temp, z_temp);
	}

	std::string Print() {
		std::string str = " ";
		str += std::to_string(getX());
		str += " , ";
		str += std::to_string(getY());
		str += " , "; 
		str += std::to_string(getZ());

		return str;
	}



private:
	int x;
	int y;
	int z;

};

#endif // _GEOMETRY_H
