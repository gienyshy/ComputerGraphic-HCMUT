#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
GLfloat offset;
#define DEG2RAD (3.14159f/180.0f)

void mydisplay() {
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.75*cos(DEG2RAD * (offset)), 0.75*sin(DEG2RAD *(offset)));
	glVertex2f(0.75*cos(DEG2RAD * (10 + offset)), 0.75*sin(DEG2RAD * (10 + offset)));
	glVertex2f(0, 0);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(0.75*cos(DEG2RAD * (120 + offset)), 0.75*sin(DEG2RAD *(120 + offset)));
	glVertex2f(0.75*cos(DEG2RAD * (120 + 10 + offset)), 0.75*sin(DEG2RAD * (120 + 10 + offset)));
	glVertex2f(0, 0);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(0.75*cos(DEG2RAD * (240 + offset)), 0.75*sin(DEG2RAD *(240 + offset)));
	glVertex2f(0.75*cos(DEG2RAD * (240 + 10 + offset)), 0.75*sin(DEG2RAD * (240 + 10 + offset)));
	glVertex2f(0, 0);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 36; i++) {
		glVertex2f(0.75*cos(DEG2RAD * 10 * i), 0.75*sin(DEG2RAD * 10 * i));
	}
	glEnd();

	glFlush();
	glutSwapBuffers();
}

void processTimer(int value) {
	offset += (GLfloat)value;
	if (offset > 360) offset = offset - 360.0f;

	glutTimerFunc(15, processTimer, value);
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	offset = 0.0f;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("simple");
	glutDisplayFunc(mydisplay);
	glutTimerFunc(15, processTimer, 10);
	glutMainLoop();
}