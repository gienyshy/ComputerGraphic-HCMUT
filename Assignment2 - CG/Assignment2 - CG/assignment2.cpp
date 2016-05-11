#include "stdafx.h"
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <iostream>
#include <gl.h>
#include <glut.h>
using namespace std;

#define _CRT_SECURE_NO_DEPRECATE
#define PI		3.1415926
#define DEG2RAD PI/180.0
#define	COLORNUM  15


////---------------------------------------Tga.h

typedef	struct
{
	GLubyte	* imageData;									// Image Data (Up To 32 Bits)
	GLuint	bpp;											// Image Color Depth In Bits Per Pixel
	GLuint	width;											// Image Width
	GLuint	height;											// Image Height
	GLuint	texID;											// Texture ID Used To Select A Texture
	GLuint	type;											// Image Type (GL_RGB, GL_RGBA)
} Texture;

typedef struct
{
	GLubyte Header[12];									// TGA File Header
} TGAHeader;

typedef struct
{
	GLubyte		header[6];								// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;									// Temporary Variable
	GLuint		type;
	GLuint		Height;									//Height of Image
	GLuint		Width;									//Width ofImage
	GLuint		Bpp;									// Bits Per Pixel
} TGA;


TGAHeader tgaheader;									// TGA header
TGA tga;										// TGA image data

GLubyte uTGAcompare[12] = { 0,0,2, 0,0,0,0,0,0,0,0,0 };	// Uncompressed TGA Header

Texture floorTex;

bool LoadTGA(Texture * texture, char * filename)				// Load a TGA file
{
	FILE * fTGA;												// File pointer to texture file
	fTGA = fopen(filename, "rb");								// Open file for reading

	if (fTGA == NULL)											// If it didn't open....
	{
		return false;														// Exit function
	}

	if (fread(&tgaheader, sizeof(TGAHeader), 1, fTGA) == 0)					// Attempt to read 12 byte header from file
	{
		if (fTGA != NULL)													// Check to seeiffile is still open
		{
			fclose(fTGA);													// If it is, close it
		}
		return false;														// Exit function
	}

	// an Uncompressed TGA image
	if (fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)					// Read TGA header
	{
		if (fTGA != NULL)													// if file is still open
		{
			fclose(fTGA);													// Close it
		}
		return false;														// Return failular
	}

	texture->width = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	texture->bpp = tga.header[4];										// Determine the bits per pixel
	tga.Width = texture->width;										// Copy width into local structure						
	tga.Height = texture->height;										// Copy height into local structure
	tga.Bpp = texture->bpp;											// Copy BPP into local structure

	if ((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp != 32)))	// Make sure all information is valid
	{
		if (fTGA != NULL)													// Check if file is still open
		{
			fclose(fTGA);													// If so, close it
		}
		return false;														// Return failed
	}

	if (texture->bpp == 24)													// If the BPP of the image is 24...
		texture->type = GL_RGB;											// Set Image type to GL_RGB
	else																	// Else if its 32 BPP
		texture->type = GL_RGBA;											// Set image type to GL_RGBA

	tga.bytesPerPixel = (tga.Bpp / 8);									// Compute the number of BYTES per pixel
	tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute the total amout ofmemory needed to store data
	texture->imageData = (GLubyte *)malloc(tga.imageSize);					// Allocate that much memory

	if (texture->imageData == NULL)											// If no space was allocated
	{
		fclose(fTGA);														// Close the file
		return false;														// Return failed
	}

	if (fread(texture->imageData, 1, tga.imageSize, fTGA) != tga.imageSize)	// Attempt to read image data
	{
		if (texture->imageData != NULL)										// If imagedata has data in it
		{
			free(texture->imageData);										// Delete data from memory
		}
		fclose(fTGA);														// Close file
		return false;														// Return failed
	}

	// switch R and B
	for (int i = 0; i < tga.imageSize; i += tga.bytesPerPixel)
	{
		GLubyte temp = texture->imageData[i];
		texture->imageData[i] = texture->imageData[i + 2];
		texture->imageData[i + 2] = temp;
	}


	fclose(fTGA);															// Close file
	return true;															// All went well, continue on
}

void loadTextures(void) {
	bool status = LoadTGA(&floorTex, "MARBLE.tga");
	if (status) {
		glGenTextures(1, &floorTex.texID);
		glBindTexture(GL_TEXTURE_2D, floorTex.texID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, floorTex.width,
			floorTex.height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, floorTex.imageData);

		if (floorTex.imageData)
			free(floorTex.imageData);
	}
}

//---------------------------------------supportClass.h
class Point3
{
public:
	float x, y, z;
	void set(float dx, float dy, float dz)
	{
		x = dx; y = dy; z = dz;
	}
	void set(Point3& p)
	{
		x = p.x; y = p.y; z = p.z;
	}
	Point3() { x = y = z = 0; }
	Point3(float dx, float dy, float dz)
	{
		x = dx; y = dy; z = dz;
	}

};
class Color3
{
public:
	float r, g, b;
	void set(float red, float green, float blue)
	{
		r = red; g = green; b = blue;
	}
	void set(Color3& c)
	{
		r = c.r; g = c.g; b = c.b;
	}
	Color3() { r = g = b = 0; }
	Color3(float red, float green, float blue)
	{
		r = red; g = green; b = blue;
	}

};
class Point2
{
public:
	Point2() { x = y = 0.0f; }
	Point2(float xx, float yy) { x = xx; y = yy; }
	void set(float xx, float yy) { x = xx; y = yy; }
	float getX() { return x; }
	float getY() { return y; }
	void draw() {
		glBegin(GL_POINTS);
		glVertex2f((GLfloat)x, (GLfloat)y);
		glEnd();
	}
private:
	float 	x, y;
};
class IntRect
{
public:
	IntRect() { l = 0; r = 100; b = 0; t = 100; } // constructor
	IntRect(int left, int right, int bottom, int top)
	{
		l = left; r = right; b = bottom; t = top;
	}
	void set(int left, int right, int bottom, int top)
	{
		l = left; r = right; b = bottom; t = top;
	}
	void draw() {
		glRecti(l, b, r, t);
		glFlush();
	} // draw this rectangle using OpenGL
	int  getWidth() { return (r - l); }
	int  getHeight() { return (t - b); }
private:
	int	l, r, b, t;
};
class RealRect
{
public:
	RealRect() { l = 0; r = 100; b = 0; t = 100; } // constructor
	RealRect(float left, float right, float bottom, float top)
	{
		l = left; r = right; b = bottom; t = top;
	}
	void set(float left, float right, float bottom, float top)
	{
		l = left; r = right; b = bottom; t = top;
	}
	float  getWidth() { return (r - l); }
	float  getHeight() { return (t - b); }
	void RealRect::draw() {
		glRectf(l, b, r, t);
		glFlush();
	};// draw this rectangle using OpenGL
private:
	float	l, r, b, t;
};

class Vector3
{
public:
	float	x, y, z;
	void set(float dx, float dy, float dz)
	{
		x = dx; y = dy; z = dz;
	}
	void set(Vector3& v)
	{
		x = v.x; y = v.y; z = v.z;
	}
	void flip()
	{
		x = -x; y = -y; z = -z;
	}
	void normalize();
	Vector3() { x = y = z = 0; }
	Vector3(float dx, float dy, float dz)
	{
		x = dx; y = dy; z = dz;
	}
	Vector3(Vector3& v)
	{
		x = v.x; y = v.y; z = v.z;
	}
	Vector3 cross(Vector3 b);
	float dot(Vector3 b);
};

//----------------------------------------------------------------------supportClass.cpp
Vector3 Vector3::cross(Vector3 b)
{
	Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
	return c;
}
float Vector3::dot(Vector3 b)
{
	return x*b.x + y*b.y + z*b.z;
}
void Vector3::normalize()
{
	float temp = sqrt(x*x + y*y + z*z);
	x = x / temp;
	y = y / temp;
	z = z / temp;
}

//--------------------------Mesh.h

class VertexID
{
public:
	int		vertIndex;
	int		colorIndex;
};

class Face
{
public:
	int		nVerts;
	VertexID*	vert;
	Vector3		facenorm; // normal vector of the face

	Face()
	{
		nVerts = 0;
		vert = NULL;
	}
	~Face()
	{
		if (vert != NULL)
		{
			delete[] vert;
			vert = NULL;
		}
		nVerts = 0;
	}
};


class Mesh
{
public:
	int		numVerts;
	Point3*		pt;

	int		numFaces;
	Face*		face;
	float		slideX, slideY, slideZ;
	float		rotateX, rotateY, rotateZ;
	float		scaleX, scaleY, scaleZ;

public:
	Mesh()
	{
		numVerts = 0;
		pt = NULL;
		numFaces = 0;
		face = NULL;
		slideX = 0, slideY = 1, slideZ = 0;
		rotateX = 0, rotateY = 0, rotateZ = 0;
		scaleX = 1, scaleY = 1, scaleZ = 1;
	}
	~Mesh()
	{
		if (pt != NULL)
		{
			delete[] pt;
		}
		if (face != NULL)
		{
			delete[] face;
		}
		numVerts = 0;
		numFaces = 0;
	}
	void DrawWireframe();
	void DrawColor();
	void DrawDisColor();
	void Draw();
	void DrawFloor();

