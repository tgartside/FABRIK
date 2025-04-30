#include "Structure.h"

Structure::Structure(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f)
{

	setVector(m_pos, 0, 0, 0);

	lengths = (double*)malloc((numPoints - 1) * sizeof(double));

	points = (double**)malloc(numPoints * (sizeof(double*) * 3));
	for (int i = 0; i < numPoints; i++) {
		points[i] = (double*)malloc(sizeof(double) * 9);

		points[i][0] = 0;
		points[i][1] = 2 * i;
		points[i][2] = 0;
	}

}	// Structure

void Structure::getState(double* s) {

	VecCopy(s, m_pos);

}	// Structre::getState

void Structure::setState(double* s) {

	VecCopy(m_pos, s);

}	// Structre::setState

void Structure::reset(double time)
{

	setVector(m_pos, 0, 0, 0);

}	// Structure::Reset

int Structure::command(int argc, myCONST_SPEC char** argv) {
	if (argc < 1) {
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}

	else if (strcmp(argv[0], "point") == 0) {
		if (argc != 5) {
			animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		}

		else {

			int index = atoi(argv[1]);
			points[index][0] = atof(argv[2]);
			points[index][1] = atof(argv[3]);
			points[index][2] = atof(argv[4]);

			if (index > 0) {
				lengths[index - 1] = sqrt(pow((points[index][0] - points[index - 1][0]), 2) +
										pow((points[index][1] - points[index - 1][1]), 2) +
										pow((points[index][2] - points[index - 1][2]), 2));
	
				totalLength += lengths[index - 1];
			}
		}
	}

	glutPostRedisplay();
	return TCL_OK;
}	// Structure::command

void Structure::display(GLenum mode)
{

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glTranslated(0, 0, 0);
	glRotated(15, 1, 0, 0);
	glScalef(m_sx, m_sy, m_sz);


	//Draw plane
	glColor3f(1, 1, 1);

	glBegin(GL_POLYGON);
		glVertex3f(-10, 0, -10);
		glVertex3f(-10, 0, 10);
		glVertex3f(10, 0, 10);
		glVertex3f(10, 0, -10);
	glEnd();

	glPopMatrix();
	glPopAttrib();


	//Draw points
	glColor3f(1, 0, 1);
	for (int i = 0; i < numPoints; i++) {
		glEnable(GL_COLOR_MATERIAL);
		//glEnable(GL_LIGHTING);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glRotated(15, 1, 0, 0);
		glTranslated(points[i][0], points[i][1], points[i][2]);
		glScalef(m_sx, m_sy, m_sz);
		glutSolidSphere(0.2, 20, 20);
		glPopMatrix();
		glPopAttrib();
	}

	//Draw line segments
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glTranslated(0, 0, 0);
	glColor3f(0, 1, 0);
	glRotated(15, 1, 0, 0);
	for (int i = 0; i < numPoints - 1; i++) {
		GLdrawCylinder(0.05, points[i], points[i + 1]);
	}

	glPopMatrix();
	glPopAttrib();

	/*glTranslatef(0, 0, 0);
	glutSolidSphere(0.2, 20, 20);

	glTranslatef(0, 2, 0);
	glutSolidSphere(0.2, 20, 20);

	glTranslatef(0, 2, 0);
	glutSolidSphere(0.2, 20, 20);

	glTranslatef(0, 2, 0);
	glutSolidSphere(0.2, 20, 20);


	glPopMatrix();*/
	//glPopAttrib();

}	// Structure::display