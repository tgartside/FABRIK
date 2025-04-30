#include "FABRIKSim.h"

double winX = 0;
double winY = 0;
double winZ = 0;

BallData* arcball = NULL;
int ball_width = 500;
int ball_height = 500;
float ball_zoom = 1;
int ball_prevY = 0;


FABRIKSim::FABRIKSim(const std::string& name, Structure* target, Hermite* hermite) :
	BaseSimulator(name),
	m_object(target),
	m_hermite(hermite)
{


}	// FABRIKSim

FABRIKSim::~FABRIKSim()
{
}	// FABRIKSim::~FABRIKSim

void mousePress(int button, int state, int x, int y) {

	GlobalResourceManager::use()->setMouseButtonInfo(button, state);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		winX = (float)x;

		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		winY = (float)viewport[3] - (float)y;
	}

	//if( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
	//{
	//	HVect arcball_coords;
	//	arcball_coords.x = 2.0*(float)x/(float)ball_width-1.0;
	//	arcball_coords.y = -2.0*(float)y/(float)ball_height+1.0;
	//	Ball_Mouse(arcball, arcball_coords) ;
	//	Ball_Update(arcball);
	//	Ball_BeginDrag(arcball);

	//}

	//if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	//{
	//	Ball_EndDrag(arcball);

	//	//		Button = -1 ;

	//	GlobalResourceManager::use()->setMouseButtonInfo(-1, state);

	//}

}

void mouseMotion(int x, int y) {

	GLMouseButtonInfo mouseButtonInfo;

	mouseButtonInfo = GlobalResourceManager::use()->getMouseButtonInfo();

	if (mouseButtonInfo.button == GLUT_LEFT_BUTTON){
		winX = (float)x;

		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		winY = (float)viewport[3] - (float)y;
	}

	/*else if (mouseButtonInfo.button == GLUT_RIGHT_BUTTON)
	{
		HVect arcball_coords;
		arcball_coords.x = 2.0 * (float)x / (float)ball_width - 1.0;
		arcball_coords.y = -2.0 * (float)y / (float)ball_height + 1.0;
		Ball_Mouse(arcball, arcball_coords);
		Ball_Update(arcball);
		glutPostRedisplay();
	}*/
}

