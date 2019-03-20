//
// Created by cesar on 2/25/19.
//

#ifndef CG_SCENE_H
#define CG_SCENE_H

#include "Model.h"
#include "group.h"
#include <vector>

class Scene {
    std::vector<Group> root;

public:
    void addGroup(Group g);
    std::vector<Group> getGroups();
    Scene();
};


#endif //CG_SCENE_H
