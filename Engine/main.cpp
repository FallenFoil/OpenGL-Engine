#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <tinyxml2.h>
#include <math.h>
#include "Ponto.h"
#include "Scene.h"
#include "EngineException.h"

using namespace std;
using namespace tinyxml2;

Scene scene;
GLenum OPTION = GL_FILL;

//Frames per second Variables
int timebase = 0, frame = 0;

//Camera Variables
#define K 0.2f
float camX = 0, camY=0, camZ = 50;
float lX=0, lY=0, lZ=0;
float upX=0, upY=1, upZ=0;
float alpha = 3.1415, beta = 0, radius = 50;


float getAttributeOrDefault(XMLElement *element, const char* atr, float defaultValue){
    const XMLAttribute *atrXml = element->FindAttribute(atr);
    return atrXml == nullptr ? defaultValue : (float) atof((char*)atrXml->Value());
}

float getAttributeOrDefault(XMLElement *element, std::string atr, float defaultValue){
    return getAttributeOrDefault(element, atr.c_str(), defaultValue);
}

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void drawAxes(){
    glBegin(GL_LINES);

    glColor3f(1,0,0);
    glVertex3f(1000,0,0);
    glVertex3f(-1000,0,0);

    glColor3f(0,1,0);
    glVertex3f(0,1000,0);
    glVertex3f(0,-1000,0);

    glColor3f(0,0,1);
    glVertex3f(0,0,1000);
    glVertex3f(0,0,-1000);

    glEnd();

    glColor3f(1,1,1);
}

/*
 * Desenha as figuras dentro da estrutura da scene
 */
void draw(){
   // drawAxes();

    vector<Group> groups = scene.getGroups();
    for(int i = 0; i < groups.size(); i++){
        Group g = groups[i];
        g.draw();
    }
}

void viewFramesPerSecond(){
    float fps;
    char s[64];

    frame++;
    int time=glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        fps = frame*1000.0/(time-timebase);
        timebase = time;
        frame = 0;
        sprintf(s, "FPS: %f6.2", fps);
        glutSetWindowTitle(s);
    }
}

void renderScene(void) {
        // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, OPTION);

    // set the camera
    glLoadIdentity();
    gluLookAt(camX,camY,camZ,
              lX,lY,lZ,
              upX,upY,upZ);

    drawAxes();
    draw();
    viewFramesPerSecond();

    // End of frame
    glutSwapBuffers();
}

void sphericalToCartesian(int type){
    if(type==1){
        camX = lX + (radius * cos(beta) * sin(alpha));
        camY = lY + (radius * sin(beta));
        camZ = lZ + (radius * cos(beta) * cos(alpha));
    }
    if(type==2){
        lX = camX + (5 * cos(beta) * sin(alpha));
        lY = camY + (5 * sin(beta));
        lZ = camZ + (5 * cos(beta) * cos(alpha));
    }
}

