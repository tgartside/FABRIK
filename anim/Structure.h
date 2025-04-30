#ifndef MY_STRUCTURE_H
#define MY_STRUCTURE_H

#include "BaseSystem.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>

#include "shared/opengl.h"

class Structure : public BaseSystem
{

public:
	Structure(const std::string& name);
	virtual void getState(double* p);
	virtual void setState(double* p);
	void reset(double time);

	void display(GLenum mode = GL_RENDER);

	void readModel(char* fname) { m_model.ReadOBJ(fname); }
	void flipNormals(void) { glmReverseWinding(&m_model); }
	int command(int argc, myCONST_SPEC char** argv);

	double** points;
	int numPoints = 8;

	double* lengths;
	double totalLength = 0;

protected:

	float m_sx;
	float m_sy;
	float m_sz;

	Vector m_pos;

	GLMmodel m_model;

};

#endif

