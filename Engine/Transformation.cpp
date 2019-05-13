//
// Created by cesar on 5/2/19.
//

#include "Transformation.h"

#include <math.h>



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


void TranslateCatMull::applyTransformation(){
    if(this->draw){
        renderCatmullRomCurve();
    }
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

    //glPushMatrix();
    glTranslatef(pos[0],pos[1],pos[2]);
    glMultMatrixf(m);

    //glPopMatrix();

    free(m);

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

void RotateWithTime::applyTransformation() {
    float relativeTime = fmod(glutGet(GLUT_ELAPSED_TIME)/ 1000.0f, time) / time;
    glRotatef(360*relativeTime, this->axisX, this->axisY, this->axisZ);
}