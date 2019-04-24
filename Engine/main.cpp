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

#define K 0.5f

using namespace std;
using namespace tinyxml2;

Scene scene;
GLenum OPTION = GL_FILL;

//Frames per second Variables
int timebase = 0, frame = 0;

//Camera Variables
float camX = 50, camY = 3, camZ = 50;
float lX = 0, lY = 3, lZ = 0;
float camAngle = 225;


float getAttributeOrDefault(XMLElement *element, const char* atr, float defaultValue){
    const XMLAttribute *atrXml = element->FindAttribute(atr);
    return atrXml == nullptr ? defaultValue : (float) atof((char*)atrXml->Value());
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
              0.0f,1.0f,0.0f);

    draw();
    viewFramesPerSecond();

    // End of frame
    glutSwapBuffers();
}

/*
 * Teclas:
 *      up    => a camara vai para cima;
 *      down  => a camara vai para baixo;
 *      right  => a camara roda para direita;
 *      left => a camara roda para esquerda;
 */
void processSpecialKeys(int key, int xx, int yy) {
    float newPx, newPz, newLx, newLz;
    newPx = camX;
    newPz = camZ;

    newLx = lX;
    newLz = lZ;

    switch (key) {
        case GLUT_KEY_RIGHT:
            camAngle -= 1.5;
            newLx = camX + sin((camAngle * 3.14) / 180);
            newLz = camZ + cos((camAngle * 3.14) / 180);
            break;
        case GLUT_KEY_LEFT:
            camAngle += 1.5;
            newLx = camX + sin((camAngle * 3.14) / 180);
            newLz = camZ + cos((camAngle * 3.14) / 180);
            break;
        case GLUT_KEY_UP:
            camY += K;
            break;
        case GLUT_KEY_DOWN:
            camY -= K;
            break;
        default:
            break;
    }

    camX = newPx;
    camZ = newPz;
    lX = newLx;
    lZ = newLz;

    lY=camY;

    glutPostRedisplay();
}

/*
 * Teclas:
 *      w => a camara vai para frente;
 *      s => a camara vai para tras;
 *      d => a camara vai para direita;
 *      a => a camara vai para esquerda;
 *      1 => Muda opção de visualização para FILL
 *      2 => Muda opção de visualização para LINE
 *      3 => Muda opção de visualização para POINT
 */
void processKeys(unsigned char key, int xx, int yy) {
    float newPx, newPz, newLx, newLz, dX, dZ, rX, rZ;
    newPx = camX;
    newPz = camZ;

    newLx = lX;
    newLz = lZ;

    dX = lX - camX;
    dZ = lZ - camZ;

    switch(key){
        case 'w':
            newPx = camX + K*dX;
            newPz = camZ + K*dZ;
            newLx = lX + K*dX;
            newLz = lZ + K*dZ;
            break;
        case 's':
            newPx = camX - K*dX;
            newPz = camZ - K*dZ;
            newLx = lX - K*dX;
            newLz = lZ - K*dZ;
            break;
        case 'a':
            rX = -dZ;
            rZ = dX;
            newPx = camX - K*rX;
            newPz = camZ - K*rZ;
            newLx = lX - K*rX;
            newLz = lZ - K*rZ;
            break;
        case 'd':
            rX = -dZ;
            rZ = dX;
            newPx = camX + K*rX;
            newPz = camZ + K*rZ;
            newLx = lX + K*rX;
            newLz = lZ + K*rZ;
            break;
        case '1':
            OPTION = GL_FILL;
            break;
        case '2':
            OPTION = GL_LINE;
            break;
        case '3':
            OPTION = GL_POINT;
            break;
        default:
            break;
    }

    camX = newPx;
    camZ = newPz;
    lX = newLx;
    lZ = newLz;

    //camY = hf(camX, camZ) + 3;
    lY=camY;

    glutPostRedisplay();
}

Model loadModel(XMLElement *model){
    Model m = Model((char*) model->FindAttribute("file")->Value());
    float red,green,blue;
    red = getAttributeOrDefault(model, "R", 255);
    green = getAttributeOrDefault(model, "G", 255);
    blue = getAttributeOrDefault(model, "B", 255);
    m.setColour(red/255, green/255, blue/255);

    return m;
}

void loadTranslation(XMLElement *trans, Group *group){
    if(trans->FindAttribute("time") != nullptr){
        group->setCatMull(true);
        group->setTransTime(getAttributeOrDefault(trans, "time", 0));
        XMLElement *child = trans->FirstChildElement();
        while(child){
            if(strcmp(child->Value(), "point") == 0){
                float transX, transY, transZ;
                transX = getAttributeOrDefault(child, "X", 0);
                transY = getAttributeOrDefault(child, "Y", 0);
                transZ = getAttributeOrDefault(child, "Z", 0);
                group->addPointToTranslation(transX, transY, transZ);
            } else {
                throw EngineException(string("There is no default tag in translation \"") + child->Value() + "\"");
            }
            child = child->NextSiblingElement();
        }
        group->setTranslate(0,0,0);
    } else {
        group->setCatMull(false);
        float transX, transY, transZ;
        transX = getAttributeOrDefault(trans, "X", 0);
        transY = getAttributeOrDefault(trans, "Y", 0);
        transZ = getAttributeOrDefault(trans, "Z", 0);
        group->setTranslate(transX, transY, transZ);
    }
}

void loadRotate(XMLElement *rotate, Group *group){
    float ang, axisX, axisY, axisZ;
    if(rotate->FindAttribute("time") != nullptr){
        group->setRotateWithTime(true);
        ang = getAttributeOrDefault(rotate, "time", 0);
    } else {
        group->setRotateWithTime(false);
        ang = getAttributeOrDefault(rotate, "ang", 0);
    }
    axisX = getAttributeOrDefault(rotate, "axisX", 0);
    axisY = getAttributeOrDefault(rotate, "axisY", 0);
    axisZ = getAttributeOrDefault(rotate, "axisZ", 0);
    group->setRotate(ang, axisX, axisY, axisZ);
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
                g.setScale(scaleX, scaleY, scaleZ);
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
            printf(e.what());
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
    doc.LoadFile( "../scene.xml" );

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