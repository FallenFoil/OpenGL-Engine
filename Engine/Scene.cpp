//
// Created by cesar on 2/25/19.
//

#include "Scene.h"

Group Scene::getGroup(){
    return this->root;
}

void Scene::setGroup(Group g) {
    this->root = g;
}

Scene::Scene(){
    this->root;
}