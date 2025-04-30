#ifndef MY_HERMITE_H
#define MY_HERMITE_H

#include "BaseSystem.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>
#include <iostream>
#include <fstream>

#include "shared/opengl.h"

extern int num_points;
extern double c_points[200][3];
extern Vector samplePoints[10000];

class Hermite : public BaseSystem
{

public:
	Hermite(const std::string& name);
	virtual void getState(double* p);
	virtual void setState(double* p);
	void reset(double time);

	// The direct method evaluate the full function
	double evaluateCurveDirect(int d, double t);
	void setSamplePointsDirect();
	double makeLookupTable(int j);

	double Lerp(double A, double B, double t);

	//Catmull-Rom stuff
	void setSamplePointsCatmullRom();
	double evaluateCurveCatmullRom(int d, double t);

	void displayControlPoint(Vector p, float r);
	void displaySamplePoints(float r);
	void displaySampledCurve(float r);
	void display(GLenum mode = GL_RENDER);

	int command(int argc, myCONST_SPEC char** argv);
	void loadSpline(char* filename);

protected:
	int numSamples = 20;
	Vector p0, p1, p2, p3, d0, d1;
};
#endif
