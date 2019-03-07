//
// Created by cesar on 2/25/19.
//

#ifndef CG_MODEL_H
#define CG_MODEL_H

#include <vector>
#include <string>
#include "Ponto.h"

class Model {
    //// Tornar o tamanho do buffer variavel mais tarde ////
    char *filePath;
    Ponto* pontos;
    int numberOfPoints;
public:
    Model(char* filePath);
    Model();
    Model(Model *m);
    Ponto* getPoints();
    Ponto getPoint(int i);
    char* getFilePath();
    int getNumberOfPoints();
private:
    void loadPoints();
};


#endif //CG_MODEL_H
