#include "GX.h"
#include "GX Objects3D.h"
#include "GX_GLWrapper.h"

void Object3D::forceSize(float size)
{
    V3 min, max;
    ComputeBoundingBox(min, max);
    V3 Dim = max - min; // dimensions de la BB

    float Lmax = std::max(std::max(Dim.x, Dim.y), Dim.z);
    float ratio = size / Lmax;
    for (Triangle & T : Triangles)
    {
        T.A = T.A * ratio;
        T.B = T.B * ratio;
        T.C = T.C * ratio;
    }
    ComputeBoundingBox(min, max);
}

void Object3D::moveVertices(V3 Tr)
{
    for (Triangle& T : Triangles)
    {
        T.A = T.A + Tr;
        T.B = T.B + Tr;
        T.C = T.C + Tr;
    }
}
void Object3D::setPivot(int xmode, int ymode, int zmode)
{
    V3 min, max;
    ComputeBoundingBox(min, max);

    V3 piv;
    if (xmode == 1) piv.x = min.x;
    if (xmode == 2) piv.x = (min.x + max.x)/2;
    if (xmode == 3) piv.x = max.x;

    if (ymode == 1) piv.y = min.y;
    if (ymode == 2) piv.y = (min.y + max.y) / 2;
    if (ymode == 3) piv.y = max.y;

    if (zmode == 1) piv.z = min.z;
    if (zmode == 2) piv.z = (min.z + max.z) / 2;
    if (zmode == 3) piv.z = max.z;

    moveVertices(-piv);

    ComputeBoundingBox(min, max);
}

void Object3D::ComputeBoundingBox(V3 & minxyz, V3 & maxxyz)
{
    V3& min = minxyz;
    V3& max = maxxyz;

    min.x = min.y = min.z =  std::numeric_limits<float>::max();
    max   = -min;

    for (const Triangle& t : Triangles)
    {
        const V3 points[3] = { t.A, t.B, t.C };

        for (const V3& p : points)
        {
            min.x = std::min(min.x, p.x);
            min.y = std::min(min.y, p.y);
            min.z = std::min(min.z, p.z);

            max.x = std::max(max.x, p.x);
            max.y = std::max(max.y, p.y);
            max.z = std::max(max.z, p.z);
        }
    }
}

void Object3D::Draw(bool flat, bool mesh, bool Normals)
{
	GLWrapper::DrawTriangles(Triangles, flat, mesh, Normals );
}


Object3D Object3D::CreateSphere(float R, float stepDeg, Color c)
{
	Object3D O;

	for (float t = 0; t < 360.0f; t += stepDeg)
	{
		for (float p = -90.0f; p < 90.0f; p += stepDeg)
		{
			float t1 = t * PI / 180.0f;
			float t2 = (t + stepDeg) * PI / 180.0f;

			float p1 = p * PI / 180.0f;
			float p2 = (p + stepDeg) * PI / 180.0f;

			// points
			float xA = R * cos(p1) * cos(t1);
			float yA = R * sin(p1);
			float zA = -R * cos(p1) * sin(t1);

			float xB = R * cos(p2) * cos(t1);
			float yB = R * sin(p2);
			float zB = -R * cos(p2) * sin(t1);

			float xC = R * cos(p2) * cos(t2);
			float yC = R * sin(p2);
			float zC = -R * cos(p2) * sin(t2);

			float xD = R * cos(p1) * cos(t2);
			float yD = R * sin(p1);
			float zD = -R * cos(p1) * sin(t2);

 
			V3 N(xA / R, yA / R, zA / R);
			V3 A(xA, yA, zA);
			V3 B(xB, yB, zB);
			V3 C(xC, yC, zC);
			V3 D(xD, yD, zD);

			O.Triangles.push_back(Triangle(A, B, C, N, c));
			O.Triangles.push_back(Triangle(A, C, D, N, c));

		}
	}
	return O;
}

