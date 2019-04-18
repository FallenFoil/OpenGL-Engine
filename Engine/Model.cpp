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
    char drawMode[20];
    int n;
    float x, y, z;

    strcpy(drawMode, "Triangles");
    std::ifstream file;
    file.open(this->filePath);
    file.getline(buffer, 99);
    n = atoi(buffer);
    this->numberOfPoints = n;

    float *vertexB = (float*) malloc(sizeof(float) * 3 * n);
    int vertexBSize = 0;

    for (int i = 0; i < n ;) {
        file.getline(buffer, 99);
        int scan = sscanf(buffer,"%f %f %f", &x, &y, &z);
        if(scan == 0){
            sscanf(buffer,"##%s", drawMode);
        } else {
            vertexB[vertexBSize++] = x;
            vertexB[vertexBSize++] = y;
            vertexB[vertexBSize++] = z;
            i++;
        }
    /*
        if()
        if(strcmp(drawMode, "Triangles") == 0){

        } else if(strcmp(drawMode, "Strips") == 0){

        } else if (strcmp(drawMode, "Fans") == 0){

        }*/
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

void Model::draw(){
    applyColour();

    glBindBuffer(GL_ARRAY_BUFFER,getBuffer());
    glVertexPointer(3,GL_FLOAT,0,0);
    glDrawArrays(GL_TRIANGLES, 0,getNumberOfPoints());

    glColor3f(1,1,1);
}