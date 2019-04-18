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

#include <unordered_map>
#include <vector>
#include <string>
#include <utility>
#include <string>
#include <map>
#include "Ponto.h"

#define DEFAULT 0
#define STRIPS 1
#define FANS 2

class Model {
    char *filePath;
    int numberOfPoints;
    float red, green, blue;
    GLuint buffer;
    //std::vector<std::pair<int, int>> drawMode;

public:
    //static std::unordered_map<std::string, Model*> modelsLoaded;

    Model();
    Model(char* filePath);
    Model(Model *m);
    char* getFilePath();
    int getNumberOfPoints();
    void setColour(float red, float green, float blue){this->red = red; this->green = green; this->blue = blue;};
    void applyColour();
    GLuint getBuffer();
    void draw();
private:
    void loadPoints();
};

#endif //CG_MODEL_H