	void CalculateFacesNorm();
	void CreateYPlane(int nXSegment, int nZSegment, float fXWidth, float fZWidth);

	void CreateTetrahedron();
	void CreateCube(float	fSize);
	void CreateCuboid(float	fSizeX, float fSizeY, float	fSizeZ);
	void CreateCylinder(int nSegment, float fHeight, float fRadius);
	void CreateSphere(int nSlice, int nStack, float radius);
	void CreateLavie();
	void SetColor(int colorIdx);

	int GetColour(int& colour);

	void CreateGeometryShape(float R1, float R2, float Height, float distance);
	void TangentLine_Cut_Circle(float C1[3], float C2[3], float x, float y, float *D1, float *D2, float* p);
	void TangentLine(float C1[3], float C2[3], float xA, float yA, float *D1, float *D2);
	void TangentLine_Get_Through(float C1[3], float C2[3], float& x, float& y);
	void SolveQuadFormula(float a, float b, float c, float& val1, float& val2);

	void CreateFloor(float fSize);
	void CreateFloor_test();

};

//-----------------------------------------Mesh.cpp
float	ColorArr[COLORNUM][3] = { { 1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 0.0,  0.0, 1.0 },
{ 1.0, 1.0,  0.0 },{ 1.0, 0.0, 1.0 },{ 0.0, 1.0, 1.0 },
{ 0.3, 0.3, 0.3 },{ 0.5, 0.5, 0.5 },{ 0.9,  0.9, 0.9 },
{ 1.0, 0.5,  0.5 },{ 0.5, 1.0, 0.5 },{ 0.5, 0.5, 1.0 },
{ 0.0, 0.0, 0.0 },{ 1.0, 1.0, 1.0 } };

void Mesh::CalculateFacesNorm()
{
	for (int f = 0; f< numFaces; f++) {
		for (int v = 0; v < face[f].nVerts; v++)
		{
			face[f].facenorm.x += (pt[face[f].vert[v].vertIndex].y - pt[face[f].vert[(v + 1) % face[f].nVerts].vertIndex].y)
				* (pt[face[f].vert[v].vertIndex].z + pt[face[f].vert[(v + 1) % face[f].nVerts].vertIndex].z);

			face[f].facenorm.y += (pt[face[f].vert[v].vertIndex].z - pt[face[f].vert[(v + 1) % face[f].nVerts].vertIndex].z)
				* (pt[face[f].vert[v].vertIndex].x + pt[face[f].vert[(v + 1) % face[f].nVerts].vertIndex].x);

			face[f].facenorm.z += (pt[face[f].vert[v].vertIndex].x - pt[face[f].vert[(v + 1) % face[f].nVerts].vertIndex].x)
				* (pt[face[f].vert[v].vertIndex].y + pt[face[f].vert[(v + 1) % face[f].nVerts].vertIndex].y);

		}
		face[f].facenorm.normalize();
	}
}

void Mesh::SetColor(int colorIdx) {
	for (int f = 0; f < numFaces; f++)
	{
		for (int v = 0; v < face[f].nVerts; v++)
		{
			face[f].vert[v].colorIndex = colorIdx;
		}
	}
}

