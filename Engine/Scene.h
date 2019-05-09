//
// Created by cesar on 2/25/19.
//

#ifndef CG_SCENE_H
#define CG_SCENE_H

#include "Model.h"
#include "group.h"
#include <vector>

class Light{
protected:
    short id;
    float pos[4] = {0};
public:
    short getID(){return id;};
    virtual void turnOn() = 0;
    void turnOff(){glDisable(GL_LIGHT0 + id);};
};

class PointLight : public Light{
public:
    PointLight(short id, float posX, float posY, float posZ){
        this->id = id;
        pos[0] = posX; pos[1] = posY; pos[2] = posZ; pos[3] = 1;
    }
    void turnOn() override{ glEnable(GL_LIGHT0 + id); glLightfv(GL_LIGHT0 + id, GL_POSITION, pos);}
};

class DiretionalLight : public Light{
public:
    DiretionalLight(short id, float posX, float posY, float posZ){
        this->id = id;
        pos[0] = posX; pos[1] = posY; pos[2] = posZ; pos[3] = 0;
    }
    void turnOn() override{ glEnable(GL_LIGHT0 + id); glLightfv(GL_LIGHT0 + id, GL_POSITION, pos);}
};

class SpotLight : public Light{
    float spotDir[3] = {0};
public:
    SpotLight(short id, float posX, float posY, float posZ, float dirX, float dirY, float dirZ){
        this->id = id;
        pos[0] = posX; pos[1] = posY; pos[2] = posZ; pos[3] = 1;
        spotDir[0] = dirX; spotDir[1] = dirY; spotDir[2] = dirZ; spotDir[3] = 1;
    }
    void turnOn() override{
        glEnable(GL_LIGHT0 + id);
        glLightfv(GL_LIGHT0 + id, GL_POSITION, pos);
        glLightfv(GL_LIGHT0 + id, GL_SPOT_DIRECTION, spotDir);
        glLightf(GL_LIGHT0 + id, GL_SPOT_CUTOFF, 45.0);
        glLightf(GL_LIGHT0 + id ,GL_SPOT_EXPONENT, 0.0);
    }
};

class Scene {
    std::vector<Group> root;
    std::vector<Light*> lights;
public:
    void turnOnLights();
    void turnOffLights();
    void addGroup(Group g);
    void addLight(Light *l);
    std::vector<Group> getGroups();
    Scene();
    Group getGroup(char* name);
};


#endif //CG_SCENE_H
