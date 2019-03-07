//
// Created by cesar on 2/25/19.
//

#ifndef CG_SCENE_H
#define CG_SCENE_H

#include "Model.h"
#include <vector>

class Scene {
    std::vector<Model> models;

public:
    void addModel(Model m);
    std::vector<Model> getModels();
    Scene();
};


#endif //CG_SCENE_H
