//
// Created by cesar on 2/25/19.
//

#ifndef CG_MODEL_H
#define CG_MODEL_H

#include <vector>
#include <string>
#include "Ponto.h"

class Model {
    char *filePath;
    Ponto* pontos;
    int numberOfPoints;
    float red, green, blue;
public:
    Model(char* filePath);
    Model();
    Model(Model *m);
    Ponto* getPoints();
    Ponto getPoint(int i);
    char* getFilePath();
    int getNumberOfPoints();
    void setColour(float red, float green, float blue){this->red = red; this->green = green; this->blue = blue;};
    void applyColour();
private:
    void loadPoints();
};


#endif //CG_MODEL_H