Object3D Object3D::CreateCube(float size, Color  c)
{
    Object3D O;

    float s = size * 0.5f;

    // Sommets
    V3 p000(-s, -s, -s);
    V3 p001(-s, -s, s);
    V3 p010(-s, s, -s);
    V3 p011(-s, s, s);
    V3 p100(s, -s, -s);
    V3 p101(s, -s, s);
    V3 p110(s, s, -s);
    V3 p111(s, s, s);

    // ===== FACE +X =====
    V3 N1(1, 0, 0);
    O.Triangles.push_back(Triangle(p100, p110, p111, N1, c));
    O.Triangles.push_back(Triangle(p100, p111, p101, N1, c));

    // ===== FACE -X =====
    V3 N2(-1, 0, 0);
    O.Triangles.push_back(Triangle(p000, p001, p011, N2, c));
    O.Triangles.push_back(Triangle(p000, p011, p010, N2, c));

    // ===== FACE +Y =====
    V3 N3(0, 1, 0);
    O.Triangles.push_back(Triangle(p010, p011, p111, N3, c));
    O.Triangles.push_back(Triangle(p010, p111, p110, N3, c));

    // ===== FACE -Y =====
    V3 N4(0, -1, 0);
    O.Triangles.push_back(Triangle(p000, p100, p101, N4, c));
    O.Triangles.push_back(Triangle(p000, p101, p001, N4, c));

    // ===== FACE +Z =====
    V3 N5(0, 0, 1);
    O.Triangles.push_back(Triangle(p001, p101, p111, N5, c));
    O.Triangles.push_back(Triangle(p001, p111, p011, N5, c));

    // ===== FACE -Z =====
    V3 N6(0, 0, -1);
    O.Triangles.push_back(Triangle(p000, p010, p110, N6, c));
    O.Triangles.push_back(Triangle(p000, p110, p100, N6, c));

    return O;
}

Object3D Object3D::CreateParallelogram(V3 A, V3 B, V3 C, Color c,bool flipNormal)
{
    Object3D O;


    // Normale (produit vectoriel)
    V3 AB = B - A;
    V3 AC = C - A;
    // Calcul du 4e point
    V3 D = A + AB + AC;
    V3 N = prodVect(AB,AC);
    N.normalize();
    if (flipNormal) N = -N;

    // Deux triangles
    O.Triangles.push_back(Triangle(A, B, C, N, c));
    O.Triangles.push_back(Triangle(B, C, D, N, c));

    return O;
}
 
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

Object3D Object3D::LoadFromPLYInAssetFolder(const  std::string& filename)
{
    Object3D O;
 
 
    std::cout << "Trying: " << filename << std::endl;
    std::cout << std::filesystem::current_path() << std::endl;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Erreur : impossible d'ouvrir le fichier " << filename << std::endl;
        return O;
    }

    std::string line;
    std::string token;

    int vertexCount = 0;
    int faceCount = 0;
    bool inHeader = true;

    // -------- Lecture du header --------
    while (inHeader && std::getline(file, line))
    {
        std::istringstream iss(line);
        iss >> token;

        if (token == "element")
        {
            std::string elementType;
            iss >> elementType;

            if (elementType == "vertex")
                iss >> vertexCount;
            else if (elementType == "face")
                iss >> faceCount;
        }
        else if (token == "end_header")
        {
            inHeader = false;
        }
    }

    if (vertexCount <= 0 || faceCount <= 0)
    {
        std::cerr << "Erreur : header PLY invalide ou incomplet." << std::endl;
        return O;
    }

    // -------- Lecture des sommets --------
    std::vector<V3> vertices;
    vertices.reserve(vertexCount);

    for (int i = 0; i < vertexCount; ++i)
    {
        if (!std::getline(file, line))
        {
            std::cerr << "Erreur : fin de fichier inattendue pendant la lecture des sommets." << std::endl;
            return O;
        }

        std::istringstream iss(line);
        float x, y, z;
        iss >> x >> y >> z;

        vertices.push_back(V3(x, y, z));
    }

    // -------- Lecture des faces --------
    for (int i = 0; i < faceCount; ++i)
    {
        if (!std::getline(file, line))
        {
            std::cerr << "Erreur : fin de fichier inattendue pendant la lecture des faces." << std::endl;
            return O;
        }

        std::istringstream iss(line);

        int n;
        iss >> n;

        // Ici on ignore les faces non triangulaires.
        if (n != 3)
            continue;

        int iA, iB, iC;
        iss >> iA >> iB >> iC;

        int r = 255, g = 255, b = 255;
        iss >> r >> g >> b;

        if (iA < 0 || iA >= (int)vertices.size() ||
            iB < 0 || iB >= (int)vertices.size() ||
            iC < 0 || iC >= (int)vertices.size())
        {
            std::cerr << "Erreur : indice de sommet invalide dans une face." << std::endl;
            continue;
        }

        V3 A = vertices[iA];
        V3 B = vertices[iB];
        V3 C = vertices[iC];

        // Normale de face
        V3 AB = B - A;
        V3 AC = C - A;
        V3 N = prodVect(AB,AC);

        float len = N.norm();

        Color faceColor(r / 255.0f, g / 255.0f, b / 255.0f);


        if (len > 1e-6f)
        {
            N = N / len;
            O.Triangles.push_back(Triangle(A, B, C, N, faceColor));
        }

 
 
    }

    return O;
}