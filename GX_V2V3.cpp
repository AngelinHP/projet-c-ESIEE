
/////////////////////////////////////////////////////////////
//
//                     V2
//
/////////////////////////////////////////////////////////////


#include <iostream>
#include "GX V2.h"

// comparaison sur des flottants... traitement spécial

bool operator == (const V2& a, const V2& b)
{
	V2 t = a - b;
	float epsilon = 0.001f;
	return t.norm() < epsilon;
}

// redéfinition des opérateurs standards

V2 operator + (const V2& a, const V2& b) { return V2(a.x + b.x, a.y + b.y); }
V2 operator - (const V2& a, const V2& b) { return V2(a.x - b.x, a.y - b.y); }
V2 operator * (float      a, const V2& b) { return V2(a * b.x, a * b.y); }
V2 operator * (const V2& a, float      b) { return V2(a.x * b, a.y * b); }
V2 operator / (const V2& a, float      b) { return V2(a.x / b, a.y / b); }
V2 operator - (const V2& a) { return V2(-a.x, -a.y); }


// produit scalaire / vectoriel

float prodScal(const V2& a, const V2& b) { return a.x * b.x + a.y * b.y; }
float prodVect(const V2& a, const V2& b) { return a.x * b.y - a.y * b.x; }

// affichage 

std::ostream& operator << (std::ostream& os, V2& t)
{
	os << "(" << t.x << "," << t.y << ")";
	return os;
}

/////////////////////////////////////////////////////////////
//
//                     V3
//
/////////////////////////////////////////////////////////////

#include "GX V3.h"

// comparaison sur des flottants... traitement spécial

bool operator == (const V3& a, const V3& b)
{
	V3 t = a - b;
	float epsilon = 0.001f;
	return t.norm() < epsilon;
}

// redéfinition des opérateurs standards

V3 operator + (const V3& a, const V3& b) { return V3(a.x + b.x, a.y + b.y, a.z + b.z); }
V3 operator - (const V3& a, const V3& b) { return V3(a.x - b.x, a.y - b.y, a.z - b.z); }
V3 operator * (float      a, const V3& b) { return V3(a * b.x, a * b.y, a * b.z); }
V3 operator * (const V3& a, float      b) { return V3(a.x * b, a.y * b, a.z * b); }
V3 operator / (const V3& a, float      b) { return V3(a.x / b, a.y / b, a.z / b); }

// produit scalaire

float operator * (const V3& a, const V3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

V3    operator -  (const V3& a) { return V3(-a.x, -a.y, -a.z); }

V3 prodVect(const V3& a, const V3& b)
{
	V3 t;
	t.x = a.y * b.z - a.z * b.y;
	t.y = a.z * b.x - a.x * b.z;
	t.z = a.x * b.y - a.y * b.x;
	return t;
}