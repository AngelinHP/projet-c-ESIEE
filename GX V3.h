#pragma once
#include <cmath>
 
struct uv // uv coordinates of textured surfaces
{
	float u, v;
	uv(float _u, float _v) { u = _u; v = _v; }
	uv() {}
};
 

struct V3
{
	float x, y, z;
	V3(float _x, float _y, float _z)	{ x = _x; y = _y; z = _z; }
	V3() {} 
	float norm()		const  { return sqrt(x * x + y * y + z * z); }
	void normalize()		   { float n = norm();     x /= n;    y /= n;  z /= n; }
    V3 GetNormalized()  const  { float n = norm(); return V3(x / n, y / n, z / n); }
};

// redéfinition des opérateurs standards

bool  operator == (const V3 & a, const V3 & b);
V3	  operator +  (const V3 & a, const V3 & b);
V3    operator -  (const V3 & a, const V3 & b);
V3    operator *  (float      a, const V3 & b);
V3    operator *  (const V3 & a, float      b);
V3    operator /  (const V3 & a, float      b);
float operator *  (const V3 & a, const V3 & b);   // produit scalaire
V3    operator -  (const V3 & a);
V3 prodVect(const V3 & a, const V3 & b);