void Mesh::CreateCube(float	fSize)
{
	int i;

	numVerts = 8;
	pt = new Point3[numVerts];
	pt[0].set(-fSize, fSize, fSize);
	pt[1].set(fSize, fSize, fSize);
	pt[2].set(fSize, fSize, -fSize);
	pt[3].set(-fSize, fSize, -fSize);
	pt[4].set(-fSize, -fSize, fSize);
	pt[5].set(fSize, -fSize, fSize);
	pt[6].set(fSize, -fSize, -fSize);
	pt[7].set(-fSize, -fSize, -fSize);

	numFaces = 6;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for (i = 0; i<face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for (i = 0; i<face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;
	for (i = 0; i<face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;
	for (i = 0; i<face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for (i = 0; i<face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for (i = 0; i<face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;

	CalculateFacesNorm();
}

void Mesh::CreateTetrahedron()
{
	int i;
	numVerts = 4;
	pt = new Point3[numVerts];
	pt[0].set(0, 0, 0);
	pt[1].set(1, 0, 0);
	pt[2].set(0, 1, 0);
	pt[3].set(0, 0, 1);

	numFaces = 4;
	face = new Face[numFaces];

	face[0].nVerts = 3;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 2;
	face[0].vert[2].vertIndex = 3;
	for (i = 0; i<face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;


	face[1].nVerts = 3;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 2;
	face[1].vert[2].vertIndex = 1;
	for (i = 0; i<face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;


	face[2].nVerts = 3;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 3;
	face[2].vert[2].vertIndex = 2;
	for (i = 0; i<face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;


	face[3].nVerts = 3;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 1;
	face[3].vert[1].vertIndex = 3;
	face[3].vert[2].vertIndex = 0;
	for (i = 0; i<face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	CalculateFacesNorm();
}

void Mesh::CreateCuboid(float	fSizeX, float fSizeY, float	fSizeZ) {
	int i;

	numVerts = 8;
	pt = new Point3[numVerts];
	pt[0].set(-fSizeX, fSizeY, fSizeZ);
	pt[1].set(fSizeX, fSizeY, fSizeZ);
	pt[2].set(fSizeX, fSizeY, -fSizeZ);
	pt[3].set(-fSizeX, fSizeY, -fSizeZ);
	pt[4].set(-fSizeX, -fSizeY, fSizeZ);
	pt[5].set(fSizeX, -fSizeY, fSizeZ);
	pt[6].set(fSizeX, -fSizeY, -fSizeZ);
	pt[7].set(-fSizeX, -fSizeY, -fSizeZ);

	numFaces = 6;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for (i = 0; i<face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for (i = 0; i<face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;
	for (i = 0; i<face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;
	for (i = 0; i<face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for (i = 0; i<face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for (i = 0; i<face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;

	CalculateFacesNorm();
}

void Mesh::CreateCylinder(int nSegment, float fHeight, float fRadius)
{
	numVerts = 2 * nSegment + 2;
	pt = new Point3[numVerts];
	float segment = 360.0 / nSegment;

	for (int i = 0; i < nSegment; i++) {
		pt[i].set(fRadius*cos(DEG2RAD*i*segment), -fHeight, fRadius*sin(DEG2RAD*i*segment));
	}
	for (int i = nSegment; i < 2 * nSegment; i++) {
		pt[i].set(fRadius*cos(DEG2RAD*(i - nSegment)*segment), fHeight, fRadius*sin(DEG2RAD*(i - nSegment)*segment));
	}
	pt[2 * nSegment].set(0, -fHeight, 0);
	pt[2 * nSegment + 1].set(0, fHeight, 0);

	numFaces = 3 * nSegment;
	face = new Face[numFaces];
	int i = 0;
	for (int i = 0; i < nSegment; i++) {
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = i;
		face[i].vert[1].vertIndex = nSegment + i;
		if (i == nSegment - 1) {
			face[i].vert[2].vertIndex = nSegment;
			face[i].vert[3].vertIndex = 0;
		}
		else {
			face[i].vert[2].vertIndex = nSegment + i + 1;
			face[i].vert[3].vertIndex = i + 1;
		}
		for (int j = 0; j<face[i].nVerts; j++)
			face[0].vert[j].colorIndex = rand() % 14;
	}

	for (int i = nSegment; i < 2 * nSegment; i++) {
		face[i].nVerts = 3;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = 2 * nSegment;
		face[i].vert[1].vertIndex = i - nSegment;
		if (i == 2 * nSegment - 1)
			face[i].vert[2].vertIndex = i - nSegment - nSegment + 1;
		else
			face[i].vert[2].vertIndex = i - nSegment + 1;
		for (int j = 0; i<face[i].nVerts; i++)
			face[0].vert[j].colorIndex = rand() % 14;
	}

	for (int i = 2 * nSegment; i < 3 * nSegment; i++) {
		face[i].nVerts = 3;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = 2 * nSegment + 1;
		face[i].vert[1].vertIndex = i - nSegment;
		if (i == 3 * nSegment - 1)
			face[i].vert[2].vertIndex = i - nSegment - nSegment + 1;
		else
			face[i].vert[2].vertIndex = i - nSegment + 1;
		for (int j = 0; i<face[i].nVerts; i++)
			face[0].vert[j].colorIndex = rand() % 14;
	}

	CalculateFacesNorm();

}

void Mesh::CreateSphere(int nSlice, int nStack, float radius) {
	numVerts = 2 + nSlice * (nStack - 1);
	pt = new Point3[numVerts];

	int anglestepslice = 360 / nSlice;
	int anglestepstack = 180 / nStack;

	pt[0].set(0, 0, radius);
	pt[1].set(0, 0, -radius);

	int count = 2;
	int pl_count = 0;

	for (int i = anglestepstack; i <= 180 - anglestepstack; i += anglestepstack) {
		for (int j = 0; j < 360; j += anglestepslice) {
			pt[count].set(radius * cos(j * PI / 180.0) * sin(i * PI / 180.0),
				radius * sin(j * PI / 180.0) * sin(i * PI / 180.0),
				radius * cos(i * PI / 180.0));
			count++;
		}
	}
	numFaces = nSlice * nStack;
	face = new Face[numFaces];

	for (int i = 0; i < nSlice; i++) {
		face[pl_count].nVerts = 3;
		face[pl_count].vert = new VertexID[face[i].nVerts];
		face[pl_count].vert[0].vertIndex = 0;
		face[pl_count].vert[1].vertIndex = i + 2;
		if (i < nSlice - 1) face[i].vert[2].vertIndex = i + 3;
		else face[nSlice - 1].vert[2].vertIndex = 2;
		for (int j = 0; j < face[i].nVerts; j++)
			face[i].vert[j].colorIndex = i % 14;
		pl_count++;
	}

	for (int i = 0; i < nStack - 2; i++) {
		for (int j = 0; j < nSlice; j++) {
			face[pl_count].nVerts = 4;
			face[pl_count].vert = new VertexID[face[pl_count].nVerts];
			face[pl_count].vert[0].vertIndex = i*nSlice + j + 2;
			face[pl_count].vert[1].vertIndex = (i + 1)*nSlice + j + 2;
			if (j < nSlice - 1) {
				face[pl_count].vert[2].vertIndex = (i + 1)*nSlice + j + 3;
				face[pl_count].vert[3].vertIndex = i*nSlice + j + 3;
			}
			else {
				face[pl_count].vert[2].vertIndex = (i + 1)*nSlice + 2;
				face[pl_count].vert[3].vertIndex = i*nSlice + 2;
			}
			for (int i = 0; i < face[pl_count].nVerts; i++)
				face[pl_count].vert[i].colorIndex = pl_count % 14;
			pl_count++;
		}
	}

	for (int i = nSlice * (nStack - 2); i < nSlice * (nStack - 1); i++) {
		face[pl_count].nVerts = 3;
		face[pl_count].vert = new VertexID[face[pl_count].nVerts];
		face[pl_count].vert[0].vertIndex = 1;
		face[pl_count].vert[1].vertIndex = i + 2;
		if (i < nSlice * (nStack - 1) - 1) face[pl_count].vert[2].vertIndex = i + 3;
		else face[pl_count].vert[2].vertIndex = nSlice * (nStack - 2) + 2;
		for (int j = 0; j < face[pl_count].nVerts; j++)
			face[pl_count].vert[j].colorIndex = i % 14;
		pl_count++;
	}

	CalculateFacesNorm();
}
int j = 0;
void Mesh::CreateLavie() {
	double c = PI / 180;
	double r = 1.25;
	double d = 10;
	// head
	numVerts = 10000;
	pt = new Point3[numVerts];

	int i = 0;
	for (int alpha = 0; alpha <= 360; alpha += 10)
	{
		double alphar = c*alpha;
		double z = d;
		double tr = r;

		while (z <= d + 3) //paraboloic
		{
			pt[i].set(tr*cos(alphar + c * 10), tr*sin(alphar + c * 10), z);
			pt[i + 1].set(tr*cos(alphar), tr*sin(alphar), z);
			i = i + 2;
			tr = tr + 0.05 *z / d - 0.09*z*z / d / d;
			z = z + 0.25;
		}

		glVertex3d((tr + 0.08)*cos(alphar + c * 10), (tr + 0.08)*sin(alphar + c * 10), z);
		glVertex3d((tr + 0.08)*cos(alphar), (tr + 0.08)*sin(alphar), z);
		while (z <= d + 4) //cylinder
		{
			pt[i].set(tr*cos(alphar + c * 10), tr*sin(alphar + c * 10), z);
			pt[i + 1].set(tr*cos(alphar), tr*sin(alphar), z);
			i = i + 2;
			z = z + 0.25;
		}
	}
	// body
	for (int alpha = 0; alpha <= 360; alpha += 10)
	{
		double alphar = c*alpha;
		double z = d;
		double tr = r;
		while (z > 0)
		{
			pt[i].set(tr*cos(alphar + c * 10), tr*sin(alphar + c * 10), z);
			pt[i + 1].set(tr*cos(alphar), tr*sin(alphar), z);
			if (z > d - 0.25 && z <= d) tr = tr - 0.15;
			else if (z > d - 0.5 && z <= d - 0.25) tr = tr + 0.15;
			if (z > d - 2.75 && z <= d - 2.5) tr = tr - 0.15;
			else if (z > d - 3 && z <= d - 2.75) tr = tr + 0.15;
			if (z <= 0.5) tr = tr - 0.05;
			z = z - 0.25;
			i = i + 2;
		}
	}
	cout << i << endl;
	numFaces = i;
	face = new Face[numFaces];
	for (j = 0; j <= i / 2; j++) {
		face[j].nVerts = 4;
		face[j].vert = new VertexID[face[j].nVerts];
		face[j].vert[0].vertIndex = j;
		face[j].vert[1].vertIndex = j + 1;
		face[j].vert[2].vertIndex = j + 2;
		face[j].vert[3].vertIndex = j + 3;
	}

	CalculateFacesNorm();
}

void Mesh::CreateYPlane(int nXSegment, int nZSegment, float fXWidth, float fZWidth)
{
	int i, j;
	int colorPlane;

	numVerts = (nXSegment + 1) * (nZSegment + 1);
	pt = new Point3[numVerts];

	for (i = 0; i <= nXSegment; i++)
		for (j = 0; j <= nZSegment; j++)
			pt[i * (nZSegment + 1) + j].set(
				-fXWidth / 2 + i * fXWidth / nXSegment, 0,
				-fZWidth / 2 + j * fZWidth / nZSegment);

	numFaces = nXSegment * nZSegment;
	face = new Face[numFaces];

	for (i = 0; i < nXSegment; i++)
		if (i % 2 == 0) {
			colorPlane = 12;	// mau den
			for (j = 0; j < nZSegment; j++) {
				face[i * nZSegment + j].nVerts = 4;
				face[i * nZSegment + j].vert =
					new VertexID[face[i * nZSegment + j].nVerts];
				face[i * nZSegment + j].vert[0].vertIndex = i * (nZSegment + 1) + j;
				face[i * nZSegment + j].vert[0].colorIndex = colorPlane;
				face[i * nZSegment + j].vert[1].vertIndex = i * (nZSegment + 1) + j + 1;
				face[i * nZSegment + j].vert[1].colorIndex = colorPlane;
				face[i * nZSegment + j].vert[2].vertIndex = (i + 1) * (nZSegment + 1) + j + 1;
				face[i * nZSegment + j].vert[2].colorIndex = colorPlane;
				face[i * nZSegment + j].vert[3].vertIndex = (i + 1) * (nZSegment + 1) + j;
				face[i * nZSegment + j].vert[3].colorIndex = colorPlane;

				colorPlane = 25 - colorPlane;	// hoan mau den-trang
			}
		}
		else {
			colorPlane = 13;	// mau trang
			for (j = 0; j < nZSegment; j++) {
				face[i * nZSegment + j].nVerts = 4;
				face[i * nZSegment + j].vert =
					new VertexID[face[i * nZSegment + j].nVerts];
				face[i * nZSegment + j].vert[0].vertIndex = i * (nZSegment + 1) + j;
				face[i * nZSegment + j].vert[0].colorIndex = colorPlane;
				face[i * nZSegment + j].vert[1].vertIndex = i * (nZSegment + 1) + j + 1;
				face[i * nZSegment + j].vert[1].colorIndex = colorPlane;
				face[i * nZSegment + j].vert[2].vertIndex = (i + 1) * (nZSegment + 1) + j + 1;
				face[i * nZSegment + j].vert[2].colorIndex = colorPlane;
				face[i * nZSegment + j].vert[3].vertIndex = (i + 1) * (nZSegment + 1) + j;
				face[i * nZSegment + j].vert[3].colorIndex = colorPlane;

				colorPlane = 25 - colorPlane;	// hoan mau den-trang
			}
		}


		CalculateFacesNorm();
}


void Mesh::DrawWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;

			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Mesh::DrawColor()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;
			int		ic = face[f].vert[v].colorIndex;

			glColor3f(ColorArr[ic][0], ColorArr[ic][1], ColorArr[ic][2]);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Mesh::DrawDisColor()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;
			int		ic = face[f].vert[v].colorIndex;

			ic = f % (COLORNUM - 1); // bo mau trang

			glColor3f(ColorArr[ic][0], ColorArr[ic][1], ColorArr[ic][2]);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

int Mesh::GetColour(int& colour) {
	if (colour > COLORNUM) {
		colour = 0;
		return colour;
	}
	else {
		colour++;
		return colour;
	}
}

void Mesh::Draw()
{
	for (int f = 0; f < numFaces; f++) {
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;

			glNormal3f(face[f].facenorm.x, face[f].facenorm.y, face[f].facenorm.z);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();

	}
}

void Mesh::DrawFloor()
{
	for (int f = 0; f < numFaces; f++) {
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;

			glNormal3f(face[f].facenorm.x, face[f].facenorm.y, face[f].facenorm.z);
			if (v == 0)
				glTexCoord2f(0., 0.);
			else if (v == 1)
				glTexCoord2f(1., 0.);
			else if (v == 2)
				glTexCoord2f(1., 1.);
			else
				glTexCoord2f(0., 1.);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();

	}
}


void Mesh::CreateGeometryShape(float R1, float R2, float Height, float distance) {
	float C1[3];
	float C2[3];
	int colour_val = -1;
	float D1[3];
	float D2[3];
	float p[8];
	C1[0] = 0;
	C1[1] = 0;
	C1[2] = R1;
	C2[0] = R1 + R2 + distance;
	C2[1] = 0;
	C2[2] = R2;

	float xA = 0;
	float yA = 0;
	TangentLine_Get_Through(C1, C2, xA, yA);
	TangentLine(C1, C2, xA, yA, D1, D2);
	float a1 = D1[0];
	float b1 = D1[1];
	float c1 = D1[2];
	float a2 = D2[0];
	float b2 = D2[1];
	float c2 = D2[2];
	TangentLine_Cut_Circle(C1, C2, xA, yA, D1, D2, p);

	float c = PI / 180;
	int split = 100;
	float angel = 0;
	float x_cal = 0;
	float y_cal = 0;

	numVerts = 4 * split;
	numFaces = 2 * (split - 1) + 4;
	pt = new Point3[numVerts];
	face = new Face[numFaces];
	int run = 0;

	if (R1 < R2) {
		float v1_x = p[0] - xA;
		float v1_y = p[1] - yA;
		float cos_val = abs(v1_x) / (sqrt(v1_x*v1_x + v1_y*v1_y));
		float phi_start = 180 * c - (90 * c - acos(cos_val));

		for (float i = 0; i<split; i++) {
			angel = phi_start + i * 2 * (180 * c - phi_start) / split;
			x_cal = C1[0] + R1*cos(angel);
			y_cal = C1[1] + R1*sin(angel);
			pt[run].set(x_cal, 0, y_cal);
			run++;
		}

		for (float i = 0; i<split; i++) {
			angel = phi_start + i * 2 * (180 * c - phi_start) / split;
			x_cal = C1[0] + R1*cos(angel);
			y_cal = C1[1] + R1*sin(angel);
			pt[run].set(x_cal, Height, y_cal);
			run++;
		}

		v1_x = p[2] - xA;
		v1_y = p[3] - yA;
		cos_val = abs(v1_x) / (sqrt(v1_x*v1_x + v1_y*v1_y));
		phi_start = (90 * c - acos(cos_val));

		for (float i = 0; i<split; i++) {
			angel = (180 * c - phi_start) - i * 2 * (180 * c - phi_start) / split;
			x_cal = C2[0] + R2*cos(angel);
			y_cal = C2[1] + R2*sin(angel);
			pt[run].set(x_cal, 0, y_cal);
			run++;
		}

		for (float i = 0; i<split; i++) {
			angel = (180 * c - phi_start) - i * 2 * (180 * c - phi_start) / split;
			x_cal = C2[0] + R2*cos(angel);
			y_cal = C2[1] + R2*sin(angel);
			pt[run].set(x_cal, Height, y_cal);
			run++;
		}
	}
	else if (R1>R2) {

		float v1_x = p[0] - xA;
		float v1_y = p[1] - yA;
		float cos_val = abs(v1_x) / (sqrt(v1_x*v1_x + v1_y*v1_y));
		float phi_start = (90 * c - acos(cos_val));

		for (float i = 0; i<split; i++) {
			angel = phi_start + i * 2 * (180 * c - phi_start) / split;
			x_cal = C1[0] + R1*cos(angel);
			y_cal = C1[1] + R1*sin(angel);
			pt[run].set(x_cal, 0, y_cal);
			run++;
		}

		for (float i = 0; i<split; i++) {
			angel = phi_start + i * 2 * (180 * c - phi_start) / split;
			x_cal = C1[0] + R1*cos(angel);
			y_cal = C1[1] + R1*sin(angel);
			pt[run].set(x_cal, Height, y_cal);
			run++;
		}

		v1_x = p[2] - xA;
		v1_y = p[3] - yA;
		cos_val = abs(v1_x) / (sqrt(v1_x*v1_x + v1_y*v1_y));
		phi_start = (90 * c - acos(cos_val));

		for (float i = 0; i<split; i++) {
			angel = (phi_start)-i * 2 * (phi_start) / split;
			x_cal = C2[0] + R2*cos(angel);
			y_cal = C2[1] + R2*sin(angel);
			pt[run].set(x_cal, 0, y_cal);
			run++;
		}

		for (float i = 0; i<split; i++) {
			angel = (phi_start)-i * 2 * (phi_start) / split;
			x_cal = C2[0] + R2*cos(angel);
			y_cal = C2[1] + R2*sin(angel);
			pt[run].set(x_cal, Height, y_cal);
			run++;
		}
	}
	else {
		float phi_start = (90 * c);

		for (float i = 0; i<split; i++) {
			angel = phi_start + i * 2 * (180 * c - phi_start) / split;
			x_cal = C1[0] + R1*cos(angel);
			y_cal = C1[1] + R1*sin(angel);
			pt[run].set(x_cal, 0, y_cal);
			run++;
		}

		for (float i = 0; i<split; i++) {
			angel = phi_start + i * 2 * (180 * c - phi_start) / split;
			x_cal = C1[0] + R1*cos(angel);
			y_cal = C1[1] + R1*sin(angel);
			pt[run].set(x_cal, Height, y_cal);
			run++;
		}

		phi_start = (90 * c);

		for (float i = 0; i<split; i++) {
			angel = (phi_start)-i * 2 * (phi_start) / split;
			x_cal = C2[0] + R2*cos(angel);
			y_cal = C2[1] + R2*sin(angel);
			pt[run].set(x_cal, 0, y_cal);
			run++;
		}

		for (float i = 0; i<split; i++) {
			angel = (phi_start)-i * 2 * (phi_start) / split;
			x_cal = C2[0] + R2*cos(angel);
			y_cal = C2[1] + R2*sin(angel);
			pt[run].set(x_cal, Height, y_cal);
			run++;
		}
	}

	run = 0;
	for (int i = 0; i < split - 1; i++) {
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		if (i == 0) {
			face[i].vert[0].vertIndex = 0;
			face[i].vert[1].vertIndex = 1;
			face[i].vert[3].vertIndex = split;
			face[i].vert[2].vertIndex = split + 1;
		}
		else if (i == split - 3) {
			face[i].vert[0].vertIndex = face[i - 1].vert[1].vertIndex;
			face[i].vert[1].vertIndex = face[i - 1].vert[1].vertIndex + 1;
			face[i].vert[3].vertIndex = face[i - 1].vert[2].vertIndex;
			face[i].vert[2].vertIndex = face[i - 1].vert[2].vertIndex + 1;
		}
		else {
			face[i].vert[0].vertIndex = face[i - 1].vert[1].vertIndex;
			face[i].vert[1].vertIndex = face[i - 1].vert[1].vertIndex + 1;
			face[i].vert[3].vertIndex = face[i - 1].vert[2].vertIndex;
			face[i].vert[2].vertIndex = face[i - 1].vert[2].vertIndex + 1;
		}
		this->GetColour(colour_val);
		for (int k = 0; k<face[i].nVerts; k++)
			face[i].vert[k].colorIndex = colour_val;

	}

	for (int i = split - 1; i < 2 * split - 2; i++) {
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		if (i == split - 1) {
			face[i].vert[0].vertIndex = 2 * split;
			face[i].vert[1].vertIndex = 2 * split + 1;
			face[i].vert[3].vertIndex = 3 * split;
			face[i].vert[2].vertIndex = 3 * split + 1;
		}
		else {
			face[i].vert[0].vertIndex = face[i - 1].vert[1].vertIndex;
			face[i].vert[1].vertIndex = face[i - 1].vert[1].vertIndex + 1;
			face[i].vert[3].vertIndex = face[i - 1].vert[2].vertIndex;
			face[i].vert[2].vertIndex = face[i - 1].vert[2].vertIndex + 1;
		}
		this->GetColour(colour_val);
		for (int k = 0; k<face[i].nVerts; k++)
			face[i].vert[k].colorIndex = colour_val;
	}

	face[2 * split - 2].nVerts = 4;
	face[2 * split - 2].vert = new VertexID[face[2 * split - 2].nVerts];
	face[2 * split - 2].vert[0].vertIndex = split - 1;
	face[2 * split - 2].vert[1].vertIndex = 2 * split - 1;
	face[2 * split - 2].vert[3].vertIndex = 3 * split - 1;
	face[2 * split - 2].vert[2].vertIndex = 4 * split - 1;
	this->GetColour(colour_val);
	for (int k = 0; k<face[2 * split - 2].nVerts; k++)
		face[2 * split - 2].vert[k].colorIndex = colour_val;

	face[2 * split - 1].nVerts = 4;
	face[2 * split - 1].vert = new VertexID[face[2 * split - 2].nVerts];
	face[2 * split - 1].vert[0].vertIndex = 0;
	face[2 * split - 1].vert[1].vertIndex = split;
	face[2 * split - 1].vert[2].vertIndex = 3 * split;
	face[2 * split - 1].vert[3].vertIndex = 2 * split;

	this->GetColour(colour_val);
	for (int k = 0; k < face[2 * split - 1].nVerts; k++)
		face[2 * split - 1].vert[k].colorIndex = colour_val;


	face[2 * split].nVerts = 2 * split;
	face[2 * split].vert = new VertexID[face[2 * split].nVerts];
	run = 0;
	for (int i = split; i < 2 * split; i++) {
		face[2 * split].vert[run].vertIndex = i;
		run++;
	}
	for (int i = 4 * split - 1; i > 3 * split - 1; i--) {
		face[2 * split].vert[run].vertIndex = i;
		run++;
	}
	this->GetColour(colour_val);
	for (int k = 0; k < face[2 * split].nVerts; k++)
		face[2 * split].vert[k].colorIndex = colour_val;

	face[2 * split + 1].nVerts = 2 * split;
	face[2 * split + 1].vert = new VertexID[face[2 * split + 1].nVerts];

	run = 0;
	for (int i = 0; i < split; i++) {
		face[2 * split + 1].vert[run].vertIndex = i;
		run++;
	}
	for (int i = 3 * split - 1; i > 2 * split - 1; i--) {
		face[2 * split + 1].vert[run].vertIndex = i;
		run++;
	}
	this->GetColour(colour_val);
	for (int k = 0; k < face[2 * split + 1].nVerts; k++)
		face[2 * split + 1].vert[k].colorIndex = colour_val;

	CalculateFacesNorm();
}

void Mesh::TangentLine_Cut_Circle(float C1[3], float C2[3], float x, float y, float *D1, float *D2, float* p) {
	float heso_a = (-D1[1])*(-D1[1]) + 1;
	float heso_b = 2 * (-D1[1])*(-D1[2] - C1[0]) - 2 * (C1[1]);
	float heso_c = (-D1[2] - C1[0])*(-D1[2] - C1[0]) + C1[1] * C1[1] - C1[2] * C1[2];
	float fillForFull = 0;
	SolveQuadFormula(heso_a, heso_b, heso_c, p[1], fillForFull);
	p[0] = -D1[1] * p[1] - D1[2];

	heso_a = (-D1[1])*(-D1[1]) + 1;
	heso_b = 2 * (-D1[1])*(-D1[2] - C2[0]) - 2 * (C2[1]);
	heso_c = (-D1[2] - C2[0])*(-D1[2] - C2[0]) + C2[1] * C2[1] - C2[2] * C2[2];
	SolveQuadFormula(heso_a, heso_b, heso_c, p[3], fillForFull);
	p[2] = -D1[1] * p[3] - D1[2];

	heso_a = (-D2[1])*(-D2[1]) + 1;
	heso_b = 2 * (-D2[1])*(-D2[2] - C1[0]) - 2 * (C1[1]);
	heso_c = (-D2[2] - C1[0])*(-D2[2] - C1[0]) + C1[1] * C1[1] - C1[2] * C1[2];
	SolveQuadFormula(heso_a, heso_b, heso_c, p[5], fillForFull);
	p[4] = -D2[1] * p[5] - D2[2];

	heso_a = (-D2[1])*(-D2[1]) + 1;
	heso_b = 2 * (-D2[1])*(-D2[2] - C2[0]) - 2 * (C2[1]);
	heso_c = (-D2[2] - C2[0])*(-D2[2] - C2[0]) + C2[1] * C2[1] - C2[2] * C2[2];
	SolveQuadFormula(heso_a, heso_b, heso_c, p[7], fillForFull);
	p[6] = -D2[1] * p[7] - D2[2];
}

void Mesh::TangentLine(float C1[3], float C2[3], float xA, float yA, float *D1, float *D2) {
	float first = (C1[0] - xA) / C1[2];
	float second = (C1[1] - yA) / C1[2];

	D1[0] = D2[0] = 1;
	float heso_a = second*second - 1;
	float heso_b = 2 * first*second;
	float heso_c = first*first - 1;
	float val1 = 0;
	float val2 = 0;
	SolveQuadFormula(heso_a, heso_b, heso_c, D1[1], D2[1]);
	D1[2] = -D1[0] * xA - D1[1] * yA;
	D2[2] = -D2[0] * xA - D2[1] * yA;
}

void Mesh::TangentLine_Get_Through(float C1[3], float C2[3], float& x, float& y) {
	float ratio = C1[2] / C2[2];
	x = (ratio*C2[0] - C1[0]) / (ratio - 1);
	y = (ratio*C2[1] - C1[1]) / (ratio - 1);
}

void Mesh::SolveQuadFormula(float a, float b, float c, float& val1, float& val2) {
	float delta = b*b - 4 * a*c;
	val1 = (-b + sqrt(abs(delta))) / (2 * a);
	val2 = (-b - sqrt(abs(delta))) / (2 * a);
}

void Mesh::CreateFloor(float fSize)
{
	for (int f = 0; f < numFaces; f++) {
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;

			glNormal3f(face[f].facenorm.x, face[f].facenorm.y, face[f].facenorm.z);
			if (v == 0)
				glTexCoord2f(0., 0.);
			else if (v == 1)
				glTexCoord2f(1., 0.);
			else if (v == 2)
				glTexCoord2f(1., 1.);
			else
				glTexCoord2f(0., 1.);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();

	}
}

void Mesh::CreateFloor_test() {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
		glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
		glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
	}
	glEnd();
}


//---------------------------------------------------------------------mainsource.cpp
int		screenWidth = 600;
int		screenHeight = 600;

//	cac thong so cua Camera
float camera_angle;
float camera_height;
float camera_dis;
float camera_X, camera_Y, camera_Z;
float lookAt_X, lookAt_Y, lookAt_Z;

bool	bWireFrame = false;

//	cac thong so cua mo hinh
float	YPlanePos = 0;


double	baseRadius = 0.6;
double	baseHeight = 0.1;
double	baseRotateStep = 5;

double	cylRadius = 0.3;
double	cylHeight = 1.0;
double  cylMaxScaleY = 1;
double	cylScaleStep = 0.02;

double	body1SizeX = 3;
double	body1SizeY = 0.2;
double	body1SizeZ = 0.8;

double   body2SizeX = 0.75;
double   body2SizeY = 0.35;
double   body2SizeZ = 0.8;

double   body3SizeX = 2.25;
double   body3SizeY = 0.2;
double   body3SizeZ = 0.8 / 3;

double   body4SizeX = 2.25;
double   body4SizeY = 0.2;
double   body4SizeZ = 0.8 / 3;

double   sliderSizeX = 0.25;
double   sliderSizeY = 0.45;
double   sliderSizeZ = 0.8 / 3;
double   sliderSlide = -2.4;
double   sliderSlideStep = 0.019;

double   hand1R1 = 0.2;
double   hand1R2 = 0.4;
double   hand1Height = 0.2;
double   hand1Distance = 0.9;
double   hand1RotateStep = 0.9;
double   hand1Rotate = 0;

double   hand2R1 = 0.2;
double   hand2R2 = 0.2;
double   hand2Height = 0.2;
double   hand2Distance = 2.6;
float   hand2RotateStep = 0.57;
double   hand2Rotate = 0;

double  pivot1SizeX = 0.3;
double  pivot1Height = 0.22;

double  pivot2Radius = 0.1;
double  pivot2Height = 0.22;

double  pivot3Radius = 0.1;
double  pivot3Height = 0.22;

bool	b4View = false;

bool	cartoonMode = true;
bool	changeCylBase = false;
bool	changeCylAxis = false;
bool	changeCuboidAxis = false;
bool	changeHand = false;

//-----------------------------------------------ass2

int ball;

// chuyen doi giua 1 va 2 nguon sang
bool	twoLightSource = false;

//	chuyen doi giua 1-4 view
bool	b4view = false;

// toa do chuot
float	mouseX, mouseY;

// kiem tra xem object co duoc pick khong
bool	pickHand1 = false;
bool	pickHand2 = false;
bool	pickSlider = false;
//bool	pickTrunc = false;

// kiem tra mau duoc nhap vao de thay doi
bool	changeToRed = false;
bool	changeToGreen = false;
bool	changeToBlue = false;
bool	changeToPurple = false;

// mau cua vat lieu
GLfloat red[] = { 1.0f, 0.0f, 0.0f,1.0f };
GLfloat green[] = { 0.0f, 1.0f, 0.0f,1.0f };
GLfloat blue[] = { 0.0f, 0.0f, 1.0f,1.0f };
GLfloat golden[] = { 1.0f, 0.5f, 0.25f };
GLfloat black[] = { 0.0f, 0.0f, 0.0f,1.0f };
GLfloat white[] = { 1.0f, 1.0f, 1.0f,1.0f };
GLfloat yellow[] = { 1.0f, 1.0f, 0.0f,1.0f };
GLfloat purple[] = { 1.0f, 0.0f, 1.0f,1.0f };
GLfloat almostpink[] = { 1.0f, 0.0f, 0.5f, 1.0f };
GLfloat gray[] = { 0.5f, 0.5f, 0.5f,1.0f };

Point3 ClosestPoint(Point3 A, Point3 B, Point3 P)
{
	Vector3 AB;
	AB.set(B.x - A.x, B.y - A.y, B.z - A.z);
	double ab = AB.dot(AB);
	Vector3 AP;
	AP.set(P.x - A.x, P.y - A.y, P.z - A.z);
	double ap_ab = AP.dot(AB);
	float t = ap_ab / ab;
	Point3 Q;
	Q.set(A.x + AB.x*t, A.y + AB.y*t, A.z + AB.z*t);
	return Q;
}

// thiet lap mau sac ban dau cua cac khoi
// co the picking
// set color default as purple color
GLfloat tmpSphereColor[] = { 1.0, 0.0, 1.0,1.0 };
// set color default as blue color
GLfloat tmpDedoColor[] = { 0.0f, 0.0f, 1.0f,1.0f };
// set color default as green color
GLfloat tmpIcoColor[] = { 0.0f, 1.0f, 0.0f,1.0f };
// set color default as golden color
GLfloat tmpTruncColor[] = { 1.0f, 0.5f, 0.25f };




Mesh	base;
Mesh	cyl;
Mesh	body1;
Mesh    body2;
Mesh    body3;
Mesh    body4;
Mesh    slider;
Mesh    hand1;
Mesh    hand2;
Mesh    pivot1;
Mesh    pivot2;
Mesh    pivot3;

void drawAxis()
{

	glPushMatrix();

	glColor3f(0, 0, 1);
	glPointSize(5);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(-4, 0, 0);//x
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);//y
	glVertex3f(0, 4, 0);


	glColor3f(0, 0, 1);
	glVertex3f(0, 0, -4);//z
	glVertex3f(0, 0, 4);
	glEnd();

	glPopMatrix();
}
bool isLeft = true;
bool isRight = false;

void changeCartoonMode(int v)
{
	if (changeCuboidAxis) {
		base.rotateY += baseRotateStep;
		if (base.rotateY > 360)
			base.rotateY -= 360;
	}
	else {
		base.rotateY -= baseRotateStep;
		if (base.rotateY < 0)
			base.rotateY += 360;
	}

	if (changeCylAxis) {
		if (cyl.scaleY <= cylMaxScaleY)
			cyl.scaleY += cylScaleStep;
	}
	else
	{
		if (cyl.scaleY >= cylHeight*0.5)
			cyl.scaleY -= cylScaleStep;
	}

	if (changeHand) {
		hand1Rotate += hand1RotateStep;
	}
	else
	{
		hand1Rotate -= hand1RotateStep;
	}

	glutPostRedisplay();
	if (cartoonMode)
		glutTimerFunc(50, changeCartoonMode, 0);
}

void changeCameraPos()
{
	camera_Y = camera_height;
	camera_X = camera_dis * cos(camera_angle);
	camera_Z = camera_dis * sin(camera_angle);
}

void myKeyboard(unsigned char key, int x, int y)
{
	float	fRInc;
	float	fAngle;

	switch (key)
	{
	case '+':
		camera_dis = camera_dis + 0.2;
		break;
	case '-':
		camera_dis = camera_dis - 0.2;
		break;
	case 'v':
	case 'V':
		b4view = !b4view;
		break;
	case '1':
		base.rotateY += baseRotateStep;
		if (base.rotateY > 360)
			base.rotateY -= 360;
		break;
	case '2':
		base.rotateY -= baseRotateStep;
		if (base.rotateY < 0)
			base.rotateY += 360;
		break;
		//case 'l':
		//	if (cyl.scaleY <= cylMaxScaleY)
		//		cyl.scaleY += cylScaleStep;
		//	break;
	case 'x':
		if (cyl.scaleY >= cylHeight*0.5)
			cyl.scaleY -= cylScaleStep;
		break;
	case '3':

		hand1Rotate += hand1RotateStep;
		if (hand1Rotate >= 360)
			hand1Rotate -= 360;

		break;
	case '4':

		hand1Rotate -= hand1RotateStep;
		if (hand1Rotate <= -360)
			hand1Rotate += 360;
		break;
	case 'w':
	case 'W':
		bWireFrame = !bWireFrame;
		break;

	case 'A':
	case 'a':
		cartoonMode = !cartoonMode;
		if (cartoonMode)
			glutTimerFunc(50, changeCartoonMode, 0);
		break;

	case 'l':
		if (cyl.scaleY <= cylMaxScaleY)
			cyl.scaleY += cylScaleStep;
		break;

	case 'd':
	case 'D':
		twoLightSource = !twoLightSource;
		if (!twoLightSource)
			glDisable(GL_LIGHT1);
		else
			glEnable(GL_LIGHT1);
		break;


	case 'R':
	case 'r':
		changeToRed = true;
		changeToBlue = false;
		changeToGreen = false;
		changeToPurple = false;
		break;
	case 'G':
	case 'g':
		changeToRed = false;
		changeToBlue = false;
		changeToGreen = true;
		changeToPurple = false;
		break;
	case 'B':
	case 'b':
		changeToRed = false;
		changeToBlue = true;
		changeToGreen = false;
		changeToPurple = false;
		break;
	case 'P':
	case 'p':
		changeToRed = false;
		changeToBlue = false;
		changeToGreen = false;
		changeToPurple = true;
		break;
	}
	glutPostRedisplay();
}



// function to calculate distance between the center of
// object and the picking ray

double getDistance(Point3 A, Point3 B, Point3 P)
{
	Vector3 AB;
	AB.set(B.x - A.x, B.y - A.y, B.z - A.z);
	double ab_square = AB.dot(AB);
	Vector3 AP;
	AP.set(P.x - A.x, P.y - A.y, P.z - A.z);
	double ap_dot_ab = AP.dot(AB);

	float t = ap_dot_ab / ab_square;
	Point3 Q;
	Q.set(A.x + AB.x*t, A.y + AB.y*t, A.z + AB.z*t);

	Vector3 vectorDist;
	vectorDist.set(Q.x - P.x, Q.y - P.y, Q.z - P.z);

	return sqrt(vectorDist.x * vectorDist.x + vectorDist.y * vectorDist.y + vectorDist.z * vectorDist.z);
}


void myMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// set all changeToColor equal to false
		changeToBlue = false;
		changeToGreen = false;
		changeToPurple = false;
		changeToRed = false;

		// This function will find 2 points in world space that are on the line into the screen defined by screen-space
		// (ie. window-space ) point (x,y)
		double mvmatrix[16];
		double projmatrix[16];
		int viewport[4];
		double dX, dY, dZ, dClickY;

		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
		dClickY = double(viewport[3] - y); // OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top

		gluUnProject((double)x, dClickY, 0.0, mvmatrix, projmatrix, viewport, &dX, &dY, &dZ);
		Point3 ClickRayP1 = Point3((float)dX, (float)dY, (float)dZ);	// near clipping plane
		gluUnProject((double)x, dClickY, 1.0, mvmatrix, projmatrix, viewport, &dX, &dY, &dZ);
		Point3 ClickRayP2 = Point3((float)dX, (float)dY, (float)dZ);	// far clipping plane

		Point3 centerObject[3];
		centerObject[0].set(-3.5, 0.5, -3.5);	// hand1
		centerObject[1].set(3.5, 0.5, 3.5);		// hand2
		centerObject[2].set(3.5, 0.45, -3.5);	// slider
		

		for (int i = 0; i<4; i++)
		{
			if (getDistance(ClickRayP1, ClickRayP2, centerObject[i]) <= 0.5)	// vector inside the object
			{
				if (i == 0)
				{
					pickHand1 = true;
					pickHand2 = false;
					pickSlider = false;
					break;
				}
				else if (i == 1)
				{
					pickHand1 = false;
					pickHand2 = true;
					pickSlider = false;
					break;
				}
				else if (i == 2)
				{
					pickHand1 = false;
					pickHand2 = false;
					pickSlider = true;
					break;
				}
			}
			else		// picking position outside all of the objects
			{
				pickHand1 = false;
				pickHand2 = false;
				pickSlider = false;
				
			}
		}
	}
	glutPostRedisplay();
}

void mySetupCameraVolume(int nType)
{
	if (nType == 4)
	{
		glMatrixMode(GL_PROJECTION);			// set projection matrix current matrix
		glLoadIdentity();						// reset projection matrix

												// calculate aspect ratio of window
		gluPerspective(60.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 1.0f, 1000.0f);
	}
	else
	{
		glMatrixMode(GL_PROJECTION);			// set projection matrix current matrix
		glLoadIdentity();						// reset projection matrix
		glOrtho(-5, 5, -5, 5, -1000, 1000);
	}

}

////////////////////////////////////////
//		Return and multiply shadow	////
//			 matrix projection		////
////////////////////////////////////////

void shadowMatrix(float plane[], float light_pos[]) {
	float shadow_mat[16];
	float dot;

	//Finds dot, which is the product of the light vetcor and the plane's normal
	dot = plane[0] * light_pos[0] +
		plane[1] * light_pos[1] +
		plane[2] * light_pos[2] +
		plane[3] * light_pos[3];

	shadow_mat[0] = dot - light_pos[0] * plane[0];
	shadow_mat[4] = -light_pos[0] * plane[1];
	shadow_mat[8] = -light_pos[0] * plane[2];
	shadow_mat[12] = -light_pos[0] * plane[3];

	shadow_mat[1] = -light_pos[1] * plane[0];
	shadow_mat[5] = dot - light_pos[1] * plane[1];
	shadow_mat[9] = -light_pos[1] * plane[2];
	shadow_mat[13] = -light_pos[1] * plane[3];

	shadow_mat[2] = -light_pos[2] * plane[0];
	shadow_mat[6] = -light_pos[2] * plane[1];
	shadow_mat[10] = dot - light_pos[2] * plane[2];
	shadow_mat[14] = -light_pos[2] * plane[3];

	shadow_mat[3] = -light_pos[3] * plane[0];
	shadow_mat[7] = -light_pos[3] * plane[1];
	shadow_mat[11] = -light_pos[3] * plane[2];
	shadow_mat[15] = dot - light_pos[3] * plane[3];

	glMultMatrixf(shadow_mat);
}

/////////////////////////////////////////
//		Setup Material Functions      ///
////////////////////////////////////////

void setupMaterial(float ambient[], float diffuse[], float specular[], float shiness)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiness);
}



////////////////////////////////
//		Display Functions	////
////////////////////////////////

//GLfloat light_position1[] = { 6.0f, 6.0f, 6.0f, 0.0f };
//GLfloat light_position2[] = { -6.0f, 6.0f, -6.0f, 0.0f };

void drawBorder(float fSize) {
	glColor3f(0, 0, 1);
	glLineWidth(3.);
	setupMaterial(blue, blue, blue, 1.0);
	glBegin(GL_LINE_LOOP);

	glVertex3f(-fSize / 2, fSize / 2, fSize / 2);
	glVertex3f(-fSize / 2, fSize / 2, -fSize / 2);
	glVertex3f(fSize / 2, fSize / 2, -fSize / 2);
	glVertex3f(fSize / 2, fSize / 2, fSize / 2);


	glEnd();
	glBegin(GL_LINES);
	glVertex3f(-fSize / 2, fSize / 2, fSize / 2);
	glVertex3f(-fSize / 2, -fSize / 2, fSize / 2);
	glVertex3f(-fSize / 2, fSize / 2, -fSize / 2);
	glVertex3f(-fSize / 2, -fSize / 2, -fSize / 2);
	glVertex3f(fSize / 2, fSize / 2, -fSize / 2);
	glVertex3f(fSize / 2, -fSize / 2, -fSize / 2);
	glVertex3f(fSize / 2, fSize / 2, fSize / 2);
	glVertex3f(fSize / 2, -fSize / 2, fSize / 2);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(fSize / 2, -fSize / 2, fSize / 2);
	glVertex3f(-fSize / 2, -fSize / 2, fSize / 2);
	glVertex3f(-fSize / 2, -fSize / 2, -fSize / 2);
	glVertex3f(fSize / 2, -fSize / 2, -fSize / 2);
	glEnd();

}

void drawFloor()
{
	glPushMatrix();
	glTranslated(base.slideX, base.slideY, base.slideZ);
	float fXWidth = 50;
	float fZWidth = 50;

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);



	glBindTexture(GL_TEXTURE_2D, floorTex.texID);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor4f(1, 1, 1, 0.5);

	glBegin(GL_POLYGON);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(-fXWidth / 2, 0, fZWidth / 2);


	glTexCoord2f(1.0, 0);
	glVertex3f(fXWidth / 2, 0, fZWidth / 2);


	glTexCoord2f(1.0, 1.0);
	glVertex3f(fXWidth / 2, 0, -fZWidth / 2);


	glTexCoord2f(0.0, 1.0);
	glVertex3f(-fXWidth / 2, 0, -fZWidth / 2);

	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();

}



void drawBase()
{
	glPushMatrix();

	glTranslated(0, base.slideY, 0);
	glRotatef(base.rotateY, 0, 1, 0);

	if (bWireFrame)
		base.DrawWireframe();
	else
		base.DrawColor();

	glPopMatrix();
}
void drawlCylinderBase()
{
	glPushMatrix();

	glTranslated(0, 2 * cyl.slideY*cyl.scaleY + baseHeight * 2, 0);
	glScalef(cyl.scaleX, cyl.scaleY, cyl.scaleZ);
	glRotatef(base.rotateY, 0, 1, 0);

	if (bWireFrame)
		cyl.DrawWireframe();
	else
		cyl.DrawColor();

	glPopMatrix();
}
void drawBody1()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(0, body1SizeY + 2 * cylHeight*cyl.scaleY + baseHeight * 2, 0);

	if (bWireFrame)
		body1.DrawWireframe();
	else
		body1.DrawColor();

	glPopMatrix();
}

void drawBody2()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(3 * body1SizeX / 4, body1SizeY * 2 + 2 * cylHeight*cyl.scaleY + baseHeight * 2 + body2SizeY, 0);

	if (bWireFrame)
		body2.DrawWireframe();
	else
		body2.DrawColor();

	glPopMatrix();
}

void drawBody3()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(-body1SizeX / 4, body1SizeY * 2 + 2 * cylHeight*cyl.scaleY + baseHeight * 2 + body3SizeY, 2 * body1SizeZ / 3);

	if (bWireFrame)
		body3.DrawWireframe();
	else
		body3.DrawColor();

	glPopMatrix();
}

void drawBody4()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(-body1SizeX / 4, body1SizeY * 2 + 2 * cylHeight*cyl.scaleY + baseHeight * 2 + body3SizeY, -2 * body1SizeZ / 3);

	if (bWireFrame)
		body4.DrawWireframe();
	else
		body4.DrawColor();

	glPopMatrix();
}

void drawSlider()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	float phi = hand1Rotate*DEG2RAD;
	float x = body1SizeX*0.5;
	float y = body1SizeX;
	float D = sqrt(y*y - x*sin(phi)*x*sin(phi)) + x*cos(phi);
	glTranslated(x + y - D, 0, 0);
	glTranslated(-(x + y) / 2, body1SizeY * 2 + 2 * cylHeight*cyl.scaleY + baseHeight * 2 + sliderSizeY, 0);

	if (bWireFrame)
		slider.DrawWireframe();
	else
		slider.DrawColor();

	glPopMatrix();
}

void drawHand1()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(hand1Distance / 2 + 0.3, body1SizeY * 2 + 2 * cylHeight*cyl.scaleY + baseHeight * 2 + 2 * body2SizeY, 0);
	glTranslated(1.5, 0, 0);
	glRotatef(hand1Rotate, 0, 1, 0);
	glTranslated(-1.5, 0, 0);
	if (bWireFrame)
		hand1.DrawWireframe();
	else
		hand1.DrawColor();

	glPopMatrix();
}
double z;
void drawHand2()
{
	glPushMatrix(); float phi = hand1Rotate*DEG2RAD;
	float x = body1SizeX*0.5;
	float y = body1SizeX;
	float D = sqrt(y*y - x*sin(phi)*x*sin(phi)) + x*cos(phi);
	float beta = asin((x*sin(phi)) / y) / PI*180.0;
	glRotatef(base.rotateY, 0, 1, 0);

	glTranslated(x + y - D, 0, 0);
	glTranslated(-(x + y) / 2, body1SizeY * 2 + 2 * cylHeight*cyl.scaleY + baseHeight * 2 + 2 * sliderSizeY, 0);
	glRotatef(-beta, 0, 1, 0);
	if (bWireFrame) {
		hand2.DrawWireframe();
	}
	else {
		hand2.DrawColor();
	}
	glPopMatrix();
}

void drawPivot1()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(3 * body1SizeX / 4, pivot1Height / 2 + body1SizeY * 2 + 2 * cylHeight*cyl.scaleY + baseHeight * 2 + 2 * body2SizeY, 0);
	if (bWireFrame)
		pivot1.DrawWireframe();
	else
		pivot1.DrawColor();

	glPopMatrix();
}

void drawPivot2()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	float phi = hand1Rotate*DEG2RAD;
	float x = body1SizeX*0.5;
	float y = body1SizeX;
	float D = sqrt(y*y - x*sin(phi)*x*sin(phi)) + x*cos(phi);
	glTranslated(x + y - D, 0, 0);
	glTranslated(-(x + y) / 2, pivot2Height / 2 + body1SizeY * 2 + 2 * cylHeight*cyl.scaleY + baseHeight * 2 + 2 * sliderSizeY, 0);
	if (bWireFrame)
		pivot2.DrawWireframe();
	else
		pivot2.DrawColor();
	glPopMatrix();
}

void drawPivot3()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(body1SizeX*0.75, pivot2Height / 2 + body1SizeY * 2 + 2 * cylHeight*cyl.scaleY + baseHeight * 2 + 2 * sliderSizeY, 0);
	glRotatef(hand1Rotate, 0, 1, 0);
	glTranslated(-body1SizeX / 2.0, 0, 0);
	glScalef(pivot2.scaleX, pivot2.scaleY, pivot2.scaleZ);
	if (bWireFrame)
		pivot2.DrawWireframe();
	else
		pivot2.DrawColor();
	glPopMatrix();
}

