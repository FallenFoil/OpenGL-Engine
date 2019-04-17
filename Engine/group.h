//
// Created by cesar on 3/6/19.
//

#ifndef XML_GRAPHICS_GROUP_H
#define XML_GRAPHICS_GROUP_H

#include <vector>
#include "Model.h"
#include "EngineException.h"

/*
class Translate{
public:
    void applyTranslate(){};
};

class TranslateDefault : public Translate{
    float transX, transY, transZ;
public:
    void applyTranslate(){glTranslatef(transX,transY,transZ);};
    TranslateDefault(float x, float y, float z){this->transX = x; this->transY = y; this->transZ = z;};
};

class TranslateCatMull : public Translate{
    float transTime;
    std::vector<float> transPoints;
public:
    void applyTranslate(){throw EngineException("Not implemented yet");};
};*/


class Group {
    int numberOfTransformation;
    bool usingCatmull, rotateWithTime;
    float oldYx, oldYy, oldYz;
    float transX, transY, transZ, transTime;
    float ang, axisX, axisY, axisZ;
    float scaleX, scaleY, scaleZ;
    int priority[3];
    std::vector<float*> transPoints;
    std::vector<Model> models;
    std::vector<Group> groups;

public:
    Group();
    Group(Group *p);
    std::vector<Model> getModels(){return this->models;};
    std::vector<Group> getGroups(){return this->groups;};
    void setCatMull(bool catMull){this->usingCatmull = catMull;};
    void setRotateWithTime(bool rotateWithTime){this->rotateWithTime = rotateWithTime;};
    void setTransTime(float time){this->transTime = time;};
    void addPointToTranslation(float x, float y, float z);
    void addGroup(Group *p){this->groups.push_back(std::move(p));};
    void addModel(Model m){this->models.push_back(m);};
    void setTranslate(float x, float y, float z);
    void setRotate(float ang, float x, float y, float z);
    void setScale(float scaleX, float scaleY, float scaleZ);
    void applyTransformations();
    void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv);
    void renderCatmullRomCurve();
    void getTransform(float* x, float* y, float *z);

private:
    int getTransformOrder(int ocurrence);
};

#endif //XML_GRAPHICS_GROUP_H
