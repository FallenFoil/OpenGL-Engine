#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <tinyxml2.h>
#include <math.h>
#include "Ponto.h"
#include "Scene.h"

using namespace std;

#define HSPEED 0.05
#define VSPEED 0.05
#define ZOOM 1

Scene scene;
double alpha=45*M_PI/180, beta=35.264389*M_PI/180, radius = sqrt(75.00),
        camX = radius * cos(beta) * sin(alpha),
        camY = radius * sin(beta),
        camZ = radius * cos(beta) * cos(alpha);

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



void draw(){
    glBegin(GL_LINES);

    glColor3f(1,0,0);
    glVertex3f(10,0,0);
    glVertex3f(-10,0,0);

    glColor3f(0,1,0);
    glVertex3f(0,10,0);
    glVertex3f(0,-10,0);

    glColor3f(0,0,1);
    glVertex3f(0,0,10);
    glVertex3f(0,0,-10);

    glEnd();

    glColor3f(1,1,1);

    vector<Model> models = scene.getModels();
    for(auto model = models.begin(); model != models.end(); model++){
        Model m = *model;
        glBegin(GL_TRIANGLES);
        for(int ponto = 0 ; ponto < m.getNumberOfPoints(); ponto++){
            Ponto p = m.getPoint(ponto);
            glVertex3f(p.getX(), p.getY(), p.getZ());
        }
        glEnd();
    }
}

void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(camX,camY,camZ,
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);

    draw();

    // End of frame
    glutSwapBuffers();
}

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

void processSpecialKeys(int key, int xx, int yy) {

// put code to process special keys in here
}

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

int main(int argc, char** argv) {
    loadScene();

// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1000,800);
    glutCreateWindow("MyWindow");

// Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT , GL_LINE);


// enter GLUT's main cycle
    glutMainLoop();

    return 1;
}