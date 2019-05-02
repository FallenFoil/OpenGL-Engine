//
// Created by cesar on 3/6/19.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <vector>

#include "group.h"
#include "Model.h"
#include "EngineException.h"

#define TRANSLATE 0
#define ROTATE 1
#define SCALE 2

Group::Group() {
    this->numberOfTransformation = 0;
    for(int i = 0; i < 3; i++)
        this->priority[i] = -1;
    Scale s(1,1,1);
    TranslateDefault t(0,0,0);
    scale = &s;
    translate = &t;
}

Group::Group(Group *g) {
    this->numberOfTransformation = g->numberOfTransformation;
    this->rotate = g->rotate;
    for(int i = 0; i < 3; i++)
        this->priority[i] = g->priority[i];
    this->scale = g->scale;
    this->groups = g->groups;
    this->models = g->models;
    this->translate = g->translate;
}


void Group::setTranslate(Transformation* t) {
    if(this->priority[TRANSLATE] >= 0)
        throw EngineException(std::string("Translate already set in a group"));
    this->translate = t;
    this->priority[TRANSLATE] = this->numberOfTransformation++;
}

void Group::setRotate(Transformation *t) {
    if(this->priority[ROTATE] >= 0)
        throw EngineException(std::string("Rotate already set in a group"));
    this->rotate = t;
    this->priority[ROTATE] = this->numberOfTransformation++;
}

void Group::setScale(Transformation *scale) {
    if(this->priority[SCALE] >= 0)
        throw EngineException(std::string("Scale already set in a group"));
    this->scale = scale;
    this->priority[SCALE] = this->numberOfTransformation++;
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
        if(transform == TRANSLATE)
            translate->applyTransformation();
        if(transform == ROTATE)
            rotate->applyTransformation();
        if(transform == SCALE)
            scale->applyTransformation();
    }
}


void Group::draw(){
    glPushMatrix();
    applyTransformations();
    std::vector<Model> models = getModels();

    for(auto model = models.begin(); model != models.end(); model++) {
        Model m = *model;
        m.draw();
    }

    std::vector<Group> childGroups = getGroups();
    for(int i = 0; i < childGroups.size(); i++){
        Group g = childGroups[i];
        g.draw();
    }
    glPopMatrix();
}
