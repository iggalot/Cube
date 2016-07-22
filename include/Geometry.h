/////////////////////////////////////////////////////////////////////////////
// Name:        geometry.h
/////////////////////////////////////////////////////////////////////////////



#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

class Point {
public:
	Point() {Point(0,0,0);}
	Point(int x_pt, int y_pt, int z_pt) {
		x = x_pt;
		y = y_pt;
		z = z_pt;
	}

	int getX() {return x;}
	int getY() {return y;}
	int getZ() {return z;}

private:
	int x;
	int y;
	int z;

};

#endif // _GEOMETRY_H
