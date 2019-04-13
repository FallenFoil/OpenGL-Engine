//
// Created by cesar on 2/25/19.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstring>
#include <fstream>
#include <stdlib.h>
#include "Model.h"
#include "Ponto.h"

Model::Model() {
    this->red = this->blue = this->green = 1;
    this->filePath = (char*) malloc(1);
    std::strcpy(this->filePath, "\0");
}

Model::Model(char* filepath)  {
    this->filePath = (char*) malloc(strlen(filepath) + 1);
    std::strcpy(this->filePath, filepath);
    loadPoints();
}

Model::Model(Model *m){
    this->filePath = m->getFilePath();
    this->red = m->red;
    this->blue = m->blue;
    this->green = m->green;
}

char* Model::getFilePath() {
    return this->filePath;
}

int Model::getNumberOfPoints(){
    return this->numberOfPoints;
}

void Model::loadPoints(){
    char buffer[100];
    int n;
    float x, y, z;
    std::ifstream file;
    file.open(this->filePath);
    file.getline(buffer, 99);
    n = atoi(buffer);
    this->numberOfPoints = n;

    float *vertexB = (float*) malloc(sizeof(float) * 3 * n);
    int vertexBSize = 0;

    for (int i = 0; i < n ; ++i) {
        file.getline(buffer, 99);
        sscanf(buffer,"%f %f %f", &x, &y, &z);
        vertexB[vertexBSize++] = x;
        vertexB[vertexBSize++] = y;
        vertexB[vertexBSize++] = z;
    }
    file.close();

    glGenBuffers(1, &this->buffer);
    glBindBuffer(GL_ARRAY_BUFFER,this->buffer);
    glBufferData(GL_ARRAY_BUFFER,vertexBSize * sizeof(float), vertexB, GL_STATIC_DRAW);

    free(vertexB);
}

GLuint Model::getBuffer(){
    return this->buffer;
}

void Model::applyColour() {
    glColor3f(this->red, this->green, this->blue);
}