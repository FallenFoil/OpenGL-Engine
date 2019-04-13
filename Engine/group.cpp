//
// Created by cesar on 3/6/19.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "group.h"
#include "Model.h"
#include "EngineException.h"

#define TRANSLATE 0
#define ROTATE 1
#define SCALE 2

Group::Group() {
    this->numberOfTransformation = 0;
    this->ang = this->axisX = this->axisY = this->axisZ = 0;
    for(int i = 0; i < 3; i++)
        this->priority[i] = -1;
    this->transX = this->transY = this->transZ = this->transTime = 0;
    this->scaleX = this->scaleY = this->scaleZ = 1;
    this->groups;
    this->models;
    this->usingCatmull = this->rotateWithTime = false;
    this->transPoints;
}

void Group::addPointToTranslation(float x, float y, float z){
    this->transPoints.push_back(x);
    this->transPoints.push_back(y);
    this->transPoints.push_back(z);
}

Group::Group(Group *g) {
    this->numberOfTransformation = g->numberOfTransformation;
    this->ang = g->ang; this->axisX = g->axisX; this->axisY = g->axisY; this->axisZ = g->axisZ;
    for(int i = 0; i < 3; i++)
        this->priority[i] = g->priority[i];
    this->transX = g->transX; this->transY = g->transY; this->transZ = g->transZ;
    this->scaleX = g->scaleX; this->scaleY = g->scaleY; this->scaleZ = g->scaleZ;
    this->groups = g->groups;
    this->models = g->models;
}


void Group::setTranslate(float x, float y, float z) {
    if(this->priority[TRANSLATE] >= 0)
        throw EngineException(std::string("Translate already set in a group"));
    this->transX = x;
    this->transY = y;
    this->transZ = z;
    this->priority[TRANSLATE] = this->numberOfTransformation++;
}

void Group::setRotate(float ang, float x, float y, float z) {
    if(this->priority[ROTATE] >= 0)
        throw EngineException(std::string("Rotate already set in a group"));
    this->ang = ang;
    this->axisX = x;
    this->axisY = y;
    this->axisZ = z;
    this->priority[ROTATE] = this->numberOfTransformation++;
}

void Group::setScale(float scaleX, float scaleY, float scaleZ) {
    if(this->priority[SCALE] >= 0)
        throw EngineException(std::string("Scale already set in a group"));
    this->scaleX = scaleX;
    this->scaleY = scaleY;
    this->scaleZ = scaleZ;
    this->priority[SCALE] = this->numberOfTransformation++;
}


void Group::getTransform(float* x, float* y, float *z){
    *x = this->transX;
    *y = this->transY;
    *z = this->transZ;
}

int Group::getTransformOrder(int ocurrence){
    if(ocurrence < 3 && ocurrence >= 0){
        int i;
        for(i = 0; i < 3 && this->priority[i] != ocurrence; i++);
        if(i == 3)
            return -1;
        return i;
    }
    else
        return -1;
}

void Group::applyTransformations(){
    for(int i = 0, transform = getTransformOrder(0); transform >= 0; i++, transform = getTransformOrder(i)) {
        if(transform == TRANSLATE) {
            if(usingCatmull) throw EngineException("Not implemented yet");
            else glTranslatef(this->transX, this->transY, this->transZ);
        }
        if(transform == ROTATE){
            if(rotateWithTime) throw EngineException("Not implemented yet");
            else glRotatef(this->ang, this->axisX, this->axisY, this->axisZ);
        }
        if(transform == SCALE){
            glScalef(this->scaleX, this->scaleY, this->scaleZ);
        }
    }
}