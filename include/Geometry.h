/////////////////////////////////////////////////////////////////////////////
// Name:        geometry.h
/////////////////////////////////////////////////////////////////////////////



#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <iostream>

class Point {
public:
	Point(float x_pt=0, float y_pt=0, float z_pt=0) {
		x = x_pt;
		y = y_pt;
		z = z_pt;
	}

	float getX() const {return x;}
	float getY() const {return y;}
	float getZ() const {return z;}

	void setX(float x_pt) {x = x_pt; return;}
	void setY(float y_pt) {y = y_pt; return;}
	void setZ(float z_pt) {z = z_pt; return;}

	void setPoint(Point point) {
		setX(point.x);
		setY(point.y);
		setZ(point.z);
		return;
	}

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
		float x_temp = getX() + b.getX();
		float y_temp = getY() + b.getY();
		float z_temp = getZ() + b.getZ();
		return Point(x_temp, y_temp, z_temp);
	}

	Point operator-(const Point& b) {
		float x_temp = getX() - b.getX();
		float y_temp = getY() - b.getY();
		float z_temp = getZ() - b.getZ();
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
	float x;
	float y;
	float z;

};

#endif // _GEOMETRY_H
