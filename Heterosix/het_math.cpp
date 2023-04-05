#include"het_math.h"
#ifndef ARDUINO
using namespace std;
#endif

// ===================== vec4 ===================== 
// construct zero vector
vec4::vec4() :x(v[0]), y(v[1]), z(v[2]) {	
	v[0] = 0; v[1] = 0, v[2] = 0, v[3] = 1; 
	x = *(v + 0); y = *(v + 1); z = *(v + 2); 
}
// construct certain vector
vec4::vec4(double x0, double y0, double z0):x(v[0]),y(v[1]),z(v[2]) {  
	v[0] = x0; v[1] = y0; v[2] = z0; v[3] = 1;
}
// copy constructor
vec4::vec4(vec4&V) : x(v[0]), y(v[1]), z(v[2]) {	
	v[0] = V.x; v[1] = V.y; v[2] = V.z; v[3] = 1;
}
vec4 operator*(double n, vec4 V) {
	return vec4(V.x * n, V.y * n, V.z * n);
}
vec4 operator*(vec4 V, double n) {
	return n*V;
}
// Copy assignment operator
vec4& vec4::operator=(const vec4&V) { // hope it works
	v[0] = V.x; v[1] = V.y; v[2] = V.z; v[3] = 1;
	return *this;
}
// vector plus
vec4 vec4::operator+(vec4 V) {	
	return vec4(this->x + V.x, this->y + V.y, this->z + V.z);
}
// vector minus
vec4 vec4::operator-(vec4 V) {
	return vec4(this->x - V.x, this->y - V.y, this->z - V.z);
}
// negative vector
vec4 vec4::operator-() {
	return vec4(-this->x, -this->y, -this->z);
}
// cross product axb --> a.cross(b)
vec4 vec4::cross(vec4 b) {
	vec4 temp;
	vec4&a = *this;
	temp.x = a.y*b.z - a.z*b.y;
	temp.y = a.z*b.x - a.x*b.z;
	temp.z = a.x*b.y - a.y*b.x;
	return temp;
}
// extend along the vector 
vec4 vec4::extend(double l) {
	vec4 temp;
	double l0= this->length();
	temp.x = this->x + l*this->x / l0;
	temp.y = this->y + l*this->y / l0;
	temp.z = this->z + l*this->z / l0;;
	return temp;
}
// set z valur to 0
vec4 vec4::XYplane() {
	return vec4(this->x, this->y, 0);
}
// dot product
double vec4::dot(vec4 V) {
	return (this->x*V.x + this->y*V.y + this->z*V.z);
}
// angle between this and ref
double vec4::angle(vec4 V) {
	if (this->length() == 0 || V.length() == 0) { throw "vector zero length"; }
	else { return acos(this->dot(V) / (this->length()*V.length())); }
}
// the length, literally
double vec4::length() {
	return sqrt((this->x)*(this->x) + (this->y)*(this->y) + (this->z)*(this->z));
}
// return unit vector
vec4 vec4::unit() {
	double l = this->length();
	return vec4(this->x / l, this->y / l, this->z / l);
}


// ===================== TransMatrix ===================== 
// default constructor: Identity matrix
TransMatrix::TransMatrix() {
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			this->v[r][c] = (1 * (r == c));
}
// construct rotaton matrix or translate matrix
TransMatrix::TransMatrix(int type, double par0 = 0, double par1 = 0, double par2 = 0) {
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			this->v[r][c] = 0;
	switch (type) {
	case TRANS_MATRIX:*this = this->Tran(par0, par1, par2); break;
	case xROT_MATRIX: *this = this->xRot(par0); break;
	case yROT_MATRIX: *this = this->yRot(par0); break;
	case zROT_MATRIX: *this = this->zRot(par0); break;
	}
}
// construct transform matrix by linear base (unit vec)
TransMatrix::TransMatrix(vec4 xuint, vec4 yuint, vec4 zuint, vec4 transl) {
	for (int i = 0; i < 3; i++) {
		this->v[i][0] = xuint.v[i];
		this->v[i][1] = yuint.v[i];
		this->v[i][2] = zuint.v[i];
		this->v[i][3] = transl.v[i];
		this->v[3][i] = 0;
	}
	this->v[3][3] = 0;
}
// more intuitive
double* TransMatrix::operator[](int index) {
	return this->v[index]; 
}
// matrix multiple
TransMatrix TransMatrix::operator*(TransMatrix T) {
	TransMatrix result;	double component;
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) { component = 0;
			for (int i = 0; i < 4; i++) {
				component += this->v[r][i] * T.v[i][c];
			}
			result.v[r][c] = component;
		}
	}
	return result;
}
// 4x4 matrix times 4*1 matrix
vec4 TransMatrix::operator*(vec4 V) {
	vec4 result; double component;
	for (int r = 0; r < 4; r++) { component = 0;
		for (int i = 0; i < 4; i++) {
			component += this->v[r][i] * V.v[i];
		}
		result.v[r] = component;
	}
	return result;
}