void FABRIKSim::doFabrik(int pIndex, char* direction) {
	
	if (strcmp(direction, "f") == 0) {
		if (pIndex == m_object->numPoints - 1) {
			m_object->points[pIndex][0] = target[0];
			m_object->points[pIndex][1] = target[1];
			m_object->points[pIndex][2] = target[2];
			doFabrik(pIndex - 1, direction);
		}

		else {
			/*double ri = abs(sqrt(pow((m_object->points[pIndex+1][0] - m_object->points[pIndex][0]), 2) +
									pow((m_object->points[pIndex + 1][0] - m_object->points[pIndex][0]), 2) +
									pow((m_object->points[pIndex + 1][0] - m_object->points[pIndex][0]), 2)));*/

			double ri = sqrt(pow((m_object->points[pIndex + 1][0] - m_object->points[pIndex][0]), 2) +
				pow((m_object->points[pIndex + 1][1] - m_object->points[pIndex][1]), 2) +
				pow((m_object->points[pIndex + 1][2] - m_object->points[pIndex][2]), 2));

			double li = m_object->lengths[pIndex] / ri;

			m_object->points[pIndex][0] = ((1 - li) * m_object->points[pIndex+1][0]) + (li * m_object->points[pIndex][0]);
			m_object->points[pIndex][1] = ((1 - li) * m_object->points[pIndex+1][1]) + (li * m_object->points[pIndex][1]);
			m_object->points[pIndex][2] = ((1 - li) * m_object->points[pIndex+1][2]) + (li * m_object->points[pIndex][2]);

			if (pIndex == 0) {
				doFabrik(pIndex, "b");
			}

			else {
				doFabrik(pIndex - 1, direction);
			}
		}
	}

	else if (strcmp(direction, "b") == 0) {
		if (pIndex == 0) {
			m_object->points[pIndex][0] = initRoot[0];
			m_object->points[pIndex][1] = initRoot[1];
			m_object->points[pIndex][2] = initRoot[2];
				
		}
	
		/*double ri = abs(sqrt(pow((m_object->points[pIndex + 1][0] - m_object->points[pIndex][0]), 2) +
			pow((m_object->points[pIndex + 1][0] - m_object->points[pIndex][0]), 2) +
			pow((m_object->points[pIndex + 1][0] - m_object->points[pIndex][0]), 2)));*/

		double ri = sqrt(pow((m_object->points[pIndex + 1][0] - m_object->points[pIndex][0]), 2) +
			pow((m_object->points[pIndex + 1][1] - m_object->points[pIndex][1]), 2) +
			pow((m_object->points[pIndex + 1][2] - m_object->points[pIndex][2]), 2));

		double li = m_object->lengths[pIndex] / ri;

		m_object->points[pIndex+1][0] = ((1 - li) * m_object->points[pIndex][0]) + (li * m_object->points[pIndex+1][0]);
		m_object->points[pIndex+1][1] = ((1 - li) * m_object->points[pIndex][1]) + (li * m_object->points[pIndex+1][1]);
		m_object->points[pIndex+1][2] = ((1 - li) * m_object->points[pIndex][2]) + (li * m_object->points[pIndex+1][2]);

		if (pIndex == m_object->numPoints - 2) {
			/*double diff = abs(sqrt(pow((target[0] - m_object->points[m_object->numPoints - 1][0]), 2) +
				pow((target[1] - m_object->points[m_object->numPoints - 1][1]), 2) +
				pow((target[2] - m_object->points[m_object->numPoints - 1][2]), 2)));*/

			double diff = sqrt(pow((m_object->points[m_object->numPoints - 1][0] - target[0]), 2) +
				pow((m_object->points[m_object->numPoints - 1][1] - target[1]), 2) +
				pow((m_object->points[m_object->numPoints - 1][2] - target[2]), 2));

			if (diff > MoE) {
				//iterations++;
				doFabrik(pIndex + 1, "f");
			}

			else {
				//iterations++;
				return;
			}
		}
		else {
			doFabrik(pIndex + 1, direction);
		}
	}
	

}	// FABRIKSim:: doFabrik

void FABRIKSim::clickOrDrag() {
	double model[16];
	double proj[16];
	int view[4];

	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	glGetIntegerv(GL_VIEWPORT, view);

	double objX = 0;
	double objY = 0;
	double objZ = 0;

	gluUnProject(winX, winY, winZ, model, proj, view, &objX, &objY, &objZ);

	target[0] = objX;
	target[1] = objY;
	target[2] = 0;

	numSteps = 0;
}

void FABRIKSim::moveToClick() {

	if (numSteps == 0) {
		xStep = (target[0] - m_object->points[m_object->numPoints - 1][0]) / 20;
		yStep = (target[1] - m_object->points[m_object->numPoints - 1][1]) / 20;
		zStep = (target[2] - m_object->points[m_object->numPoints - 1][2]) / 20;
	}

	if (numSteps < 20) {
		target[0] = m_object->points[m_object->numPoints - 1][0] + xStep;
		target[1] = m_object->points[m_object->numPoints - 1][1] + yStep;
		target[2] = m_object->points[m_object->numPoints - 1][2] + zStep;
		//numSteps++;
	}

}

void FABRIKSim::followSpline() {

	if (targetIndex == 0 || targetIndex == (num_points - 1) * 20) {
		
		targetIndex = 0;

		if (numSteps < 20) {

			if (numSteps == 0) {
				xStep = (c_points[0][0] - m_object->points[m_object->numPoints - 1][0]) / 20;
				yStep = (c_points[0][1] - m_object->points[m_object->numPoints - 1][1]) / 20;
				zStep = (c_points[0][2] - m_object->points[m_object->numPoints - 1][2]) / 20;
			}

			target[0] = m_object->points[m_object->numPoints - 1][0] + xStep;
			target[1] = m_object->points[m_object->numPoints - 1][1] + yStep;
			target[2] = m_object->points[m_object->numPoints - 1][2] + zStep;

			numSteps++;
			return;
		}

		targetIndex++;
		numSteps = 0;

	}

	target[0] = samplePoints[targetIndex][0];
	target[1] = samplePoints[targetIndex][1];
	target[2] = samplePoints[targetIndex][2];

	targetIndex++;

}

