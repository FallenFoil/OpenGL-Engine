//
// Created by cesar on 3/1/19.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string>
#include <sstream>

#include "Ponto.h"

using namespace std;

Ponto::Ponto(){
    this->x = this->y = this->z = 0;
}

Ponto::Ponto(float fx, float fy, float fz){
    this->x = fx;
    this->y = fy;
    this->z = fz;
}

Ponto::Ponto(Ponto *p){
    this->x = p->x;
    this->y = p->y;
    this->z = p->z;
}

Ponto Ponto::translate(float tx, float ty, float tz){
    return Ponto(this->x + tx, this->y + ty, this->z + tz);
}

float Ponto::getX(){
    return this->x;
}

float Ponto:: getY(){
    return this->y;
}

float Ponto:: getZ(){
    return this->z;
}
/*
void Ponto::draw(){
    glVertex3f(this->x, this->y, this->z);
}
*/

string Ponto::toString(){
    stringstream ss;
    ss << this->x << " " << this->y << " " << this->z;
    return ss.str();
}
/*
void drawTriangle(Ponto p1, Ponto p2, Ponto p3){
    p1.draw();
    p2.draw();
    p3.draw();
}
*/
string toStringTriangle(Ponto p1, Ponto p2, Ponto p3){
    stringstream ss;
    ss << p1.toString() << "\n";
    ss << p2.toString() << "\n";
    ss << p3.toString() << "\n";
    return ss.str();
}
/*
void drawPlane(Ponto p1, Ponto p2, Ponto p3, Ponto p4) {
    p1.draw();
    p2.draw();
    p3.draw();

    p1.draw();
    p3.draw();
    p4.draw();
}*/

string toStringPlane(Ponto p1, Ponto p2, Ponto p3, Ponto p4) {
    stringstream ss;
    ss << p1.toString() << "\n";
    ss << p2.toString() << "\n";
    ss << p3.toString() << "\n";

    ss << p1.toString() << "\n";
    ss << p3.toString() << "\n";
    ss << p4.toString() << "\n";
    return ss.str();
}