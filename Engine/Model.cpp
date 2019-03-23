//
// Created by cesar on 2/25/19.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Model.h"
#include <cstring>
#include <fstream>
#include "Ponto.h"

Model::Model(char* filepath)  {
    this->filePath = (char*) malloc(strlen(filepath) + 1);
    std::strcpy(this->filePath, filepath);
    loadPoints();
}

Model::Model(Model *m){
    this->filePath = m->getFilePath();
    this->pontos = m->getPoints();
    this->red = m->red;
    this->blue = m->blue;
    this->green = m->green;
}

Model::Model() {
    this->red = this->blue = this->green = 1;
    this->pontos;
    this->filePath = (char*) malloc(1);
    std::strcpy(this->filePath, "\0");
}

char* Model::getFilePath() {
    return this->filePath;
}

void Model::loadPoints(){
    char buffer[100];
    int n;
    float x, y, z;
    std::ifstream file;
    file.open(this->filePath);
    file.getline(buffer, 99);
    n = atoi(buffer);
    Ponto p;
    this->numberOfPoints = n;
    this->pontos = (Ponto*) malloc(sizeof(Ponto) * n);
    for (int i = 0; i < n ; ++i) {
        file.getline(buffer, 99);
        sscanf(buffer,"%f %f %f", &x, &y, &z);
        p = Ponto(x,y,z);
        this->pontos[i] = p;
    }
    file.close();
}

Ponto* Model::getPoints(){
    return this->pontos;
}

int Model::getNumberOfPoints(){
    return this->numberOfPoints;
}

Ponto Model::getPoint(int i) {
    return this->pontos[i];
}

void Model::applyColour() {
    glColor3f(this->red, this->green, this->blue);
}