int FABRIKSim::step(double time) {

	//iterations = 0;

	if (strcmp(mode, "click") == 0 || strcmp(mode, "drag") == 0) {
		clickOrDrag();

	/*	if (strcmp(mode, "click") == 0) {
			moveToClick();
		}*/
	}

	else if (strcmp(mode, "spline") == 0) {
		followSpline();
	}
	

	double dist = sqrt(pow((target[0] - m_object->points[0][0]), 2) + 
						pow((target[1] - m_object->points[0][1]), 2) + 
						pow((target[2] - m_object->points[0][2]), 2));

	//target is out of reach
	/*if (abs(dist) >= m_object->totalLength) {*/
	if (dist > m_object->totalLength) {
		for (int i = 0; i < m_object->numPoints - 1; i++) {
			/*double ri = abs(sqrt(pow((target[0] - m_object->points[i][0]), 2) +
							pow((target[1] - m_object->points[i][1]), 2) +
							pow((target[2] - m_object->points[i][2]), 2)));*/

			double ri = sqrt(pow((target[0] - m_object->points[i][0]), 2) +
				pow((target[1] - m_object->points[i][1]), 2) +
				pow((target[2] - m_object->points[i][2]), 2));

			double li = m_object->lengths[i] / ri;

			m_object->points[i + 1][0] = ((1 - li) * m_object->points[i][0]) + (li * target[0]);
			m_object->points[i + 1][1] = ((1 - li) * m_object->points[i][1]) + (li * target[1]);
			m_object->points[i + 1][2] = ((1 - li) * m_object->points[i][2]) + (li * target[2]);
		}
	}

	//target is in reach
	else {
		initRoot[0] = m_object->points[0][0];
		initRoot[1] = m_object->points[0][1];
		initRoot[2] = m_object->points[0][2];
		

		doFabrik(m_object->numPoints - 1, "f");

		//animTcl::OutputMessage("Iterations: %d", iterations);
	}

	return 0;
}	// FABRIKSim::step

int FABRIKSim::command(int argc, myCONST_SPEC char** argv) {
	
	if (argc < 1) {
		animTcl::OutputMessage("simulator %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}

	else if (strcmp(argv[0], "target") == 0) {
		if (argc != 4) {
			animTcl::OutputMessage("simulator %s: wrong number of params.", m_name.c_str());
		}

		else {
			target = (double*)malloc(3 * sizeof(double));
			target[0] = atof(argv[1]);
			target[1] = atof(argv[2]);
			target[2] = atof(argv[3]);
		}
	}

	else if (strcmp(argv[0], "mode") == 0) {
		if (argc != 2) {
			animTcl::OutputMessage("simulator %s: wrong number of params.", m_name.c_str());
		}

		else {

			/*glutMotionFunc(NULL);
			glutMouseFunc(NULL);*/

			if (strcmp(argv[1], "click") == 0) {
				mode = "click";
				glutMouseFunc(mousePress);
			}

			else if (strcmp(argv[1], "drag") == 0) {
				mode = "drag";
				glutMotionFunc(mouseMotion);
			}

			else if (strcmp(argv[1], "command") == 0) {
				mode = "command";
			}

			else if (strcmp(argv[1], "spline") == 0) {
				mode = "spline";
			}

			else {
				animTcl::OutputMessage("simulator %s: invalid mode.", m_name.c_str());
			}
		}

	}

	else if (strcmp(argv[0], "read") == 0) {
		if (argc != 2) {
			animTcl::OutputMessage("simulator %s: wrong number of params.", m_name.c_str());
		}

		else {
			m_hermite->loadSpline(argv[1]);
		}
	}

	glutPostRedisplay();
	return TCL_OK;

}	// FABRIKSim::command