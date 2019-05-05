//
// Created by cesar on 2/25/19.
//

#include "Scene.h"

std::vector<Group> Scene::getGroups(){
    return this->root;
}

void Scene::addGroup(Group g) {
    this->root.push_back(g);
}

void Scene::addLight(Light *l) {
    if(l->getID() > 7) throw EngineException("Can only support 8 lights maximum");
    else this->lights.push_back(l);
}

void Scene::turnOnLights() {
    for(int i = 0; i < lights.size(); i++){
        lights[i]->turnOn();
    }
}

void Scene::turnOffLights() {
    for(int i = 0; i < lights.size(); i++){
        lights[i]->turnOff();
    }
}

Scene::Scene(){
    this->root;
}