//
// Created by cesar on 3/6/19.
//

#ifndef XML_GRAPHICS_GROUP_H
#define XML_GRAPHICS_GROUP_H

#include <vector>
#include "Model.h"
#include "EngineException.h"


class Translate{
public:
    virtual void applyTranslate() = 0;
};

class TranslateDefault : public Translate{
    float transX, transY, transZ;
public:
    TranslateDefault(){transX =0; transY = 0; transZ = 0;};
    TranslateDefault(float x, float y, float z){this->transX = x; this->transY = y; this->transZ = z;};
    void applyTranslate() override {glTranslatef(transX,transY,transZ);};
};

class TranslateCatMull : public Translate{
    float transTime;
    std::vector<float* > transPoints;
    float oldYx, oldYy, oldYz;
public:
    TranslateCatMull(){oldYx = 0; oldYy = 1; oldYz = 0; transTime = 0; transPoints;}
    void renderCatmullRomCurve();
    void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv);
    void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv);
    void applyTranslate() override;
    void setTransTime(float time){this->transTime = time;};
    void addPointToTranslation(float x, float y, float z){
        float * point =  (float*)malloc(sizeof(float) * 3);
        point[0] = x;
        point[1] = y;
        point[2] = z;
        this->transPoints.push_back(point);
    };
};


class Group {
    int numberOfTransformation;
    bool usingCatmull, rotateWithTime;
    float ang, axisX, axisY, axisZ;
    float scaleX, scaleY, scaleZ;
    int priority[3];
    TranslateDefault translateDefault;
    TranslateCatMull translateCatMull;
    std::vector<Model> models;
    std::vector<Group> groups;

public:
    Group();
    Group(Group *p);
    std::vector<Model> getModels(){return this->models;};
    std::vector<Group> getGroups(){return this->groups;};
    void setCatMull(bool catMull){this->usingCatmull = catMull;};
    void setRotateWithTime(bool rotateWithTime){this->rotateWithTime = rotateWithTime;};
    void addGroup(Group *p){this->groups.push_back(std::move(p));};
    void addModel(Model m){this->models.push_back(m);};
    void setTranslate(float x, float y, float z);
    void setRotate(float ang, float x, float y, float z);
    void setScale(float scaleX, float scaleY, float scaleZ);
    void applyTransformations();
    void setTransTime(float time){this->translateCatMull.setTransTime(time);};
    void addPointToTranslation(float x, float y, float z){this->translateCatMull.addPointToTranslation(x,y,z);};
private:
    int getTransformOrder(int ocurrence);
};

#endif //XML_GRAPHICS_GROUP_H
