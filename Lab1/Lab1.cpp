#include "stdafx.h"
#include <windows.h>
#include <gl.h>
#include <glut.h>
#include <math.h>
#include <stdio.h>
GLfloat angle;
#define DEG2RAD (3.14159f/180.0f)
#define PI  3.14159

void mydisplay(){
	glClear(GL_COLOR_BUFFER_BIT);
	GLfloat x = 0,y = 0,R = 0.8;
		
	glColor3f(1.0,1.0,0.0);
	glBegin(GL_LINE_LOOP);
		for(int i = 0; i < 36;i++)
			glVertex2f(R*cos(2*PI*i/36) + x,R*sin(2*PI*i/36) +y);		
	glEnd();

	GLfloat cx, cy;
	cx = 0.8*cos(DEG2RAD*angle); cy = 0.8*sin(DEG2RAD*angle);
	glBegin(GL_POLYGON);
		glVertex2f(cx - 0.05, cy - 0.05);
		glVertex2f(cx - 0.05, cy + 0.05);
		glVertex2f(cx + 0.05, cy + 0.05);
		glVertex2f(cx + 0.05, cy - 0.05);
	glEnd();

	glFlush();
	glutSwapBuffers();
}
void processTimer(int value){
	angle += (GLfloat)value;
	if(angle > 360) angle = angle - 360.0f;
	glutTimerFunc(100, processTimer, 10);
	glutPostRedisplay();
}
int main(int argc, char** argv){
	angle = 0.0f;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Lab1");
	glutDisplayFunc(mydisplay);
	glutTimerFunc(100, processTimer, 10);
	glutMainLoop();
}