void processSpecialKeys(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            alpha -= 0.1;
            sphericalToCartesian(2);
            break;
        case GLUT_KEY_LEFT:
            alpha += 0.1;
            sphericalToCartesian(2);
            break;
        case GLUT_KEY_UP:
            beta += 0.1f;
            if (beta >= 1.57f){beta = 1.57f;}
            sphericalToCartesian(2);
            break;
        case GLUT_KEY_DOWN:
            beta -= 0.1f;
            if (beta <= -1.57f){beta = -1.57f;}
            sphericalToCartesian(2);
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

void processKeys(unsigned char key, int xx, int yy) {
    float dX = lX - camX;
    float dY = lY - camY;
    float dZ = lZ - camZ;
    float rX,rY,rZ;

    switch(key){
        case 'w':
            camX = camX + K*dX;
            camY = camY + K*dY;
            camZ = camZ + K*dZ;
            lX = lX + K*dX;
            lY = lY + K*dY;
            lZ = lZ + K*dZ;
            break;
        case 's':
            camX = camX - K*dX;
            camY = camY - K*dY;
            camZ = camZ - K*dZ;
            lX = lX - K*dX;
            lY = lY - K*dY;
            lZ = lZ - K*dZ;
            break;
        case 'a':
            rX = -dZ;
            rY = 0;
            rZ = dX;
            camX = camX - K*rX;
            camY = camY - K*rY;
            camZ = camZ - K*rZ;
            lX = lX - K*rX;
            lY = lY - K*rY;
            lZ = lZ - K*rZ;
            break;
        case 'd':
            rX = -dZ;
            rY = 0;
            rZ = dX;
            camX = camX + K*rX;
            camY = camY + K*rY;
            camZ = camZ + K*rZ;
            lX = lX + K*rX;
            lY = lY + K*rY;
            lZ = lZ + K*rZ;
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

Model loadModel(XMLElement *model){
    Model m = Model((char*) model->FindAttribute("file")->Value());
    float red,green,blue;
    std::string colors[] = {"", "diff", "spec", "emiss", "ambi"};
    for(int i = 0; i < colors->length(); i++){
        red = getAttributeOrDefault(model, colors[i] + "R", 0);
        green = getAttributeOrDefault(model, colors[i] + "G", 0);
        blue = getAttributeOrDefault(model, colors[i] + "B", 0);
        switch(i){
            case 0: m.setColour(red, green, blue); break;
            case 1: m.setDiffuseColor(red, green, blue); break;
            case 2: m.setSpecularColor(red, green, blue); break;
            case 3: m.setEmissiveColor(red, green, blue); break;
            case 4: m.setAmbientColor(red, green, blue); break;
            default: break;
        }
    }
    return m;
}

void loadTranslation(XMLElement *trans, Group *group){
    if(trans->FindAttribute("time") != nullptr){
        auto *t = new TranslateCatMull();
        t->setTransTime(getAttributeOrDefault(trans, "time", 0));
        XMLElement *child = trans->FirstChildElement();
        while(child){
            if(strcmp(child->Value(), "point") == 0){
                float transX, transY, transZ;
                transX = getAttributeOrDefault(child, "X", 0);
                transY = getAttributeOrDefault(child, "Y", 0);
                transZ = getAttributeOrDefault(child, "Z", 0);
                t->addPointToTranslation(transX, transY, transZ);
            } else {
                throw EngineException(string("There is no default tag in translation \"") + child->Value() + "\"");
            }
            child = child->NextSiblingElement();
        }
        group->setTranslate(t);
    } else {
        float transX, transY, transZ;
        transX = getAttributeOrDefault(trans, "X", 0);
        transY = getAttributeOrDefault(trans, "Y", 0);
        transZ = getAttributeOrDefault(trans, "Z", 0);
        auto *t = new TranslateDefault(transX, transY, transZ);
        group->setTranslate(t);
    }
}

void loadRotate(XMLElement *rotate, Group *group){
    float ang, axisX, axisY, axisZ;
    axisX = getAttributeOrDefault(rotate, "axisX", 0);
    axisY = getAttributeOrDefault(rotate, "axisY", 0);
    axisZ = getAttributeOrDefault(rotate, "axisZ", 0);
    if(rotate->FindAttribute("time") != nullptr){
        ang = getAttributeOrDefault(rotate, "time", 0);
        RotateWithTime *r = new RotateWithTime(ang, axisX, axisY, axisZ);
        group->setRotate(r);
    } else {
        ang = getAttributeOrDefault(rotate, "ang", 0);
        RotateDefault *r = new RotateDefault(ang, axisX, axisY, axisZ);
        group->setRotate(r);
    }
}

Group loadGroup(XMLElement *group){
    Group g;
    Group toAdd;
    XMLElement *child = group->FirstChildElement();
    while(child) {
        try {
            if (strcmp(child->Value(), "translate") == 0) {
                loadTranslation(child, &g);
            } else if (strcmp(child->Value(), "rotate") == 0) {
                loadRotate(child, &g);
            } else if (strcmp(child->Value(), "scale") == 0) {
                float scaleX, scaleY, scaleZ;
                scaleX = getAttributeOrDefault(child, "X", 1);
                scaleY = getAttributeOrDefault(child, "Y", 1);
                scaleZ = getAttributeOrDefault(child, "Z", 1);
                auto *s = new Scale(scaleX, scaleY, scaleZ);
                g.setScale(s);
            } else if (strcmp(child->Value(), "group") == 0) {
                toAdd = loadGroup(child);
                g.addGroup(&toAdd);
            } else if (strcmp(child->Value(), "models") == 0) {
                XMLElement *model = child->FirstChildElement("model");
                while (model) {
                    Model m = loadModel(model);
                    g.addModel(m);
                    model = model->NextSiblingElement("model");
                }
            } else {
                throw EngineException(string("There is no default tag \"") + child->Value() + "\"");
            }
        } catch (EngineException &e){
            printf("%s", e.what());
            exit(1);
        }
        child = child->NextSiblingElement();
    }
    return g;
}

/*
 * Faz parse do ficheiro XML colocando a scene em memoria numa estrutura apropriada.
 */
void loadScene() {
    printf("Loading Scene\n");
    XMLElement *child;
    XMLDocument doc;
    doc.LoadFile( "../scene3.xml" );

    child = doc.FirstChildElement( "scene" )->FirstChildElement( "group");
    while(child){
        Group group = loadGroup(child);
        scene.addGroup(group);
        child = child->NextSiblingElement( "group");
    }
    printf("Finished loading Scene!!!\n");
}

int main(int argc, char** argv) {
    //init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1000,800);
    glutCreateWindow("MyWindow");
    glEnableClientState(GL_VERTEX_ARRAY);

    //Required callback registry
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);

    //Callback registration for keyboard processing
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

#ifndef __APPLE__
    glewInit();
#endif

    loadScene();

    //OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    //enter GLUT's main cycle
    glutMainLoop();

    return 0;
}