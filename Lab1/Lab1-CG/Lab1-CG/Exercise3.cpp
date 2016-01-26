#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#define DEG2RAD (3.14159f/180.0f)

void mydisplay() {
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 36; i++) {
		glVertex2f(0.75*cos(DEG2RAD * 10 * i), 0.75*sin(DEG2RAD * 10 * i));
		i++;
		glVertex2f(0.75*cos(DEG2RAD * 10 * i), 0.75*sin(DEG2RAD * 10 * i));
		glVertex2f(0, 0);
	}
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex2f(0, 0);
	for (int i = 0; i < 36; i++) {
		glVertex2f(0.75*cos(DEG2RAD * 10 * i), 0.75*sin(DEG2RAD * 10 * i));
	}
	glEnd();

	glFlush();
	glutSwapBuffers();

}

int main(int argc, char** argv) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("simple");
	glutDisplayFunc(mydisplay);
	glutMainLoop();
}
