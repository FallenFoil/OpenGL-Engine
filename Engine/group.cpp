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


Group::Group() {
    this->numberOfTransformation = 0;
    this->ang = this->axisX = this->axisY = this->axisZ = 0;
    for(int i = 0; i < 3; i++)
        this->priority[i] = -1;
    this->transX = this->transY = this->transZ = 0;
    this->scaleX = this->scaleY = this->scaleZ = 1;
    this->groups;
    this->models;
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
    this->transX = x;
    this->transY = y;
    this->transZ = z;
    this->priority[TRANSLATE] = this->numberOfTransformation++;
}

void Group::setRotate(float ang, float x, float y, float z) {
    this->ang = ang;
    this->axisX = x;
    this->axisY = y;
    this->axisZ = z;
    this->priority[ROTATE] = this->numberOfTransformation++;
}

void Group::setScale(float scaleX, float scaleY, float scaleZ) {
    this->scaleX = scaleX;
    this->scaleY = scaleY;
    this->scaleZ = scaleZ;
    this->priority[SCALE] = this->numberOfTransformation++;
}

int Group::getTransformOrder(int ocurrence){
    if(ocurrence < 3 && ocurrence >= 0){
        int i;
        for(i = 0; i < 3 && this->priority[i] != ocurrence; i++);
        return i;
    }
    else
        return -1;
}

void Group::applyTransformations(){
    for(int i = 0, transform = getTransformOrder(0); transform >= 0; i++, transform = getTransformOrder(i)) {
        if(transform == TRANSLATE) {
            glTranslatef(this->transX, this->transY, this->transZ);
        }
        if(transform == ROTATE){
            glRotatef(this->ang, this->axisX, this->axisY, this->axisZ);
        }
        if(transform == SCALE){
            glScalef(this->scaleX, this->scaleY, this->scaleZ);
        }
    }
}