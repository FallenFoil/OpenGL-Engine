//
// Created by cesar on 2/25/19.
//

#ifndef CG_MODEL_H
#define CG_MODEL_H

#include <string>

class Model {
    //// Tornar o tamanho do buffer variavel mais tarde ////
    char filePath[100];
public:
    Model(char* filePath);
    Model();
    char* getFilePath();
};


#endif //CG_MODEL_H
