#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>

#include "Hermite.h"

int num_points = 0;
double p_entry = 1.0 / (num_points - 1);

double c_points[200][3];
double lookup_table[2][100];
Vector samplePoints[10000];


Hermite::Hermite(const std::string& name) : BaseSystem(name) {
    setSamplePointsDirect();
}


void Hermite::getState(double* p) {

}


void Hermite::setState(double* p) {

}


void Hermite::reset(double time) {

}


/// <summary>
/// This is actually the expanded form for 4 control points (3rd degree), ie a Cubic Bezier
/// not the de Casteljau approach itself (you can do it iteratively or recursively, see below)
/// This evaluates the curve given control points p0, p1, p2, p3 for dimension d (x, y, z) at parameter t in [0,1]
/// </summary>
/// <param name="d">Dimension (0, 1, 2) is (x, y, z) respectively</param>
/// <param name="t">Parameter in [0,1]</param>
/// <returns></returns>
double Hermite::evaluateCurveDirect(int d, double t) {
    return (2 * pow(t,3) - 3 * pow(t,2) + 1) * p0[d] + 
            (-2 * pow(t,3) + 3 * pow(t,2)) * p1[d] + 
            (pow(t,3) - 2 * pow(t,2) + t) * d0[d] + 
            (pow(t,3) - pow(t,2)) * d1[d];
}



/// <summary>
/// This iteratively samples points along the curve by plugging in uniformly sampled t params for x, y, and z
/// </summary>
void Hermite::setSamplePointsDirect() {
    double stepSize = 1.0 / double(numSamples - 1);

    for (int j = 0; j < num_points - 1; j++) {

        setVector(p0, c_points[(j * 2)][0], c_points[(j * 2)][1], c_points[(j * 2)][2]);
        setVector(d0, c_points[(j * 2) + 1][0], c_points[(j * 2) + 1][1], c_points[(j * 2) + 1][2]);
        setVector(p1, c_points[(j * 2) + 2][0], c_points[(j * 2) + 2][1], c_points[(j * 2) + 2][2]);
        setVector(d1, c_points[(j * 2) + 3][0], c_points[(j * 2) + 3][1], c_points[(j * 2) + 3][2]);

        for (int i = 0; i < numSamples; i++) {
            double t = i * stepSize;
            samplePoints[i + j * numSamples][0] = Hermite::evaluateCurveDirect(0, t);
            samplePoints[i + j * numSamples][1] = Hermite::evaluateCurveDirect(1, t);
            samplePoints[i + j * numSamples][2] = Hermite::evaluateCurveDirect(2, t);
        }

        lookup_table[0][j] = p_entry * j;
        lookup_table[1][j+1] = lookup_table[1][j] + Hermite::makeLookupTable(j);

    }
}

double Hermite::makeLookupTable(int j) {
    double total = 0;
    for (int i = 1; i < numSamples; i++) {
        total += sqrt(pow((samplePoints[i+j*numSamples][0] - samplePoints[(i-1)+j* numSamples][0]), 2) +
                      pow((samplePoints[i + j * numSamples][1] - samplePoints[(i - 1) + j * numSamples][1]), 2) +
                      pow((samplePoints[i + j * numSamples][2] - samplePoints[(i - 1) + j * numSamples][2]), 2));
    }

    total += sqrt(pow((c_points[j * 2 + 2][0] - samplePoints[j * numSamples + numSamples-1][0]), 2) +
        pow((c_points[j * 2 + 2][1] - samplePoints[j * numSamples + numSamples - 1][1]), 2) +
        pow((c_points[j * 2 + 2][2] - samplePoints[j * numSamples + numSamples - 1][2]), 2));

    return total;
}





double Hermite::evaluateCurveCatmullRom(int d, double t) {
    return  0.5 * (( - 1 * pow(t, 3) + 2 * pow(t, 2) - t) * p0[d] +
        (3 * pow(t, 3) - 5 * pow(t, 2) + 2) * p1[d] +
        (-3 * pow(t, 3) + 4 * pow(t, 2) + t) * p2[d] +
        (pow(t, 3) - pow(t, 2)) * p3[d]);
}