void drawFourObjectWOBorder()
{
	glPushMatrix();
	glTranslated(hand1.slideX, hand1.slideY, hand1.slideZ);
	if (bWireFrame)
		hand1.DrawWireframe();
	else
	{
		if (pickHand1)
		{
			if (changeToBlue)
				for (int i = 0; i<4; i++)
					tmpSphereColor[i] = blue[i];
			else if (changeToGreen)
				for (int i = 0; i<4; i++)
					tmpSphereColor[i] = green[i];
			else if (changeToRed)
				for (int i = 0; i<4; i++)
					tmpSphereColor[i] = red[i];
			else if (changeToPurple)
				for (int i = 0; i<4; i++)
					tmpSphereColor[i] = purple[i];
			//drawBorder(1.0);
		}
		setupMaterial(black, tmpSphereColor, white, 100);
		hand1.Draw();
	}
	glPopMatrix();

	///////////////////

	glPushMatrix();
	glTranslated(hand2.slideX, hand2.slideY, hand2.slideZ);
	if (bWireFrame)
		hand2.DrawWireframe();
	else
	{
		if (pickHand2)
		{
			if (changeToBlue)
				for (int i = 0; i<4; i++)
					tmpDedoColor[i] = blue[i];
			else if (changeToGreen)
				for (int i = 0; i<4; i++)
					tmpDedoColor[i] = green[i];
			else if (changeToRed)
				for (int i = 0; i<4; i++)
					tmpDedoColor[i] = red[i];
			else if (changeToPurple)
				for (int i = 0; i<4; i++)
					tmpDedoColor[i] = purple[i];
			//drawBorder(0.8);
		}
		setupMaterial(black, tmpDedoColor, white, 100);
		hand2.Draw();
	}
	glPopMatrix();

	//////////

	glPushMatrix();
	glTranslated(slider.slideX, slider.slideY, slider.slideZ);
	if (bWireFrame)
		slider.DrawWireframe();
	else
	{
		if (pickSlider)
		{
			if (changeToBlue)
				for (int i = 0; i<4; i++)
					tmpIcoColor[i] = blue[i];
			else if (changeToGreen)
				for (int i = 0; i<4; i++)
					tmpIcoColor[i] = green[i];
			else if (changeToRed)
				for (int i = 0; i<4; i++)
					tmpIcoColor[i] = red[i];
			else if (changeToPurple)
				for (int i = 0; i<4; i++)
					tmpIcoColor[i] = purple[i];
			//drawBorder(0.8);
		}
		setupMaterial(black, tmpIcoColor, white, 100);
		slider.Draw();
	}
	glPopMatrix();

	///////
}

