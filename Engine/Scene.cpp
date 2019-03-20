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

Scene::Scene(){
    this->root;
}