void Hermite::setSamplePointsCatmullRom() {
    double stepSize = 1.0 / double(numSamples - 1);

    for (int j = 0; j < num_points - 1; j++) {

        if (j < num_points - 2 && j != 0) {

            setVector(p0, c_points[(j * 2) - 2][0], c_points[(j * 2) - 2][1], c_points[(j * 2) - 2][2]);
            setVector(p1, c_points[(j * 2)][0], c_points[(j * 2)][1], c_points[(j * 2)][2]);
            setVector(p2, c_points[(j * 2) + 2][0], c_points[(j * 2) + 2][1], c_points[(j * 2) + 2][2]);
            setVector(p3, c_points[(j * 2) + 4][0], c_points[(j * 2) + 4][1], c_points[(j * 2) + 4][2]);

            for (int i = 0; i < numSamples; i++) {
                double t = i * stepSize;
                samplePoints[i + j * numSamples][0] = evaluateCurveCatmullRom(0, t);
                samplePoints[i + j * numSamples][1] = evaluateCurveCatmullRom(1, t);
                samplePoints[i + j * numSamples][2] = evaluateCurveCatmullRom(2, t);   
            }

            lookup_table[0][j] = p_entry * j;
            if (j == 0) {
                lookup_table[1][j] = 0.00;
            }

            else {
                lookup_table[1][j] = lookup_table[1][j - 1] + makeLookupTable(j);
            }

        }

        else {
            setVector(p0, c_points[(j * 2)][0], c_points[(j * 2)][1], c_points[(j * 2)][2]);
            setVector(d0, c_points[(j * 2) + 1][0], c_points[(j * 2) + 1][1], c_points[(j * 2) + 1][2]);
            setVector(p1, c_points[(j * 2) + 2][0], c_points[(j * 2) + 2][1], c_points[(j * 2) + 2][2]);
            setVector(d1, c_points[(j * 2) + 3][0], c_points[(j * 2) + 3][1], c_points[(j * 2) + 3][2]);

            for (int i = 0; i < numSamples; i++) {
                double t = i * stepSize;
                samplePoints[i + j * numSamples][0] = evaluateCurveDirect(0, t);
                samplePoints[i + j * numSamples][1] = evaluateCurveDirect(1, t);
                samplePoints[i + j * numSamples][2] = evaluateCurveDirect(2, t);
            }

           
            lookup_table[0][j] = p_entry * j;
            if (j == 0) {
                lookup_table[1][j] = 0.00;
            }

            else{
                lookup_table[1][j] = lookup_table[1][j-1] + makeLookupTable(j);
            }

        }
    }
}


int Hermite::command(int argc, myCONST_SPEC char** argv) {
    
    if (argc == 0) {
        animTcl::OutputMessage("No command given for system hermite");
    }
    else {
        if (strcmp(argv[0], "cr") == 0) {
            if (argc > 1) {
                animTcl::OutputMessage("Too many arguments");
            }
            else {
                setSamplePointsCatmullRom();
            }
        }

        else if (strcmp(argv[0], "set") == 0) {
            if (argc != 6) {
                animTcl::OutputMessage("Incorrect number of arguments");
            }
            else {
                if (strcmp(argv[1], "point") == 0) {
                    if (atoi(argv[2]) > num_points - 1) {
                        animTcl::OutputMessage("Point given does not exist");
                    }
                    else {
                        c_points[atoi(argv[2]) * 2][0] = atoi(argv[3]);
                        c_points[atoi(argv[2]) * 2][1] = atoi(argv[4]);
                        c_points[atoi(argv[2]) * 2][2] = atoi(argv[5]);

                        setSamplePointsDirect();
                    }
                }

                else if (strcmp(argv[1], "tangent") == 0) {
                    if (atoi(argv[2]) > num_points - 1) {
                        animTcl::OutputMessage("Tangent given does not exist");
                    }
                    else {
                        c_points[(atoi(argv[2]) * 2) + 1][0] = atoi(argv[3]);
                        c_points[(atoi(argv[2]) * 2) + 1][1] = atoi(argv[4]);
                        c_points[(atoi(argv[2]) * 2) + 1][2] = atoi(argv[5]);

                        setSamplePointsDirect();
                    }
                }
                else {
                    animTcl::OutputMessage("Command after set not recognized");
                }
            }
        }

        else if (strcmp(argv[0], "add") == 0) {
            if (argc != 8) {
                animTcl::OutputMessage("Incorrect number of arguments");
            }
            else {
                if (strcmp(argv[1], "point") == 0) {
                    c_points[num_points * 2][0] = atoi(argv[2]);
                    c_points[num_points * 2][1] = atoi(argv[3]);
                    c_points[num_points * 2][2] = atoi(argv[4]);

                    c_points[(num_points * 2) + 1][0] = atoi(argv[5]);
                    c_points[(num_points * 2) + 1][1] = atoi(argv[6]);
                    c_points[(num_points * 2) + 1][2] = atoi(argv[7]);

                    num_points++;

                    setSamplePointsDirect();
                }

                else {
                    animTcl::OutputMessage("Command after add not recognized");
                }
            }
        }

        else if (strcmp(argv[0], "getArcLength") == 0) {
            if (argc != 2) {
                animTcl::OutputMessage("Incorrect number of arguments");
            }
            else {

                double u = atof(argv[1]);
                int i = (int)(u / (p_entry));

                if (u == 1.0) {
                    animTcl::OutputMessage("arclength = %.3f", lookup_table[1][num_points - 1]);
                }

                else {
                    double result = lookup_table[1][i] +
                        ((u - lookup_table[0][i]) / (lookup_table[0][i + 1] - lookup_table[0][i])) *
                        (lookup_table[1][i + 1] - lookup_table[1][i]);

                    animTcl::OutputMessage("arclength = %.3f", result);

                }
            }
        }

        else if (strcmp(argv[0], "load") == 0) {
            if (argc != 2) {
                animTcl::OutputMessage("Incorrect number of arguments");
            }
            else {
                Hermite::loadSpline(argv[1]);
            }
        }

        else if (strcmp(argv[0], "export") == 0) {
            if (argc != 2) {
                animTcl::OutputMessage("Incorrect number of arguments");
            }
            else {
                std::ofstream myfile(argv[1]);

                myfile << "hermite " << num_points << "\n";
                for (int i = 0; i < num_points; i++) {
                    myfile << c_points[i * 2][0] << " " << c_points[i * 2][1] << " " << c_points[i * 2][2] << " "
                        << c_points[i * 2 + 1][0] << " " << c_points[i * 2 + 1][1] << " " << c_points[i * 2 + 1][2] << "\n";
                }

                myfile.close();
            }
        }

        glutPostRedisplay();
        return TCL_OK;

    }
}

