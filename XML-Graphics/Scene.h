//
// Created by cesar on 2/25/19.
//

#ifndef CG_SCENE_H
#define CG_SCENE_H

#include "Model.h"
#include <forward_list>

class Scene {
    std::forward_list<Model> models;

public:
    void addModel(Model m);
    std::forward_list<Model> getModels();
    Scene();
};


#endif //CG_SCENE_H
