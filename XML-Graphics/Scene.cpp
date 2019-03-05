//
// Created by cesar on 2/25/19.
//

#include "Scene.h"

void Scene::addModel(Model m){
    this->models.push_back(m);
}

std::vector<Model> Scene::getModels(){
    return this->models;
}

Scene::Scene(){
    this->models;
}