//
// Created by cesar on 3/6/19.
//

#ifndef XML_GRAPHICS_GROUP_H
#define XML_GRAPHICS_GROUP_H

#include <vector>
#include "Model.h"

#define TRANSLATE 0
#define ROTATE 1
#define SCALE 2

class Group {
    int numberOfTransformation;
    float transX, transY, transZ;
    float ang, axisX, axisY, axisZ;
    float scaleX, scaleY, scaleZ;
    int priority[3];
    std::vector<Model> models;
    std::vector<Group> groups;

public:
    Group();
    Group(Group *p);
    std::vector<Model> getModels(){return this->models;};
    std::vector<Group> getGroups(){return this->groups;};
    void addGroup(Group *p){this->groups.push_back(std::move(p));};
    void addModel(Model m){this->models.push_back(m);};
    void setTranslate(float x, float y, float z);
    void setRotate(float ang, float x, float y, float z);
    void setScale(float scaleX, float scaleY, float scaleZ);
    void applyTransformations();

private:
    int getTransformOrder(int ocurrence);
};


#endif //XML_GRAPHICS_GROUP_H