void drawAllObjects()
{
	drawAxis();
	drawBase();
	drawlCylinderBase();

	drawBody1();
	drawBody2();
	drawBody3();
	drawBody4();

	drawSlider();

	drawHand1();
	drawHand2();

	drawPivot1();
	drawPivot2();
	drawPivot3();
}

void drawAllObjectsWOBorder()
{
	drawAxis();
	drawBase();
	drawlCylinderBase();

	drawBody1();
	drawBody2();
	drawBody3();
	drawBody4();

	drawSlider();

	drawHand1();
	drawHand2();

	drawPivot1();
	drawPivot2();
	drawPivot3();

	drawFourObjectWOBorder();
}

GLfloat light_position1[] = { 6.0f, 6.0f, 6.0f, 0.0f };
GLfloat light_position2[] = { -6.0f, 6.0f, -6.0f, 0.0f };


void DisplayOneView(int nType, int left, int right, int top, int bottom)
{
	mySetupCameraVolume(nType);
	glViewport(left, top, right - left, bottom - top);

	changeCameraPos();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (nType == 1)
		gluLookAt(0, camera_dis, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	else if (nType == 2)
		gluLookAt(0, 0.0, camera_dis, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	else if (nType == 3)
		gluLookAt(camera_dis, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	else
		gluLookAt(camera_X, camera_Y, camera_Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	/*draw the reflection of all objects
	through the floor*/
	glPushMatrix();
	glEnable(GL_NORMALIZE);
	glCullFace(GL_FRONT);
	glScalef(1.0, -1.0, 1.0);

	drawAllObjects();

	glDisable(GL_NORMALIZE);
	glCullFace(GL_BACK);
	glPopMatrix();

	//////////////////////////

	/*draw planar shadow of all object */
	float floorPlane[] = { 0 , 1, 0, 0 };
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glColor3f(0.1, 0.1, 0.1);	// set shadow's color
	glPushMatrix();
	shadowMatrix(floorPlane, light_position1);

	drawAllObjectsWOBorder();

	glPopMatrix();

	if (twoLightSource)	// check if the 2nd light is enabled
	{
		glColor3f(0.1, 0.1, 0.1);	// set shadow's color
		glPushMatrix();
		shadowMatrix(floorPlane, light_position2);
		drawAllObjectsWOBorder();
		glPopMatrix();
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	///////////////////

	/*draw all objects in the read world space*/
	drawFloor();
	drawAllObjects();

	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
}

void mySpecialKey(int theKey, int x, int y) {
	if (theKey == GLUT_KEY_DOWN)
		camera_height -= 0.5;
	if (theKey == GLUT_KEY_UP)
		camera_height += 0.5;
	if (theKey == GLUT_KEY_LEFT)
		camera_angle -= 0.5;
	if (theKey == GLUT_KEY_RIGHT)
		camera_angle += 0.5;
	glutPostRedisplay();
}

void myDisplay()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!b4view)
		DisplayOneView(4, 0, screenWidth, 0, screenHeight);
	else {
		DisplayOneView(1, 0, screenWidth / 2, 0, screenHeight / 2);
		DisplayOneView(2, 0, screenWidth / 2, screenHeight / 2, screenHeight);
		DisplayOneView(3, screenWidth / 2, screenWidth, screenHeight / 2, screenHeight);
		DisplayOneView(4, screenWidth / 2, screenWidth, 0, screenHeight / 2);
	}

	glFlush();
	glutSwapBuffers();
}

void myInit()
{
	float	fHalfSize = 4;
	
	camera_angle = 90;
	camera_dis = 10;
	camera_height = 2;

	loadTextures();

	lookAt_X = 0;
	lookAt_Y = 2;
	lookAt_Z = 0;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 0.0f);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// setup the 2 lighting source
	GLfloat	lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat	lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat	lightAmbient1[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat	lightAmbient2[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_position1[] = { 6.0f, 6.0f, 6.0f, 0.0f };
	GLfloat light_position2[] = { -6.0f, 6.0f, -6.0f, 0.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, 1, 1, 1000);

	glOrtho(-fHalfSize, fHalfSize, -fHalfSize, fHalfSize, -1000, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

int main(int argc, char **argv)
{
	cout << "1, 2: Rotate the base" << endl;
	cout << "3, 4: Rotate the arm" << endl;
	cout << "L, l: Cylinder up" << endl;
	cout << "X, x: Cylinder down" << endl;
	cout << "W, w: Switch between wireframe and solid mode" << endl;
	cout << "V, v: to switch between 1 and 4 views." << endl;
	cout << "A, a: Turn on / off animation" << endl;
	cout << "+   : to increase camera distance." << endl;
	cout << "-   : to decrease camera distance." << endl;
	cout << "up arrow  : to increase camera height." << endl;
	cout << "down arrow: to decrease camera height." << endl;
	cout << "<-        : to rotate camera clockwise." << endl;
	cout << "->        : to rotate camera counterclockwise." << endl;

	glutInit(&argc, (char**)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment 2 - Huynh Trung Hieu (51201076)");

	base.CreateYPlane(10, 10, 10, 10);
	base.slideY = YPlanePos;

	base.CreateCylinder(20, baseHeight, baseRadius);
	base.SetColor(2);
	base.slideY = baseHeight;

	cyl.CreateCylinder(20, cylHeight, cylRadius);
	cyl.SetColor(0);
	cyl.slideY = cylHeight / 2.0;

	body1.CreateCuboid(body1SizeX, body1SizeY, body1SizeZ);
	body1.SetColor(1);

	body2.CreateCuboid(body2SizeX, body2SizeY, body2SizeZ);
	body2.SetColor(3);

	body3.CreateCuboid(body3SizeX, body3SizeY, body3SizeZ);
	body3.SetColor(2);

	body4.CreateCuboid(body3SizeX, body3SizeY, body3SizeZ);
	body4.SetColor(4);

	slider.CreateCuboid(sliderSizeX, sliderSizeY, sliderSizeZ);
	slider.SetColor(8);

	hand1.CreateGeometryShape(hand1R1, hand1R2, hand1Height, hand1Distance);
	hand1.SetColor(9);

	hand2.CreateGeometryShape(hand2R1, hand2R2, hand2Height, hand2Distance);
	hand2.SetColor(10);

	pivot1.CreateCylinder(20, pivot1Height / 2, pivot1SizeX);
	pivot1.SetColor(5);

	pivot2.CreateCylinder(20, pivot2Height / 2, pivot2Radius);
	pivot2.SetColor(5);

	pivot3.CreateCylinder(20, pivot3Height / 2, pivot3Radius);
	pivot3.SetColor(5);

	//base.CreateFloor(1);

	myInit();

	glutSpecialFunc(mySpecialKey);
	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	
	glutMainLoop();
	return 0;
}