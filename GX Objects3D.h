#pragma once
#include "GX V3.h"
#include "GX Color.h"
#include <vector>

struct Triangle
{
	V3 A, B, C;
	V3 N;
	Color c;

	Triangle(V3& _A, V3& _B, V3& _C, V3& _N, Color& _c) : A(_A), B(_B), C(_C), N(_N), c(_c) {}
};

struct Object3D
{
	std::vector<Triangle> Triangles;
	void ComputeBoundingBox(V3& minxyz, V3& maxxyz);  // calcule la BB sous forme de min/max
	void forceSize(float size);                       // force la taille de l'objet à size
	void setPivot(int xmode, int ymode, int zmode);   // positionne le 0 de l'objet à 1: xmin, 2: (xmin+xmax)/2, 3: xmax
	void moveVertices(V3 T);					      // décale tous les vertex de T

	void Draw(bool flat = true, bool mesh = true, bool Normals = true);
	 
	static Object3D CreateSphere(float R = 100.0f, float stepDeg = 10.0f, Color c= Color::Red);
	static Object3D CreateCube(float size = 100.0f, Color c = Color::Red);
	static Object3D CreateParallelogram(V3 A, V3 B, V3 C, Color c, bool flipNormal); // AB-AC
	static Object3D LoadFromPLYInAssetFolder(const std::string& filename);
};