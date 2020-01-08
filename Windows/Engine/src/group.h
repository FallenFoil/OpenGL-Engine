//
// Created by cesar on 3/6/19.
//

#ifndef XML_GRAPHICS_GROUP_H
#define XML_GRAPHICS_GROUP_H

#include <vector>
#include <string.h>
#include "Model.h"
#include "EngineException.h"
#include "Transformation.h"


class Group {
    float x;
    float y;
    float z;
    int numberOfTransformation;
    int priority[3];
    Transformation *rotate;
    Transformation *scale;
    Transformation *translate;
    std::vector<Model> models;
    std::vector<Group> groups;

public:
    Group();
    Group(Group *p);
    std::vector<Model> getModels(){return this->models;};
    std::vector<Group> getGroups(){return this->groups;};
    void draw();
    void addGroup(Group *p){this->groups.push_back(std::move(p));};
    void addModel(Model m){this->models.push_back(m);};
    void setTranslate(Transformation *t);
    void setRotate(Transformation *t);
    void setScale(Transformation *t);
    void applyTransformations();
private:
    int getTransformOrder(int ocurrence);
};

#endif //XML_GRAPHICS_GROUP_H
