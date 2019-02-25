//
// Created by cesar on 2/25/19.
//

#include "Model.h"
#include <cstring>

Model::Model(char* filepath) {
    std::strcpy(this->filePath, filepath);
}

Model::Model() {
    std::strcpy(this->filePath, "");
}

char* Model::getFilePath() {
    return this->filePath;
}
