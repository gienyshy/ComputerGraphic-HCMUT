// Lab3.cpp : Defines the entry point for the console application.
//



#include <iostream>
#include <windows.h>
#include <gl.h>
#include <glut.h>
#include <math.h>
#include "supportClass.h"
#include "Mesh.h"

using namespace std;

#define PI		3.1415926

int		screenWidth = 600;
int		screenHeight = 600;

bool	bWireFrame = false;

float	baseRadius = 0.6;
float	baseHeight = 0.15;
float	baseRotateStep = 5;

float	cylRadius = 0.3;
float	cylHeight = 1.5;
float   cylMaxScaleY = 3.0;
float	cylScaleStep = 0.05;
float	cycHeightStandart = 1.5;

float	body1SizeX = 1.6;
float	body1SizeY = 0.3;
float	body1SizeZ = 4.0;



float	body2SizeX = 1.6;
float	body2SizeY = 0.6;
float	body2SizeZ = 1.0;

float	body3SizeX = 0.5;
float	body3SizeY = 0.4;
float	body3SizeZ = 3.0;

float	body4SizeX = 0.5;
float	body4SizeY = 0.4;
float	body4SizeZ = 3.0;

float	chot1Radius = 0.2;
float	chot1Height = 0.1;

float	chot2Radius = 0.1;
float	chot2Height = 0.15;

float	armRotateStep = 5;

float	arm1R1 = 0.15;
float	arm1R2 = 0.3;
float	arm1Height = 0.1;
float	arm1dis = 1.2;

float	arm2R1 = 0.15;
float	arm2R2 = 0.15;
float	arm2Height = 0.1;
float	arm2dis = 2.0;

float	lenSlicer = 0.5;


Mesh	base;
Mesh	cyl;
Mesh	body1;
Mesh	body2;
Mesh	body3;
Mesh	body4;
Mesh	chot1;
Mesh	chot2;
Mesh	chot3;
Mesh	arm1;
Mesh	arm2;
Mesh	slicer;

void drawAxis()
{
	glPushMatrix();

	glColor3f(0, 0, 1);
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
void myKeyboard(unsigned char key, int x, int y)
{
	float	fRInc;
	float	fAngle;
	switch (key)
	{
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
	case '3':
		arm1.rotateY += armRotateStep;
		if (arm1.rotateY > 0)
			arm1.rotateY -= 360;
		break;
	case '4':
		arm1.rotateY -= armRotateStep;
		if (arm1.rotateY < 0)
			arm1.rotateY += 360;
		break;

	case 'l':
	case 'L':
		if (cylHeight <= (cylMaxScaleY - cylScaleStep))
			cylHeight += cylScaleStep;
		break;

	case 'X':
	case 'x':
		if (cylHeight >= cylScaleStep)
			cylHeight -= cylScaleStep;
		break;

	case 'w':
	case 'W':
		bWireFrame = !bWireFrame;
		break;
	}
	glutPostRedisplay();
}

void drawBase()
{
	glPushMatrix();


	glRotatef(base.rotateY, 0, 1, 0);

	if (bWireFrame)
		base.DrawWireframe();
	else
		base.SetColor(1);

	glPopMatrix();
}
void drawCyl()
{
	glPushMatrix();

	glTranslated(0, baseHeight, 0);
	glScaled(1, cylHeight / cycHeightStandart, 1);
	glRotatef(base.rotateY, 0, 1, 0);

	if (bWireFrame)
		cyl.DrawWireframe();
	else
		cyl.SetColor(2);

	glPopMatrix();
}
void drawBody1()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(-body1SizeX / 2.0, cylHeight + baseHeight, -(body2SizeZ + body1SizeZ) / 2.0);

	if (bWireFrame)
		body1.DrawWireframe();
	else
		body1.SetColor(3);

	glPopMatrix();
}
void drawBody2()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(-body1SizeX / 2.0, cylHeight + baseHeight + body1SizeY, -(body2SizeZ + body1SizeZ) / 2.0);

	if (bWireFrame)
		body2.DrawWireframe();
	else
		body2.SetColor(0);

	glPopMatrix();
}

void drawBody3()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(-body1SizeX / 2.0, cylHeight + baseHeight + body1SizeY, (body2SizeZ - body1SizeZ) / 2.0);

	if (bWireFrame)
		body3.DrawWireframe();
	else
		body3.SetColor(4);

	glPopMatrix();
}

void drawBody4()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(body1SizeX / 2.0 - body4SizeX, cylHeight + baseHeight + body1SizeY, (body2SizeZ - body1SizeZ) / 2.0);

	if (bWireFrame)
		body4.DrawWireframe();
	else
		body4.SetColor(5);

	glPopMatrix();
}

void drawchot1()
{
	glPushMatrix();

	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(0, cylHeight + baseHeight + body1SizeY + body2SizeY, -(body2SizeZ + body3SizeZ) / 2.0);
	if (bWireFrame)
		chot1.DrawWireframe();
	else
		chot1.SetColor(6);

	glPopMatrix();
}

void drawchot2()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(0, 0, -(body2SizeZ + body3SizeZ) / 2.0);
	glRotatef(arm1.rotateY, 0, 1, 0);
	glTranslated(0, cylHeight + baseHeight + body1SizeY + body2SizeY + arm1Height, arm1dis);
	if (bWireFrame)
		chot2.DrawWireframe();
	else
		chot2.SetColor(7);

	glPopMatrix();
}