void Hermite::loadSpline(char* filename) {
    std::ifstream myfile(filename);

    if (myfile.fail()) {
        animTcl::OutputMessage("file does not exist");
        return;
    }

    std::string line;
    std::getline(myfile, line);

    char* cline = new char[line.length() + 1];
    strcpy(cline, line.c_str());

    char* token = strtok(cline, " ");
    token = strtok(NULL, " ");

    num_points = atoi(token);

    int index = 0;
    while (std::getline(myfile, line)) {
        char* cline = new char[line.length() + 1];
        strcpy(cline, line.c_str());

        c_points[index * 2][0] = atof(strtok(cline, " "));
        c_points[index * 2][1] = atof(strtok(NULL, " "));
        c_points[index * 2][2] = 0;
        /*c_points[index * 2 + 1][0] = atof(strtok(NULL, " "));
        c_points[index * 2 + 1][1] = atof(strtok(NULL, " "));
        c_points[index * 2 + 1][2] = atof(strtok(NULL, " "));*/
        
        index++;
    }

    myfile.close();
    Hermite::setSamplePointsCatmullRom();
}


/// <summary>
/// Displays a single control point given the point and radius
/// </summary>
/// <param name="p">point</param>
/// <param name="r">radius</param>
void Hermite::displayControlPoint(Vector p, float r) {
    glPointSize(r);
    glBegin(GL_POINTS);
    glVertex3dv(p);
    glEnd();
}


/// <summary>
/// Display all of the sample points given their radius (points stored in the class, see Bezier.h)
/// </summary>
/// <param name="r">The radius of the point rendering</param>
void Hermite::displaySamplePoints(float r) {
    glPointSize(r);

    glBegin(GL_POINTS);
    for (int i = 0; i < (num_points - 1) * numSamples; i++)
        glVertex3dv(samplePoints[i]);

    glEnd();
}


/// <summary>
/// Render the curve as a polyline approximation (ie draw lines between the sample points)
/// </summary>
void Hermite::displaySampledCurve(float r) {
    glLineWidth(r);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < (num_points - 1) * numSamples; i++)
        glVertex3f(samplePoints[i][0], samplePoints[i][1], samplePoints[i][2]);
    glEnd();
}


void Hermite::display(GLenum mode) {
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glEnable(GL_COLOR_MATERIAL);
    glPushMatrix();

    /*glRotated(15, 1, 0, 0);
    glRotated(45, 0, 1, 0);
    glScalef(0.4, 0.4, 0.4);*/

    // Draw the control points
 /*   glColor3f(1, 0, 0);
    for (int j = 0; j < num_points; j++) {
        setVector(p0, c_points[(j * 2)][0], c_points[(j * 2)][1], c_points[(j * 2)][2]);
        displayControlPoint(p0, 10.0);
    }*/


    // Draw the intermediate samples along the curve
   /* glColor3f(0, 0, 0);
    displaySamplePoints(3.0);*/

    // Draw the lines in between the samples
    glColor3f(1.0, 0, 0);
    glRotated(15, 1, 0, 0);
    displaySampledCurve(1.5);

    glPopAttrib();
    glPopMatrix();
}
