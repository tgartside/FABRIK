#ifndef MY_FABRIK_SIMULATOR_H
#define MY_FABRIK_SIMULATOR_H

#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include "BaseSimulator.h"
#include "BaseSystem.h"
#include "Structure.h"
#include "GlobalResourceManager.h"
#include "Hermite.h"

class FABRIKSim : public BaseSimulator
{
public:

	FABRIKSim(const std::string& name, Structure* target, Hermite* hermite);
	~FABRIKSim();

	int step(double time);
	int init(double time)
	{
		m_object->getState(m_pos0);
		setVector(m_vel0, 0, 0, 0);
		return 0;
	};

	void doFabrik(int pIndex, char* direction);
	void clickOrDrag();
	void moveToClick();
	void followSpline();

	double* target;
	double MoE = 0.01;
	char* mode = "";
	int iterations = 0;

	//moveToClick variables
	double xStep = 0;
	double yStep = 0;
	double zStep = 0;
	int numSteps = 0;

	//followSpline variables
	int targetIndex = 0;

	double* initRoot = (double*)malloc(3 * sizeof(double));

	int command(int argc, myCONST_SPEC char** argv);

protected:

	Vector m_pos0; // initial position
	Vector m_vel0; // initial velocity
	Vector m_pos;
	Vector m_vel;

	Structure* m_object;
	Hermite* m_hermite;
};

#endif