void drawarm1()
{
	glPushMatrix();
	glRotatef(base.rotateY - 90, 0, 1, 0);
	glTranslated(-(body2SizeZ + body3SizeZ) / 2.0, cylHeight + baseHeight + body1SizeY + body2SizeY, 0);
	glRotatef(arm1.rotateY, 0, 1, 0);
	if (bWireFrame)
		arm1.DrawWireframe();
	else
		arm1.SetColor(11);

	glPopMatrix();
}

void drawarm2()
{
	glPushMatrix();
	float alpha = (asin(arm1dis * sin(arm1.rotateY * PI / 180.0) / arm2dis))*180.0 / PI;

	float dis2arm = 0;
	if (arm1.rotateY == 0) dis2arm = arm1dis + arm2dis;
	else if (alpha > -0.5 && alpha < 0.5)dis2arm = arm2dis - arm1dis;
	else dis2arm = arm2dis * sin((alpha + arm1.rotateY)* PI / 180.0) / sin(arm1.rotateY * PI / 180.0);

	glRotatef(base.rotateY - 90, 0, 1, 0);
	glTranslated(dis2arm - (body2SizeZ + body3SizeZ) / 2.0, 0, 0);
	glRotatef(-alpha, 0, 1, 0);
	glTranslated(-arm2dis, cylHeight + baseHeight + body1SizeY + body2SizeY + arm1Height, 0);
	if (bWireFrame)
		arm2.DrawWireframe();
	else
		arm2.SetColor(8);
	glPopMatrix();
}

void drawchot3()
{
	glPushMatrix();
	float alpha = (asin(arm1dis * sin(arm1.rotateY * PI / 180.0) / arm2dis))*180.0 / PI;

	float dis2arm = 0;
	if (arm1.rotateY == 0) dis2arm = arm1dis + arm2dis;
	else if (alpha > -0.5 && alpha < 0.5)dis2arm = arm2dis - arm1dis;
	else dis2arm = arm2dis * sin((alpha + arm1.rotateY)* PI / 180.0) / sin(arm1.rotateY * PI / 180.0);

	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(0, cylHeight + baseHeight + body1SizeY + body2SizeY + arm1Height, dis2arm - (body2SizeZ + body3SizeZ) / 2.0);
	if (bWireFrame)
		chot3.DrawWireframe();
	else
		chot3.SetColor(7);

	glPopMatrix();
}

void drawslicer()
{
	glPushMatrix();
	float alpha = (asin(arm1dis * sin(arm1.rotateY * PI / 180.0) / arm2dis))*180.0 / PI;

	float dis2arm = 0;
	if (arm1.rotateY == 0) dis2arm = arm1dis + arm2dis;
	else if (alpha > -0.5 && alpha < 0.5)dis2arm = arm2dis - arm1dis;
	else dis2arm = arm2dis * sin((alpha + arm1.rotateY)* PI / 180.0) / sin(arm1.rotateY * PI / 180.0);

	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(-(body1SizeX - 2 * body3SizeX) / 2.0, cylHeight + baseHeight + body1SizeY, dis2arm - (body2SizeZ + body3SizeZ) / 2.0 - lenSlicer / 2.0);
	if (bWireFrame)
		slicer.DrawWireframe();
	else
		slicer.SetColor(9);

	glPopMatrix();
}


void myDisplay()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-8, 6, 8, 0, 1, 0, 0, 1, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, screenWidth, screenHeight);

	drawAxis();
	drawBase();
	drawCyl();

	drawBody1();
	drawBody2();
	drawBody3();
	drawBody4();

	drawchot1();
	drawarm1();
	drawchot2();
	drawarm2();
	drawchot3();
	drawslicer();

	glFlush();
	glutSwapBuffers();
}

void myInit()
{
	float	fHalfSize = 4;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 0.0f);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.2*fHalfSize, 1.2*fHalfSize, -1.2*fHalfSize, 1.2*fHalfSize, -1500, 1500);

	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char* argv[])
{
	cout << "1, 2: Rotate the base" << endl;
	cout << "3, 4: Rotate the arm" << endl;
	cout << "L, l: Cylinder up" << endl;
	cout << "X, x: Cylinder down" << endl;
	cout << "W, w: Switch between wireframe and solid mode" << endl;

	glutInit(&argc, (char**)argv); //initialize the tool kit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//set the display mode
	glutInitWindowSize(screenWidth, screenHeight); //set window size
	glutInitWindowPosition(100, 100); // set window position on screen
	glutCreateWindow("Lab3-2015-2016"); // open the screen window

	base.CreateCylinder(20, baseHeight, baseRadius);

	cyl.CreateCylinder(20, cylHeight, cylRadius);

	body1.CreateCuboid(body1SizeX, body1SizeY, body1SizeZ);

	body2.CreateCuboid(body2SizeX, body2SizeY, body2SizeZ);

	body3.CreateCuboid(body3SizeX, body3SizeY, body3SizeZ);

	body4.CreateCuboid(body4SizeX, body4SizeY, body4SizeZ);

	chot1.CreateCylinder(20, chot1Height, chot1Radius);

	arm1.CreateSpecial(arm1Height, arm1R1, arm1R2, arm1dis);

	chot2.CreateCylinder(20, chot2Height, chot2Radius);

	arm2.CreateSpecial(arm2Height, arm2R1, arm2R2, arm2dis);

	chot3.CreateCylinder(20, chot2Height, chot2Radius);

	slicer.CreateCuboid(body1SizeX - 2 * body3SizeX, body2SizeY + arm1Height, lenSlicer);
	slicer.SetColor(4);

	myInit();

	glutKeyboardFunc(myKeyboard);
	glutDisplayFunc(myDisplay);

	glutMainLoop();
	return 0;
}


