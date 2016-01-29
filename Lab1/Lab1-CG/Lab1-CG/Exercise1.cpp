#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
GLfloat offset;
#define DEG2RAD (3.14159f/180.0f)

void drawHalfCircle(double numberA, int numberB) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(5);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < numberB; i++) {
		glVertex2f(numberA*cos(DEG2RAD * 1 * i), numberA*sin(DEG2RAD * 1 * i));
	}
	glEnd();
}

void mydisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	for (double j = 0; j < 0.7; j = j + 0.05) {
		drawHalfCircle(j, 181);
	}
	
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("simple");
	glutDisplayFunc(mydisplay);
	glutTimerFunc(50, processTimer, 10);
	glutMainLoop();
}