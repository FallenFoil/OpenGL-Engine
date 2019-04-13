//
// Created by cesar on 2/25/19.
//

#ifndef CG_MODEL_H
#define CG_MODEL_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>
#include <string>
#include "Ponto.h"


class Model {
    char *filePath;
    int numberOfPoints;
    float red, green, blue;
    GLuint buffer;

public:
    Model();
    Model(char* filePath);
    Model(Model *m);
    char* getFilePath();
    int getNumberOfPoints();
    void setColour(float red, float green, float blue){this->red = red; this->green = green; this->blue = blue;};
    void applyColour();
    GLuint getBuffer();
private:
    void loadPoints();
};


#endif //CG_MODEL_H
