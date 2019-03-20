//
// Created by cesar on 2/25/19.
//

#ifndef CG_SCENE_H
#define CG_SCENE_H

#include "Model.h"
#include "group.h"
#include <vector>

class Scene {
    Group root;

public:
    void setGroup(Group g);
    Group getGroup();
    Scene();
};


#endif //CG_SCENE_H
