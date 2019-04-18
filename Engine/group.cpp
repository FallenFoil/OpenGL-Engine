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


void buildRotMatrix(float *x, float *y, float *z, float *m) {

    m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
    m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
    m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void cross(float *a, float *b, float *res) {

    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}

void normalize(float *a) {

    float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}

float length(float *v) {

    float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    return res;

}

void multMatrixVector(float *m, float *v, float *res) {

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }

}


Group::Group() {
    this->numberOfTransformation = 0;
    this->ang = this->axisX = this->axisY = this->axisZ = 0;
    for(int i = 0; i < 3; i++)
        this->priority[i] = -1;
    this->scaleX = this->scaleY = this->scaleZ = 1;
    this->groups;
    this->models;
    this->usingCatmull = this->rotateWithTime = false;
    this->translateDefault;
    this->translateCatMull;
}

Group::Group(Group *g) {
    this->numberOfTransformation = g->numberOfTransformation;
    this->ang = g->ang; this->axisX = g->axisX; this->axisY = g->axisY; this->axisZ = g->axisZ;
    for(int i = 0; i < 3; i++)
        this->priority[i] = g->priority[i];
    this->scaleX = g->scaleX; this->scaleY = g->scaleY; this->scaleZ = g->scaleZ;
    this->groups = g->groups;
    this->models = g->models;
    this->translateDefault = g->translateDefault;
    this->translateCatMull = g->translateCatMull;
    this->usingCatmull = g->usingCatmull;
    this->rotateWithTime = g->rotateWithTime;
}


void Group::setTranslate(float x, float y, float z) {
    if(this->priority[TRANSLATE] >= 0)
        throw EngineException(std::string("Translate already set in a group"));
    this->translateDefault.set(x,y,z);
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
            if(this->usingCatmull)
                translateCatMull.applyTranslate();
            else
                translateDefault.applyTranslate();
        }
        if(transform == ROTATE){
            if(this->rotateWithTime){
                float relativeTime = fmod(glutGet(GLUT_ELAPSED_TIME)/ 1000.0f, ang) / ang;
                glRotatef(360*relativeTime, this->axisX, this->axisY, this->axisZ);
                /*glBegin(GL_LINE_LOOP);
                int pointCount = 100;
                for (int i = 0; i < pointCount; i++) {
                    glVertex3f(cos(), 0, sin());
                }
                glEnd();*/
            }
            else glRotatef(this->ang, this->axisX, this->axisY, this->axisZ);
        }
        if(transform == SCALE){
            glScalef(this->scaleX, this->scaleY, this->scaleZ);
        }
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

void TranslateCatMull::applyTranslate() {
    renderCatmullRomCurve();
    //Transformations
    float pos[4];
    float deriv[4];
    float relativeTime = fmod(glutGet(GLUT_ELAPSED_TIME)/ 1000.0f, transTime) / transTime;
    getGlobalCatmullRomPoint(relativeTime, pos, deriv);

    float* m = (float*) malloc(sizeof(float) * 16);
    float xi[3] = {deriv[0], deriv[1], deriv[2]};
    float zi[3], yi[3];
    float yiOld[3] = {oldYx, oldYy, oldYz};

    normalize(xi);
    normalize(yiOld);
    cross(xi,yiOld, zi);
    normalize(zi);
    cross(zi, xi, yi);
    normalize(yi);

    oldYx=yi[0];
    oldYy=yi[1];
    oldYz=yi[2];

    buildRotMatrix(xi, yi, zi, m);

    glPushMatrix();
    glTranslatef(pos[0],pos[1],pos[2]);
    glMultMatrixf(m);
    //glutWireTeapot(0.1);
    //glutWireCone(0.1, 0.2, 10, 1);

    //glPopMatrix();

    //free(m);

    //glutSwapBuffers();
    //grouptime += 0.001;
}

void TranslateCatMull::getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv) {
    // catmull-rom matrix
    float m[4][4] = {	{-0.5f,  1.5f, -1.5f,  0.5f},
                         { 1.0f, -2.5f,  2.0f, -0.5f},
                         {-0.5f,  0.0f,  0.5f,  0.0f},
                         { 0.0f,  1.0f,  0.0f,  0.0f}};

    // Compute A = M * P
    float pX[] = {p0[0], p1[0], p2[0], p3[0]};
    float pY[] = {p0[1], p1[1], p2[1], p3[1]};
    float pZ[] = {p0[2], p1[2], p2[2], p3[2]};

    float aX[4], aY[4], aZ[4];
    multMatrixVector(*m, pX, aX);
    multMatrixVector(*m, pY, aY);
    multMatrixVector(*m, pZ, aZ);


    // Compute pos = T * A
    float bigT[] = {t*t*t*1.0f, t*t*1.0f, t*1.0f, 1.0f};

    pos[0] = bigT[0]*aX[0] + bigT[1]*aX[1] + bigT[2]*aX[2] + bigT[3]*aX[3];
    pos[1] = bigT[0]*aY[0] + bigT[1]*aY[1] + bigT[2]*aY[2] + bigT[3]*aY[3];
    pos[2] = bigT[0]*aZ[0] + bigT[1]*aZ[1] + bigT[2]*aZ[2] + bigT[3]*aZ[3];

    // compute deriv = T' * A
    float bigTLine[] = {t*t*3.0f, t*2.0f, 1.0f, 0.0f};

    deriv[0] = bigTLine[0]*aX[0] + bigTLine[1]*aX[1] + bigTLine[2]*aX[2] + bigTLine[3]*aX[3];
    deriv[1] = bigTLine[0]*aY[0] + bigTLine[1]*aY[1] + bigTLine[2]*aY[2] + bigTLine[3]*aY[3];
    deriv[2] = bigTLine[0]*aZ[0] + bigTLine[1]*aZ[1] + bigTLine[2]*aZ[2] + bigTLine[3]*aZ[3];

}


// given  global t, returns the point in the curve
void TranslateCatMull::getGlobalCatmullRomPoint(float gt, float *pos, float *deriv){
    int pointsCount =  (int) transPoints.size();
    float t = gt * pointsCount; // this is the real global t
    int index = floor(t);  // which segment
    t = t - index; // where within  the segment

    // indices store the points
    int indices[4];
    indices[0] = (index + pointsCount-1)%pointsCount;
    indices[1] = (indices[0]+1)%pointsCount;
    indices[2] = (indices[1]+1)%pointsCount;
    indices[3] = (indices[2]+1)%pointsCount;

    getCatmullRomPoint(t, transPoints[indices[0]], transPoints[indices[1]], transPoints[indices[2]], transPoints[indices[3]], pos, deriv);
}

void TranslateCatMull::renderCatmullRomCurve() {

// draw curve using line segments with GL_LINE_LOOP
    float pos[4];
    float deriv[4];
    int pointCount = 100;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < pointCount; i++) {
        getGlobalCatmullRomPoint(i * (1.0f) / pointCount, pos, deriv);
        glVertex3f(pos[0], pos[1], pos[2]);

    }
    glEnd();
}
