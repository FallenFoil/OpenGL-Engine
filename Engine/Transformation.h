//
// Created by cesar on 5/2/19.
//

#ifndef ENGINE_TRANSFORMATIONS_H
#define ENGINE_TRANSFORMATIONS_H

#include <vector>
#include "Model.h"
#include "EngineException.h"

class Transformation{
public:
    virtual void applyTransformation() = 0;
};

class RotateDefault: public Transformation {
    float ang, axisX, axisY, axisZ;
public:
    RotateDefault(float ang, float x, float y, float z){this->ang = ang; this->axisX = x; this->axisY = y; this->axisZ = z;};
    void applyTransformation() override {glRotatef(this->ang, this->axisX, this->axisY, this->axisZ);};
};

class RotateWithTime: public Transformation {
    float time, axisX, axisY, axisZ;
public:
    RotateWithTime(float time, float x, float y, float z){this->time = time; this->axisX = x; this->axisY = y; this->axisZ = z;};
    void applyTransformation() override;
};

class Scale : public Transformation {
    float scaleX, scaleY, scaleZ;
public:
    explicit Scale(Scale *s){scaleX = s->scaleX; scaleY = s->scaleY; scaleZ = s->scaleZ;};
    Scale(float x, float y, float z){ scaleX = x; scaleY = y; scaleZ = z;};
    void applyTransformation() override {glScalef(scaleX, scaleY, scaleZ);};
};



class TranslateDefault : public Transformation{
    float transX, transY, transZ;
public:
    TranslateDefault(){transX =0; transY = 0; transZ = 0;};
    TranslateDefault(float x, float y, float z){this->transX = x; this->transY = y; this->transZ = z;};
    void applyTransformation() override {glTranslatef(transX,transY,transZ);};
};

class TranslateCatMull : public Transformation{
    float transTime;
    std::vector<float* > transPoints;
    float oldYx, oldYy, oldYz;
public:
    TranslateCatMull(){oldYx = 0; oldYy = 1; oldYz = 0; transTime = 0;}
    void renderCatmullRomCurve();
    void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv);
    void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv);
    void applyTransformation() override;
    void setTransTime(float time){this->transTime = time;};
    void addPointToTranslation(float x, float y, float z){
        float * point =  (float*)malloc(sizeof(float) * 3);
        point[0] = x;
        point[1] = y;
        point[2] = z;
        this->transPoints.push_back(point);
    };
};


#endif //ENGINE_TRANSFORMATIONS_H
