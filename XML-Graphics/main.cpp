#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <tinyxml2.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include "Ponto.h"
#include "Scene.h"

using namespace std;

Scene scene;

void loadScene() {
    printf("Loading Scene\n");
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
    printf("Finished loading Scene!!!\n");
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



void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here
    switch(c){

        default: break;
    }
    glutPostRedisplay();
}


void processSpecialKeys(int key, int xx, int yy) {

// put code to process special keys in here
}

void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(5.0,2,5.0,
              0.0,0.0 ,0.0,
              0.0f,1.0f,0.0f);

    glColor3f(1,1,1);

    glBegin(GL_TRIANGLES);
    forward_list<Model> models = scene.getModels();
    for(auto model = models.begin(); model != models.end(); model++){
        Model m = *model;
        forward_list<Ponto> pontos = m.getPoints();
        for(auto ponto = pontos.begin(); ponto != pontos.end(); ponto++){
            Ponto p = *ponto;
            glVertex3f(p.getX(), p.getY(), p.getZ());
            printf("%f %f %f\n", p.getX(), p.getY(), p.getZ());
        }
    }
    glEnd();

    //drawCylinder(1,2,10);
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(-4, 0, 0);
    glVertex3f(4, 0, 0);

    glColor3f(0,1,0);
    glVertex3f(0, -4, 0);
    glVertex3f(0, 4, 0);

    glColor3f(0,0,1);
    glVertex3f(0, 0, -4);
    glVertex3f(0, 0, 4);
    glEnd();
    // End of frame
    glutSwapBuffers();
}




int main(int argc, char** argv) {
    loadScene();

    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("CG@DI-UM");
    glPolygonMode(GL_FRONT , GL_LINE);

// Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


// enter GLUT's main cycle
    glutMainLoop();
    return 0;
}