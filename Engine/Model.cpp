#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <unordered_map>
#include <IL/il.h>
#include <cstring>
#include <fstream>
#include <stdlib.h>
#include <string>
#include "Model.h"
#include "Ponto.h"

//std::unordered_map<std::string, Model*> Model::modelsLoaded;


Model::Model() {
    this->red = this->blue = this->green = 1;
    this->filePath = (char*) malloc(1);
    std::strcpy(this->filePath, "\0");
    texture = 0;
    texturePath = new std::string("");
}

Model::Model(char* filepath)  {
    this->filePath = (char*) malloc(strlen(filepath) + 1);
    std::strcpy(this->filePath, filepath);
    loadPoints();
    this->red = this->blue = this->green = -1;
    dColor = nullptr;
    sColor = nullptr;
    eColor = nullptr;
    aColor = nullptr;
}

Model::Model(Model *m){
    this->filePath = m->getFilePath();
    this->red = m->red;
    this->blue = m->blue;
    this->green = m->green;
    this->texture = m->texture;
    this->texturePath = m->texturePath;
    this->dColor = m->dColor;
    this->sColor = m->sColor;
    this->eColor = m->eColor;
    this->aColor = m->aColor;
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
    std::vector<float> normal, texCoord;
    int vertexBSize = 0;

    for (int i = 0; i < n ; i++) {
        file.getline(buffer, 99);
        sscanf(buffer,"%f %f %f", &x, &y, &z);
        vertexB[vertexBSize++] = x;
        vertexB[vertexBSize++] = y;
        vertexB[vertexBSize++] = z;

        file.getline(buffer, 99);
        sscanf(buffer,"%f %f %f", &x, &y, &z);
        normal.push_back(x);
        normal.push_back(y);
        normal.push_back(z);

        file.getline(buffer, 99);
        sscanf(buffer,"%f %f", &x, &y);
        texCoord.push_back(x);
        texCoord.push_back(y);
    }

    file.close();

    glGenBuffers(3, this->buffer);

    glBindBuffer(GL_ARRAY_BUFFER,this->buffer[0]);
    glBufferData(GL_ARRAY_BUFFER,vertexBSize * sizeof(float), vertexB, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, this->buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(float), &(normal[0]), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, this->buffer[2]);
    glBufferData(GL_ARRAY_BUFFER, texCoord.size() * sizeof(float), &(texCoord[0]), GL_STATIC_DRAW);

    free(vertexB);
}
#include <iostream>
void Model::applyColour() {
    if(red >= 0 && green >= 0 && blue >= 0){
        glColor3f(this->red, this->green, this->blue);
    }
}

void Model::loadTexture() {

    unsigned int t, tw, th;
    unsigned char *texData;
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring)texturePath->c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
}

void Model::setTexture(std::string *texture){
    this->texturePath = texture;
    if(!this->texturePath->empty()){
        loadTexture();
    }
}

void Model::draw(){
    applyColour();
    if(dColor != nullptr) dColor->applyColor();
    if(sColor != nullptr) sColor->applyColor();
    if(eColor != nullptr) eColor->applyColor();
    if(aColor != nullptr) aColor->applyColor();

    if(texturePath != nullptr && !texturePath->empty()){
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glVertexPointer(3,GL_FLOAT,0,0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, getNumberOfPoints());

    if(texturePath != nullptr && !texturePath->empty()){
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glColor3f(1,1,1);
}