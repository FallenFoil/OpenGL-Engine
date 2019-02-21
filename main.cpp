#include <iostream>
#include <tinyxml2.h>

int main() {
    std::cout << "Hello, World!" << std::endl;

    tinyxml2::XMLDocument doc;
    doc.LoadFile( "../scene.xml" );
    tinyxml2::XMLElement* child = doc.FirstChildElement( "Scene" )->FirstChildElement( "model");
    const char* text =  child->FindAttribute("file")->Value();
    printf("1st model = %s\n", text);
    printf("Finished!!!\n");
    return 0;
}