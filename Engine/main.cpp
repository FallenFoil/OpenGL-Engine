#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <tinyxml2.h>
#include <math.h>
#include "Ponto.h"
#include "Scene.h"

using namespace std;
using namespace tinyxml2;

#define HSPEED 0.05
#define VSPEED 0.05
#define ZOOM 1

Scene scene;
int frame= 0, timebase = 0;
char title[100];
double alpha=45*M_PI/180, beta=35.264389*M_PI/180, radius = sqrt(75.00),
        camX = radius * cos(beta) * sin(alpha),
        camY = radius * sin(beta),
        camZ = radius * cos(beta) * cos(alpha);


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

void drawModel(Model m){
    m.applyColour();
    glBegin(GL_TRIANGLES);
    for (int ponto = 0; ponto < m.getNumberOfPoints(); ponto++) {
        Ponto p = m.getPoint(ponto);
        glVertex3f(p.getX(), p.getY(), p.getZ());
    }
    glEnd();
    glColor3f(1,1,1);
}

void drawGroup(Group fatherGroup){
    glPushMatrix();
        fatherGroup.applyTransformations();
        vector<Model> models = fatherGroup.getModels();

        for(auto model = models.begin(); model != models.end(); model++) {
            Model m = *model;
            drawModel(m);
        }

    vector<Group> childGroups = fatherGroup.getGroups();
    for(int i = 0; i < childGroups.size(); i++){
        Group g = childGroups[i];
        drawGroup(g);
    }
    glPopMatrix();
}

/*
 * Desenha as figuras dentro da estrutura da scene
 */
void draw(){
    /*glBegin(GL_LINES);

    glColor3f(1,0,0);
    glVertex3f(1000,0,0);
    glVertex3f(-1000,0,0);

    glColor3f(0,1,0);
    glVertex3f(0,1000,0);
    glVertex3f(0,-1000,0);

    glColor3f(0,0,1);
    glVertex3f(0,0,1000);
    glVertex3f(0,0,-1000);

    glEnd();*/

    glColor3f(1,1,1);

    vector<Group> groups = scene.getGroups();
    for(int i = 0; i < groups.size(); i++){
        Group g = groups[i];
        drawGroup(g);
    }

}

void viewFramesPerSecond(){
    int time;
    float fps;
    char buffer[100] ;
    frame++;
    time=glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        fps = frame*1000.0/(time-timebase);
        timebase = time;
        frame = 0;
        sprintf(title, "FPS - %f", fps);
    }
    glutSetWindowTitle(title);
}

void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(camX,camY,camZ,
              0,0,0,
              0.0f,1.0f,0.0f);

    draw();
    viewFramesPerSecond();

    // End of frame
    glutSwapBuffers();
}

/*
 * Teclas:
 *      w => a camara vai para cima;
 *      s => a camara vai para baixo;
 *      d => a camara vai para direita;
 *      a => a camara vai para esquerda;
 *      i => a camara da zoom in;
 *      o => a camara da zoom out;
 */
void processKeys(unsigned char c, int xx, int yy) {
// put code to process regular keys in here
    switch (c) {
        case 'w':
            if(beta+VSPEED<=M_PI/2){beta+=VSPEED;}
            else{beta=M_PI/2;}
            break;
        case 'a':
            alpha-=HSPEED;
            break;
        case 'd':
            alpha+=HSPEED;
            break;
        case 's':
            if(beta-VSPEED>=-M_PI/2){beta-=VSPEED;}
            else{beta=-M_PI/2;}
            break;
        case 'o':
            radius += ZOOM;
            break;
        case 'i':
            if(radius-ZOOM>0){radius -= ZOOM;}
            break;
        default:
            break;
    }

    camX = radius * cos(beta) * sin(alpha);
    camY = radius * sin(beta);
    camZ = radius * cos(beta) * cos(alpha);

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

Group loadGroup(XMLElement *group){
    Group g;
    Group toAdd;
    XMLElement *child = group->FirstChildElement();
    while(child) {
        if (strcmp(child->Value(), "translate") == 0) {
            float transX, transY, transZ;
            transX = getAttributeOrDefault(child, "X", 0);
            transY = getAttributeOrDefault(child, "Y", 0);
            transZ = getAttributeOrDefault(child, "Z", 0);
            g.setTranslate(transX, transY, transZ);
        } else if (strcmp(child->Value(), "rotate") == 0) {
            float ang, axisX, axisY, axisZ;
            ang = getAttributeOrDefault(child, "ang", 0);
            axisX = getAttributeOrDefault(child, "axisX", 0);
            axisY = getAttributeOrDefault(child, "axisY", 0);
            axisZ = getAttributeOrDefault(child, "axisZ", 0);
            g.setRotate(ang, axisX, axisY, axisZ);
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
            while(model){
                Model m = loadModel(model);
                g.addModel(m);
                model = model->NextSiblingElement("model");
            }
        } else {
            printf("There is no default tag: %s", child->Value());
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
    loadScene();

    //init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1000,800);
    glutCreateWindow("MyWindow");

    //Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    //Callback registration for keyboard processing
    glutKeyboardFunc(processKeys);

    //OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT , GL_FILL);


    //enter GLUT's main cycle
    glutMainLoop();

    return 1;
}