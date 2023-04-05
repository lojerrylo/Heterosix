#pragma once
#include"het_hexa_cfg.h"
#ifndef ARDUINO
  #include<cmath>
  #include<iostream>
  #include<iomanip>
  using namespace std;
#else
  #include<Arduino.h>
#endif

#ifndef PI
  #define PI 3.1415926535897932
#endif
#define dtr(x) (x / 180.0 * PI)
#define rtd(x) (x * 180.0 / PI)
#define NORMALIZE true


class TransMatrix;
class vec4 {
public:
// member
	double v[4]; // somehow waste space
	double&x; double&y; double&z;
// constructor
	vec4();
	vec4(double x, double y, double z);
	vec4(vec4&V);
	vec4& operator=(const vec4&V);
// function
	vec4 unit();
	
	vec4 operator+(vec4 V);
	vec4 operator-(vec4 V); // minus
	vec4 operator-();		// negative
	vec4 cross(vec4 V);
	vec4 extend(double l);
	vec4 XYplane();
	double dot(vec4 V);
	double angle(vec4 V);
	double length();
	friend vec4 operator*(double n, vec4 V);
	friend vec4 operator*(vec4 V, double n);
#ifndef ARDUINO
	friend ostream& operator<<(ostream&, vec4&T);
#endif
};
#define TRANS_MATRIX 1
#define xROT_MATRIX	2
#define yROT_MATRIX	3
#define zROT_MATRIX	4
class TransMatrix {
public:
	double v[4][4];
//constructor
	TransMatrix(int, double, double, double);
	TransMatrix(vec4 xUnint, vec4 yUnit, vec4 zUnit, vec4 transl);
	TransMatrix();
//transform
	void eye();
	TransMatrix Tran(double x, double y, double z);
	TransMatrix xRot(double theta);
	TransMatrix yRot(double theta);
	TransMatrix zRot(double theta);
//function
	TransMatrix operator*(TransMatrix T);
	TransMatrix inv();
	vec4 operator*(vec4 V);
	vec4 xUnit();
	vec4 yUnit();
	vec4 zUnit();
	vec4 Origin();
	
	double* operator[](int index);
#ifndef ARDUINO
	friend ostream& operator<<(ostream&, TransMatrix&T);
#endif
};
