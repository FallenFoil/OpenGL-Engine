#include <iostream>
#include <tinyxml2.h>
#include "Scene.h"

int main() {
    Scene scene;
    std::cout << "Hello, World!" << std::endl;
    tinyxml2::XMLElement *child, *sibling;
    tinyxml2::XMLDocument doc;
    doc.LoadFile( "../scene.xml" );

    child = doc.FirstChildElement( "Scene" )->FirstChildElement( "model");
    Model toAdd;
    if(child){
        toAdd = Model((char*) child->FindAttribute("file")->Value());
        scene.addModel(toAdd);
    }
    while(child && (sibling = child->NextSiblingElement())){
        toAdd = Model((char*) sibling->FindAttribute("file")->Value());
        scene.addModel(toAdd);
        child = sibling;
    }
    printf("Finished!!!\n");
    return 0;
}