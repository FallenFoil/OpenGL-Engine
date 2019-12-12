//
// Created by cesar on 3/1/19.
//

#ifndef CLASS3_PONTO_H
#define CLASS3_PONTO_H

class Ponto {
    float x;
    float y;
    float z;
public:
    Ponto translate(float tx, float ty, float tz);
    Ponto(float x, float y, float z);
    Ponto(Ponto *p);
    Ponto();
    //void draw();
    std::string toString();
    float getX();
    float getY();
    float getZ();
};


// void drawTriangle(Ponto p1, Ponto p2, Ponto p3);
// drawPlane(Ponto p1, Ponto p2, Ponto p3, Ponto p4);
std::string toStringTriangle(Ponto p1, Ponto p2, Ponto p3);
std::string toStringPlane(Ponto p1, Ponto p2, Ponto p3, Ponto p4);

#endif //CLASS3_PONTO_H