// return inverse matrix
TransMatrix TransMatrix::inv() {
/*
NOTE: no  deforming transformation. e.g. stretching, sheering, perspective...
i.e. for 
T =		| nx  ox  ax  px |
		| ny  oy  ay  py |
		| nz  oz  az  pz |
		|  0   0   0   1 |     <- they should always be [0,0,0,1]
and thus 
T^-1 =	| nx  ny  nz  -p.n |
		| ox  oy  oz  -p.o |
		| ax  ay  az  -p.a |
		|  0   0   0     1 | 

*/
	TransMatrix result;
	vec4 n(this->xUnit());
	vec4 o(this->yUnit());
	vec4 a(this->zUnit());
	vec4 p(this->Origin());
	for (int i = 0; i < 3; i++) {
		result.v[0][i] = n.v[i];
		result.v[1][i] = o.v[i];
		result.v[2][i] = a.v[i];
	}
	result.v[0][3] = -p.dot(n);
	result.v[1][3] = -p.dot(o);
	result.v[2][3] = -p.dot(a);

	return result;
}
// turn this matrix to Identity matrix
 void TransMatrix::eye() {
	 for (int r = 0; r < 4; r++)
		 for (int c = 0; c < 4; c++)
			 this->v[r][c] = (1 * (r == c));
}
 // return the result of this matrix translates in its own frame. *matrix itself don't change.
TransMatrix TransMatrix::Tran(double x, double y, double z) {
	TransMatrix temp;
	/*temp[0][0] = 1;		temp[0][1] = 0;		temp[0][2] = 0;		*/temp[0][3] = x;
	/*temp[1][0] = 0;		temp[1][1] = 1;		temp[1][2] = 0;		*/temp[1][3] = y;
	/*temp[2][0] = 0;		temp[2][1] = 0;		temp[2][2] = 1;		*/temp[2][3] = z;
	/*temp[3][0] = 0;		temp[3][1] = 0;		temp[3][2] = 0;		*/temp[3][3] = 1;
	temp = *this * temp;
	return temp;
}
// this matrix rotate by its own x axis. *matrix itself don't change.
TransMatrix TransMatrix::xRot(double t) {
	TransMatrix temp;
	double c = cos(t); double s = sin(t);
//	temp[0][0] = 1;		temp[0][1] = 0;	temp[0][2] = 0;		temp[0][3] = 0;
/*	temp[1][0] = 0;*/	temp[1][1] = c;	temp[1][2] = -s;  /*temp[1][3] = 0;
	temp[2][0] = 0;*/	temp[2][1] = s;	temp[2][2] = c;	  /*temp[2][3] = 0;
	temp[3][0] = 0;		temp[3][1] = 0;	temp[3][2] = 0;		temp[3][3] = 1;*/
	temp = *this * temp;
	return temp;
}

// this matrix rotate by its own y axis. *matrix itself don't change.
TransMatrix TransMatrix::yRot(double t) {
	TransMatrix temp;
	double c = cos(t); double s = sin(t);
	temp[0][0] = c;	 /* temp[0][1] = 0;	*/	temp[0][2] = s;	//	temp[0][3] = 0;
//	temp[1][0] = 0;		temp[1][1] = 1;		temp[1][2] = 0;		temp[1][3] = 0;
	temp[2][0] = -s;/*	temp[2][1] = 0;	*/	temp[2][2] = c;	//	temp[2][3] = 0;
//	temp[3][0] = 0;		temp[3][1] = 0;		temp[3][2] = 0;		temp[3][3] = 1;
	temp = *this * temp;
	return temp;
}

// this matrix rotate by its own z axis. *matrix itself don't change.
TransMatrix TransMatrix::zRot(double t) {
	TransMatrix temp;
	double c = cos(t); double s = sin(t);
	temp[0][0] = c;		temp[0][1] = -s;//	temp[0][2] = 0;	temp[0][3] = 0;
	temp[1][0] = s;		temp[1][1] = c;	/*	temp[1][2] = 0;	temp[1][3] = 0;
	temp[2][0] = 0;		temp[2][1] = 0;		temp[2][2] = 1;	temp[2][3] = 0;
	temp[3][0] = 0;		temp[3][1] = 0;		temp[3][2] = 0;	temp[3][3] = 1;*/
	temp = *this * temp;
	return temp;
}
// return the x axis unit vector of this matrix wrt world coordination
vec4 TransMatrix::xUnit() {
	return vec4(this->v[0][0], this->v[1][0], this->v[2][0]).unit();
}
// return the y axis unit vector of this matrix wrt world coordination
vec4 TransMatrix::yUnit() {
	return vec4(this->v[0][1], this->v[1][1], this->v[2][1]).unit();
}
// return the z axis unit vector of this matrix wrt world coordination
vec4 TransMatrix::zUnit() {
	return vec4(this->v[0][2], this->v[1][2], this->v[2][2]).unit();
}
// return the origin position of this frame wrt world coordination
vec4 TransMatrix::Origin() {
	return vec4(this->v[0][3], this->v[1][3], this->v[2][3]);
}
  #ifndef ARDUINO
  //  ===================== testing only  ===================== 

  // output matrix
  ostream& operator<<(ostream&os, TransMatrix&T) {
    for (int r = 0; r < 4; r++) {
      for (int c = 0; c < 4; c++) {
        os << setw(10) << T.v[r][c] << setw(10);
      }
      os << endl;
    }
    return os;
  }
  // out put vec4
  ostream& operator<<(ostream&os, vec4&T) {
    os << setw(10) << T.v[0] <<setw(10) << T.v[1] << setw(10) << T.v[2];
    return os;
  }
#endif

