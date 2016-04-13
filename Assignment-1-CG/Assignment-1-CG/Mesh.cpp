#include "stdafx.h"
#include "Mesh.h"
#include <math.h>

#define PI			3.1415926
#define	COLORNUM		14


float	ColorArr[COLORNUM][3] = { { 1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 0.0,  0.0, 1.0 },
{ 1.0, 1.0,  0.0 },{ 1.0, 0.0, 1.0 },{ 0.0, 1.0, 1.0 },
{ 0.3, 0.3, 0.3 },{ 0.5, 0.5, 0.5 },{ 0.9,  0.9, 0.9 },
{ 1.0, 0.5,  0.5 },{ 0.5, 1.0, 0.5 },{ 0.5, 0.5, 1.0 },
{ 0.0, 0.0, 0.0 },{ 1.0, 1.0, 1.0 } };


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
}

void Mesh::CreateCuboid(float fSizeX, float fSizeY, float fSizeZ) {
	int i;
	numVerts = 8;
	pt = new Point3[numVerts];
	pt[0].set(-fSizeX / 2, -fSizeY / 2, -fSizeZ / 2);
	pt[1].set(-fSizeX / 2, fSizeY / 2, -fSizeZ / 2);
	pt[2].set(-fSizeX / 2, fSizeY / 2, fSizeZ / 2);
	pt[3].set(-fSizeX / 2, -fSizeY / 2, fSizeZ / 2);
	pt[4].set(fSizeX / 2, -fSizeY / 2, -fSizeZ / 2);
	pt[5].set(fSizeX / 2, fSizeY / 2, -fSizeZ / 2);
	pt[6].set(fSizeX / 2, fSizeY / 2, fSizeZ / 2);
	pt[7].set(fSizeX / 2, -fSizeY / 2, fSizeZ / 2);


	numFaces = 6;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 0;
	face[0].vert[1].vertIndex = 1;
	face[0].vert[2].vertIndex = 2;
	face[0].vert[3].vertIndex = 3;
	for (i = 0; i<face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 4;
	face[1].vert[1].vertIndex = 5;
	face[1].vert[2].vertIndex = 6;
	face[1].vert[3].vertIndex = 7;
	for (i = 0; i<face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 1;
	face[2].vert[1].vertIndex = 5;
	face[2].vert[2].vertIndex = 6;
	face[2].vert[3].vertIndex = 2;
	for (i = 0; i<face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 0;
	face[3].vert[1].vertIndex = 4;
	face[3].vert[2].vertIndex = 7;
	face[3].vert[3].vertIndex = 3;
	for (i = 0; i<face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 2;
	face[4].vert[1].vertIndex = 6;
	face[4].vert[2].vertIndex = 7;
	face[4].vert[3].vertIndex = 3;
	for (i = 0; i<face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 1;
	face[5].vert[1].vertIndex = 5;
	face[5].vert[2].vertIndex = 4;
	face[5].vert[3].vertIndex = 0;
	for (i = 0; i<face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;
}


void Mesh::CreateCylinder(int nSegment, float fHeight, float fRadius) {
	int i;

	numVerts = nSegment * 2 + 2;
	pt = new Point3[numVerts];

	float pi = 3.141592;
	float alpha = 2 * pi / nSegment;

	for (i = 0; i < nSegment; i++) {
		float angle = alpha * i;
		float x = fRadius * cos(angle);
		float z = fRadius * sin(angle);
		float y = -fHeight / 2;
		pt[i].set(x, y, z);

	}

	for (i = 0; i < nSegment; i++) {
		float angle = alpha * i;
		float x = fRadius * cos(angle);
		float z = fRadius * sin(angle);
		float y = fHeight / 2;
		pt[i + nSegment].set(x, y, z);
	}


	pt[nSegment * 2].set(0, -fHeight / 2, 0);

	pt[nSegment * 2 + 1].set(0, fHeight / 2, 0);

	numFaces = nSegment * 3;
	face = new Face[numFaces];


	for (i = 0; i < nSegment; i++) {
		face[i].nVerts = 3;
		face[i].vert = new VertexID[3];
		face[i].vert[0].vertIndex = nSegment * 2;
		face[i].vert[1].vertIndex = i;
		face[i].vert[2].vertIndex = (i + 1) % (nSegment);
	}


	for (i = 0; i < nSegment; i++) {
		face[i + nSegment].nVerts = 3;
		face[i + nSegment].vert = new VertexID[3];
		face[i + nSegment].vert[0].vertIndex = nSegment * 2 + 1;
		face[i + nSegment].vert[1].vertIndex = i + nSegment;
		face[i + nSegment].vert[2].vertIndex = nSegment + (i + nSegment + 1) % (nSegment);
	}


	for (int i = 0; i < nSegment; i++) {
		face[i + 2 * nSegment].nVerts = 4;
		face[i + 2 * nSegment].vert = new VertexID[4];
		face[i + 2 * nSegment].vert[0].vertIndex = i;
		face[i + 2 * nSegment].vert[1].vertIndex = (i + 1) % (nSegment);
		face[i + 2 * nSegment].vert[2].vertIndex = nSegment + (i + nSegment + 1) % (nSegment);
		face[i + 2 * nSegment].vert[3].vertIndex = (i + nSegment);
	}

}

void Mesh::CreateSphere(int nSlice, int nStack, float radius) {
	numVerts = nSlice * nStack + 2;
	pt = new Point3[numVerts];

	float pi = 3.141592;
	float alpha = 2 * pi / nSlice;
	float beta = pi / nStack;
	for (int i = 0; i < nStack - 1; i++) {
		float theta = (i + 1) * beta; // Góc theta nằm giư
		for (int j = 0; j < nSlice; j++) {
			float phi = j * alpha;
			float x = radius * cos(phi) * sin(theta); // r là bán kính hình tròn.
			float y = radius * sin(phi) * sin(theta); // 
			float z = radius * cos(theta);
			pt[i * nSlice + j].set(x, y, z);
		}
	}
	pt[nSlice * nStack].set(0, 0, radius);
	pt[nSlice * nStack + 1].set(0, 0, -radius);

	numFaces = nSlice * nStack;
	face = new Face[numFaces];

	for (int i = 0; i < nSlice; i++) {
		face[i].nVerts = 3;
		face[i].vert = new VertexID[3];
		face[i].vert[0].vertIndex = nSlice * nStack;
		face[i].vert[1].vertIndex = i;
		face[i].vert[2].vertIndex = (i + 1) % nSlice;
	}
	for (int i = 0; i < nStack - 2; i++) {
		for (int j = 0; j < nSlice; j++) {
			face[(i + 1) * nSlice + j].nVerts = 4;
			face[(i + 1) * nSlice + j].vert = new VertexID[4];
			face[(i + 1) * nSlice + j].vert[0].vertIndex = i * nSlice + j;
			face[(i + 1) * nSlice + j].vert[1].vertIndex = (i + 1) * nSlice + j;
			face[(i + 1) * nSlice + j].vert[2].vertIndex = (i + 1) * nSlice + (j + 1) % nSlice;
			face[(i + 1) * nSlice + j].vert[3].vertIndex = i * nSlice + (j + 1) % nSlice;
		}
	}
	for (int i = 0; i < nSlice; i++) {
		face[nSlice * (nStack - 1) + i].nVerts = 3;
		face[nSlice * (nStack - 1) + i].vert = new VertexID[3];
		face[nSlice * (nStack - 1) + i].vert[0].vertIndex = nSlice * nStack + 1;
		face[nSlice * (nStack - 1) + i].vert[1].vertIndex = (nSlice)* (nStack - 2) + i;
		face[nSlice * (nStack - 1) + i].vert[2].vertIndex = (nSlice)* (nStack - 2) + (i + 1) % nSlice;
	}

}

void Mesh::twocicleblock(float r1, float r2, float d, float thickNess) {
	int n = 30;
	numVerts = 4 * (n + 1);
	pt = new Point3[numVerts];

	float pi = 3.141592;
	float alpha = acos((r1 - r2) / d);
	if (r1 > r2) {
		alpha = pi - alpha;
	}
	float step = 2 * alpha / n;
	for (int i = 0; i <= n; i++) {
		float angle = (pi - alpha) + i * step;
		float x = r1 * cos(angle) - d / 2;
		float z = r1 * sin(angle);
		pt[i].set(x, -thickNess / 2, z);
		pt[i + (n + 1)].set(x, thickNess / 2, z);
	}
	step = 2 * (pi - alpha) / n;
	for (int i = 0; i <= n; i++) {
		float angle = (pi + alpha) + i * step;
		float x = r2 * cos(angle) + d / 2;
		float z = r2 * sin(angle);
		pt[i + 2 * (n + 1)].set(x, -thickNess / 2, z);
		pt[i + 3 * (n + 1)].set(x, thickNess / 2, z);
	}

	numFaces = 2 * n + 4;
	face = new Face[numFaces];
	for (int i = 0; i < n; i++) {
		face[i].nVerts = 4;
		face[i].vert = new VertexID[4];
		face[i].vert[0].vertIndex = i;
		face[i].vert[1].vertIndex = i + 1;
		face[i].vert[2].vertIndex = i + 1 + (n + 1);
		face[i].vert[3].vertIndex = i + (n + 1);
	}
	for (int i = 0; i < n; i++) {
		face[i + n].nVerts = 4;
		face[i + n].vert = new VertexID[4];
		face[i + n].vert[0].vertIndex = i + 2 * (n + 1);
		face[i + n].vert[1].vertIndex = i + 1 + 2 * (n + 1);
		face[i + n].vert[2].vertIndex = i + 1 + 3 * (n + 1);
		face[i + n].vert[3].vertIndex = i + 3 * (n + 1);
	}

	face[2 * n].nVerts = 4;
	face[2 * n].vert = new VertexID[4];
	face[2 * n].vert[0].vertIndex = 0;
	face[2 * n].vert[1].vertIndex = n + 1;
	face[2 * n].vert[2].vertIndex = 4 * (n + 1) - 1;
	face[2 * n].vert[3].vertIndex = 3 * (n + 1) - 1;

	face[2 * n + 1].nVerts = 4;
	face[2 * n + 1].vert = new VertexID[4];
	face[2 * n + 1].vert[0].vertIndex = n;
	face[2 * n + 1].vert[1].vertIndex = 2 * (n + 1) - 1;
	face[2 * n + 1].vert[2].vertIndex = 3 * (n + 1);
	face[2 * n + 1].vert[3].vertIndex = 2 * (n + 1);

	face[2 * n + 2].nVerts = 2 * (n + 1);
	face[2 * n + 2].vert = new VertexID[2 * (n + 1)];
	for (int i = 0; i < n + 1; i++) {
		face[2 * n + 2].vert[i].vertIndex = i;
	}
	for (int i = 0; i < n + 1; i++) {
		face[2 * n + 2].vert[i + (n + 1)].vertIndex = i + 2 * (n + 1);
	}

	face[2 * n + 3].nVerts = 2 * (n + 1);
	face[2 * n + 3].vert = new VertexID[2 * (n + 1)];
	for (int i = 0; i < n + 1; i++) {
		face[2 * n + 3].vert[i].vertIndex = i + (n + 1);
	}
	for (int i = 0; i < n + 1; i++) {
		face[2 * n + 3].vert[i + (n + 1)].vertIndex = i + 3 * (n + 1);
	}
}

void Mesh::SetColor(int colorIdx)
{
	for (int f = 0; f < numFaces; f++) {
		for (int v = 0; v < face[f].nVerts; v++) {
			face[f].vert[v].colorIndex = colorIdx;
		}
	}
}

void Mesh::CreateSpecial(float Height, float R1, float R2, float Distance)
{
}

void Mesh::CreateBonus(float R, float r)
{
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

			ic = f % COLORNUM;

			glColor3f(ColorArr[ic][0], ColorArr[ic][1], ColorArr[ic][2]);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}



