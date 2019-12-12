//
// Created by cesar on 2/25/19.
//

#ifndef CG_MODEL_H
#define CG_MODEL_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <unordered_map>
#include <vector>
#include <string>
#include <utility>
#include <string>
#include <map>

class Color {
    virtual void applyColor() = 0;
protected:
    float colors[4];
};

class DiffuseColor : public Color {
public:
    DiffuseColor(){ colors[0] = 0.8; colors[1] = 0.8; colors[2] = 0.8; colors[3] = 1.0; };
    DiffuseColor(float r, float g, float b){colors[0] = r; colors[1] = g; colors[2] = b;colors[3]=1.0f;};
    void applyColor() override {glMaterialfv(GL_FRONT, GL_DIFFUSE, colors);}
};

class SpecularColor : public Color {
public:
    SpecularColor(){ colors[0] = 0.0; colors[1] = 0.0; colors[2] = 0.0; colors[3] = 1.0; };
    SpecularColor(float r, float g, float b){colors[0] = r; colors[1] = g; colors[2] = b;colors[3]=1.0f;};
    void applyColor() override {glMaterialfv(GL_FRONT, GL_SPECULAR, colors);}
};

class EmissiveColor : public Color {
public:
    EmissiveColor(){ colors[0] = 0.0; colors[1] = 0.0; colors[2] = 0.0; colors[3] = 1.0; };
    EmissiveColor(float r, float g, float b){colors[0] = r; colors[1] = g; colors[2] = b;colors[3]=1.0f;};
    void applyColor() override {glMaterialfv(GL_FRONT, GL_EMISSION, colors);}
};

class AmbientColor : public Color {
public:
    AmbientColor(){ colors[0] = 0.2; colors[1] = 0.2; colors[2] = 0.2; colors[3] = 1.0; };
    AmbientColor(float r, float g, float b){colors[0] = r; colors[1] = g; colors[2] = b;colors[3]=1.0f;};
    void applyColor() override {glMaterialfv(GL_FRONT, GL_AMBIENT, colors);}
};

class Model {
    char *filePath;
    int numberOfPoints;
    float red, green, blue;
    std::string *texturePath;
    unsigned int texture;
    GLuint buffer[3];
    DiffuseColor *dColor;
    SpecularColor *sColor;
    EmissiveColor *eColor;
    AmbientColor *aColor;

public:
    //static std::unordered_map<std::string, Model*> modelsLoaded;

    Model();
    Model(char* filePath);
    Model(Model *m);
    char* getFilePath();
    int getNumberOfPoints();
    void setTexture(std::string *texture);
    void setColour(float red, float green, float blue){this->red = red; this->green = green; this->blue = blue;};
    void setDiffuseColor(float red, float green, float blue){dColor = new DiffuseColor(red, green, blue);};
    void setSpecularColor(float red, float green, float blue){sColor = new SpecularColor(red, green, blue);};
    void setEmissiveColor(float red, float green, float blue){eColor = new EmissiveColor(red, green, blue);};
    void setAmbientColor(float red, float green, float blue){aColor = new AmbientColor(red, green, blue);};
    void applyColour();
    void draw();
private:
    void loadPoints();
    void loadTexture();
};

#endif //CG_MODEL_H
