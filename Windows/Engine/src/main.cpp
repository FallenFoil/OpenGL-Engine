#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <ATB/AntTweakBar.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <tinyxml2.h>
#include <IL/il.h>
#include "Scene.h"
#include "EngineException.h"

#include <GL/glut.h>

using namespace std;
using namespace tinyxml2;

Scene scene;
XMLDocument doc;

//Frames per second Variables
int timebase = 0, frame = 0;

//Camera Variables
float camX = 0, camY = 0, camZ = 50;
float lX = 0, lY = 0, lZ = 0;
float upX = 0, upY = 1, upZ = 0;
float alpha = 3.1415, beta = 0, radius = 50;
float rotationSpeed = 0.001;
float camSpeed = 0.001;

//Controller Variables
/*
 *|--------------------------------------------------|
 *| RIGHT | LEFT | UP | DOWN | w | s | a | d | i | j |
 *|-------|------|----|------|---|---|---|---|---|---|
 *|   0   |   1  |  2 |  3   | 4 | 5 | 6 | 7 | 8 | 9 |
 *|--------------------------------------------------|
 */
int keys[10] = { 0,0,0,0,0,0,0,0,0,0 };
int mode = 0, axes = 0, light = 1;
GLenum OPTION = GL_FILL;

std::string getAttributeOrDefault(XMLElement* element, const char* atr, std::string defaultValue) {
	const XMLAttribute* atrXml = element->FindAttribute(atr);
	return atrXml == nullptr ? defaultValue : atrXml->Value();
}

float getAttributeOrDefault(XMLElement* element, const char* atr, float defaultValue) {
	const XMLAttribute* atrXml = element->FindAttribute(atr);
	return atrXml == nullptr ? defaultValue : (float)atof((char*)atrXml->Value());
}

bool getAttributeOrDefaultBool(XMLElement* element, const char* atr, bool defaultValue) {
	const XMLAttribute* atrXml = element->FindAttribute(atr);
	if (atrXml == nullptr) {
		return defaultValue;
	}
	else {
		if (strcmp((char*)atrXml->Value(), "True") == 0 || strcmp((char*)atrXml->Value(), "true") == 0) {
			return (bool)true;
		}
		else {
			return (bool)false;
		}
	}
}

float getAttributeOrDefault(XMLElement* element, std::string atr, float defaultValue) {
	return getAttributeOrDefault(element, atr.c_str(), defaultValue);
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
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
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);

	TwWindowSize(w, h);
}

void drawAxes() {
	float colors[4];



	glBegin(GL_LINES);

	colors[0] = 1.0; colors[1] = 0.0; colors[2] = 0.0; colors[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, colors);
	glColor3f(1, 0, 0);
	glVertex3f(1000, 0, 0);
	glVertex3f(-1000, 0, 0);

	colors[0] = 0.0; colors[1] = 1.0; colors[2] = 0.0; colors[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, colors);
	glColor3f(0, 1, 0);
	glVertex3f(0, 1000, 0);
	glVertex3f(0, -1000, 0);

	colors[0] = 0.0; colors[1] = 0.0; colors[2] = 1.0; colors[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, colors);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 1000);
	glVertex3f(0, 0, -1000);

	glEnd();

	glColor3f(1, 1, 1);
}

/*
 * Desenha as figuras dentro da estrutura da scene
 */
void draw() {
	vector<Group> groups = scene.getGroups();
	for (int i = 0; i < groups.size(); i++) {
		Group g = groups[i];
		g.draw();
	}
}

void viewFramesPerSecond() {
	float fps;
	char s[64];

	frame++;
	int time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		fps = frame * 1000.0 / (time - timebase);
		timebase = time;
		frame = 0;
		sprintf(s, "FPS: %f6", fps);
		glutSetWindowTitle(s);
	}
}

void sphericalToCartesian() {
	if (mode == 1) {
		camX = lX + (radius * cos(beta) * sin(alpha));
		camY = lY + (radius * sin(beta));
		camZ = lZ + (radius * cos(beta) * cos(alpha));
	}
	if (mode == 0) {
		lX = camX + (radius * cos(beta) * sin(alpha));
		lY = camY + (radius * sin(beta));
		lZ = camZ + (radius * cos(beta) * cos(alpha));
	}
}

void controllers() {
	int modeMult = -1;
	if (mode == 1) { modeMult = 1; }
	if (keys[0] == 1) { alpha += modeMult * rotationSpeed; sphericalToCartesian(); }
	if (keys[1] == 1) { alpha -= modeMult * rotationSpeed; sphericalToCartesian(); }
	if (keys[2] == 1) { beta += rotationSpeed; if (beta >= 1.57f) { beta = 1.57f; }sphericalToCartesian(); }
	if (keys[3] == 1) { beta -= rotationSpeed; if (beta <= -1.57f) { beta = -1.57f; }sphericalToCartesian(); }

	radius = sqrt(pow((lX - camX), 2) + pow((lY - camY), 2) + pow((lZ - camZ), 2));

	float dX = lX - camX;
	float dY = lY - camY;
	float dZ = lZ - camZ;
	float rX, rY, rZ;

	if (keys[4] == 1) { camX = camX + camSpeed * dX; camY = camY + camSpeed * dY; camZ = camZ + camSpeed * dZ; lX = lX + camSpeed * dX; lY = lY + camSpeed * dY; lZ = lZ + camSpeed * dZ; }
	if (keys[5] == 1) { camX = camX - camSpeed * dX; camY = camY - camSpeed * dY; camZ = camZ - camSpeed * dZ; lX = lX - camSpeed * dX; lY = lY - camSpeed * dY; lZ = lZ - camSpeed * dZ; }
	if (keys[6] == 1) { rX = -dZ; rY = 0; rZ = dX; camX = camX - camSpeed * rX; camY = camY - camSpeed * rY; camZ = camZ - camSpeed * rZ; lX = lX - camSpeed * rX; lY = lY - camSpeed * rY; lZ = lZ - camSpeed * rZ; }
	if (keys[7] == 1) { rX = -dZ; rY = 0; rZ = dX; camX = camX + camSpeed * rX; camY = camY + camSpeed * rY; camZ = camZ + camSpeed * rZ; lX = lX + camSpeed * rX; lY = lY + camSpeed * rY; lZ = lZ + camSpeed * rZ; }

	if (keys[8] == 1) { camY += 0.2f; lY += 0.2f; }
	if (keys[9] == 1) { camY -= 0.2f; lY -= 0.2f; }
}

void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, OPTION);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		lX, lY, lZ,
		upX, upY, upZ);

	scene.turnOnLights();

	controllers();
	if (axes) { drawAxes(); }
	viewFramesPerSecond();
	draw();
	TwDraw();

	// End of frame
	glutSwapBuffers();
}

void processSpecialKeys(int key, int xx, int yy) {
	if (!TwEventSpecialGLUT(key, xx, yy)) {
		switch (key) {
		case GLUT_KEY_RIGHT:
			keys[0] = 1;
			break;
		case GLUT_KEY_LEFT:
			keys[1] = 1;
			break;
		case GLUT_KEY_UP:
			keys[2] = 1;
			break;
		case GLUT_KEY_DOWN:
			keys[3] = 1;
			break;
		default:
			break;
		}
	}

	glutPostRedisplay();
}

void processUpSpecialKeys(int key, int xx, int yy) {
	if (!TwEventSpecialGLUT(key, xx, yy)) {
		switch (key) {
		case GLUT_KEY_RIGHT:
			keys[0] = 0;
			break;
		case GLUT_KEY_LEFT:
			keys[1] = 0;
			break;
		case GLUT_KEY_UP:
			keys[2] = 0;
			break;
		case GLUT_KEY_DOWN:
			keys[3] = 0;
			break;
		default:
			break;
		}
	}

	glutPostRedisplay();
}

void processKeys(unsigned char key, int xx, int yy) {
	if (!TwEventSpecialGLUT(key, xx, yy)) {
		switch (key) {
		case 'w':
			keys[4] = 1;
			break;
		case 's':
			keys[5] = 1;
			break;
		case 'a':
			keys[6] = 1;
			break;
		case 'd':
			keys[7] = 1;
			break;
		case 'i':
			keys[8] = 1;
			break;
		case 'j':
			keys[9] = 1;
			break;
		case 'm':
			if (mode == 0) { mode = 1; alpha -= 3.1415; }
			else { mode = 0; alpha += 3.1415; }
			beta = 0 - beta;
			break;
		default:
			break;
		}
	}

	glutPostRedisplay();
}

void processUpKeys(unsigned char key, int xx, int yy) {
	if (!TwEventSpecialGLUT(key, xx, yy)) {
		switch (key) {
		case 'w':
			keys[4] = 0;
			break;
		case 's':
			keys[5] = 0;
			break;
		case 'a':
			keys[6] = 0;
			break;
		case 'd':
			keys[7] = 0;
			break;
		case 'i':
			keys[8] = 0;
			break;
		case 'j':
			keys[9] = 0;
			break;
		default:
			break;
		}
	}

	glutPostRedisplay();
}

Model loadModel(XMLElement* model) {
	Model m = Model((char*)model->FindAttribute("file")->Value());
	std::string* texturePath = new std::string(getAttributeOrDefault(model, "texture", ""));
	m.setTexture(texturePath);
	float red, green, blue;
	std::string colors[] = { "", "diff", "spec", "emiss", "ambi" };
	for (int i = 0; i < 4; i++) {
		red = getAttributeOrDefault(model, colors[i] + "R", -1);
		green = getAttributeOrDefault(model, colors[i] + "G", -1);
		blue = getAttributeOrDefault(model, colors[i] + "B", -1);

		if (red >= 0 && red <= 255 && green >= 0 && green <= 255 && blue >= 0 && blue <= 255)
			switch (i) {
			case 0: m.setColour(red / 255.0f, green / 255.0f, blue / 255.0f); break;
			case 1: m.setDiffuseColor(red / 255.0f, green / 255.0f, blue / 255.0f); break;
			case 2: m.setSpecularColor(red / 255.0f, green / 255.0f, blue / 255.0f); break;
			case 3: m.setEmissiveColor(red / 255.0f, green / 255.0f, blue / 255.0f); break;
			case 4: m.setAmbientColor(red / 255.0f, green / 255.0f, blue / 255.0f); break;
			default: break;
			}
		else {
			if (red >= 0 || green >= 0 || blue >= 0) {
				throw EngineException("R, G and B attributes are all required to define color: " + colors[i]);
			}
			if (red > 255 || green > 255 || blue > 255) {
				throw EngineException("R, G and B attributes must be >=0 and <=255: " + colors[i]);
			}
		}
	}
	return m;
}

void loadTranslation(XMLElement* trans, Group* group) {
	if (trans->FindAttribute("time") != nullptr) {
		auto* t = new TranslateCatMull();
		t->setTransTime(getAttributeOrDefault(trans, "time", 0));
		t->setDrawOrbit(getAttributeOrDefaultBool(trans, "drawOrbit", false));
		XMLElement* child = trans->FirstChildElement();
		while (child) {
			if (strcmp(child->Value(), "point") == 0) {
				float transX, transY, transZ;
				transX = getAttributeOrDefault(child, "X", 0);
				transY = getAttributeOrDefault(child, "Y", 0);
				transZ = getAttributeOrDefault(child, "Z", 0);
				t->addPointToTranslation(transX, transY, transZ);
			}
			else {
				throw EngineException(string("There is no default tag in translation \"") + child->Value() + "\"");
			}
			child = child->NextSiblingElement();
		}
		group->setTranslate(t);
	}
	else {
		float transX, transY, transZ;
		float drawOrbit;
		transX = getAttributeOrDefault(trans, "X", 0);
		transY = getAttributeOrDefault(trans, "Y", 0);
		transZ = getAttributeOrDefault(trans, "Z", 0);
		drawOrbit = getAttributeOrDefaultBool(trans, "drawOrbit", false);
		auto* t = new TranslateDefault(transX, transY, transZ, drawOrbit);
		group->setTranslate(t);
	}
}

void loadRotate(XMLElement* rotate, Group* group) {
	float ang, axisX, axisY, axisZ;
	axisX = getAttributeOrDefault(rotate, "axisX", 0);
	axisY = getAttributeOrDefault(rotate, "axisY", 0);
	axisZ = getAttributeOrDefault(rotate, "axisZ", 0);
	if (rotate->FindAttribute("time") != nullptr) {
		ang = getAttributeOrDefault(rotate, "time", 0);
		RotateWithTime* r = new RotateWithTime(ang, axisX, axisY, axisZ);
		group->setRotate(r);
	}
	else {
		ang = getAttributeOrDefault(rotate, "ang", 0);
		RotateDefault* r = new RotateDefault(ang, axisX, axisY, axisZ);
		group->setRotate(r);
	}
}

Group loadGroup(XMLElement* group) {
	Group g;
	Group toAdd;
	XMLElement* child = group->FirstChildElement();
	while (child) {
		try {
			if (strcmp(child->Value(), "translate") == 0) {
				loadTranslation(child, &g);
			}
			else if (strcmp(child->Value(), "rotate") == 0) {
				loadRotate(child, &g);
			}
			else if (strcmp(child->Value(), "scale") == 0) {
				float scaleX, scaleY, scaleZ;
				scaleX = getAttributeOrDefault(child, "X", 1);
				scaleY = getAttributeOrDefault(child, "Y", 1);
				scaleZ = getAttributeOrDefault(child, "Z", 1);
				auto* s = new Scale(scaleX, scaleY, scaleZ);
				g.setScale(s);
			}
			else if (strcmp(child->Value(), "group") == 0) {
				toAdd = loadGroup(child);
				g.addGroup(&toAdd);
			}
			else if (strcmp(child->Value(), "models") == 0) {
				XMLElement* model = child->FirstChildElement("model");
				while (model) {
					Model m = loadModel(model);
					g.addModel(m);
					model = model->NextSiblingElement("model");
				}
			}
			else {
				throw EngineException(string("There is no default tag \"") + child->Value() + "\"");
			}
		}
		catch (EngineException & e) {
			printf("%s", e.what());
			exit(1);
		}
		child = child->NextSiblingElement();
	}
	return g;
}

void loadLights() {
	XMLElement* child;
	child = doc.FirstChildElement("scene")->FirstChildElement("lights");
	if (child) {
		child = child->FirstChildElement();
		glEnable(GL_LIGHTING);
	}

	while (child) {
		short id = 0;
		if (strcmp(child->Value(), "light") == 0) {
			string type = getAttributeOrDefault(child, "type", "");
			float ambR, ambG, ambB;
			ambR = getAttributeOrDefault(child, "ambR", 255) / 255;
			ambG = getAttributeOrDefault(child, "ambG", 255) / 255;
			ambB = getAttributeOrDefault(child, "ambB", 255) / 255;

			float diffR, diffG, diffB;
			diffR = getAttributeOrDefault(child, "diffR", 255) / 255;
			diffG = getAttributeOrDefault(child, "diffG", 255) / 255;
			diffB = getAttributeOrDefault(child, "diffB", 255) / 255;



			if (type == string("")) throw EngineException("type attribute is required in a light");

			if (type == string("POINT")) {
				float posX, posY, posZ;
				posX = getAttributeOrDefault(child, "posX", 0);
				posY = getAttributeOrDefault(child, "posY", 0);
				posZ = getAttributeOrDefault(child, "posZ", 0);
				auto* l = new PointLight(id, posX, posY, posZ);
				l->setAmb(ambR, ambG, ambB);
				l->setDiff(diffR, diffG, diffB);
				scene.addLight(l);
			}
			else if (type == string("DIRECTIONAL")) {
				float dirX, dirY, dirZ;
				dirX = getAttributeOrDefault(child, "dirX", 0);
				dirY = getAttributeOrDefault(child, "dirY", 0);
				dirZ = getAttributeOrDefault(child, "dirZ", 0);
				auto* l = new DiretionalLight(id, dirX, dirY, dirZ);
				l->setAmb(ambR, ambG, ambB);
				l->setDiff(diffR, diffG, diffB);
				scene.addLight(l);
			}
			else if (type == string("SPOT")) {
				float posX, posY, posZ, dirX, dirY, dirZ;
				posX = getAttributeOrDefault(child, "posX", 0);
				posY = getAttributeOrDefault(child, "posY", 0);
				posZ = getAttributeOrDefault(child, "posZ", 0);
				dirX = getAttributeOrDefault(child, "dirX", 0);
				dirY = getAttributeOrDefault(child, "dirY", 0);
				dirZ = getAttributeOrDefault(child, "dirZ", 0);
				auto* l = new SpotLight(id, posX, posY, posZ, dirX, dirY, dirZ);
				l->setAmb(ambR, ambG, ambB);
				l->setDiff(diffR, diffG, diffB);
				scene.addLight(l);
			}
			else {
				throw EngineException(string("There is no default type: ") + type);
			}
		}
		else throw EngineException(string("tag") + child->Value() + string("not recognized in <lights> </lights>"));
		child = child->NextSiblingElement();
	}
}

void cartesianToSpherical() {
	radius = sqrt(pow((camX - lX), 2) + pow((camY - lY), 2) + pow((camZ - lZ), 2));
	if (mode == 1) {
		alpha = atan((camX - lX) / (camZ - lZ));
		beta = asin((camY - lY) / radius);
	}
	if (mode == 0) {
		alpha = 3.1415 + atan((lX - camX) / (lZ - camZ));
		beta = asin((lY - camY) / radius);
	}
}

/*
 * Faz parse do ficheiro XML colocando a scene em memoria numa estrutura apropriada.
 */
void loadScene(char* str) {
	printf("Loading Scene\n");
	ilInit();
	XMLElement* child, * aux;
	doc.LoadFile(str);

	loadLights();
	child = doc.FirstChildElement("scene")->FirstChildElement("group");

	aux = doc.FirstChildElement("scene");
	camX = getAttributeOrDefault(aux, "CamX", 0);
	camY = getAttributeOrDefault(aux, "CamY", 0);
	camZ = getAttributeOrDefault(aux, "CamZ", 50);

	lX = getAttributeOrDefault(aux, "LookAtX", 0);
	lY = getAttributeOrDefault(aux, "LookAtY", 0);
	lZ = getAttributeOrDefault(aux, "LookAtZ", 0);

	cartesianToSpherical();

	while (child) {
		Group group = loadGroup(child);
		scene.addGroup(group);
		child = child->NextSiblingElement("group");
	}

	printf("Finished loading Scene!!!\n");
}

void Terminate() {
	TwTerminate();
}

void TW_CALL setCamX(const void* value, void* clientData) {
	camX = *(const float*)value;
	cartesianToSpherical();
}

void TW_CALL getCamX(void* value, void* clientData) {
	*(float*)value = camX;
}

void TW_CALL setCamY(const void* value, void* clientData) {
	camY = *(const float*)value;
	cartesianToSpherical();
}

void TW_CALL getCamY(void* value, void* clientData) {
	*(float*)value = camY;
}

void TW_CALL setCamZ(const void* value, void* clientData) {
	camZ = *(const float*)value;
	cartesianToSpherical();
}

void TW_CALL getCamZ(void* value, void* clientData) {
	*(float*)value = camZ;
}

void TW_CALL setLX(const void* value, void* clientData) {
	lX = *(const float*)value;
	cartesianToSpherical();
}

void TW_CALL getLX(void* value, void* clientData) {
	*(float*)value = lX;
}

void TW_CALL setLY(const void* value, void* clientData) {
	lY = *(const float*)value;
	cartesianToSpherical();
}

void TW_CALL getLY(void* value, void* clientData) {
	*(float*)value = lY;
}

void TW_CALL setLZ(const void* value, void* clientData) {
	lZ = *(const float*)value;
	cartesianToSpherical();
}

void TW_CALL getLZ(void* value, void* clientData) {
	*(float*)value = lZ;
}

void TW_CALL setAlpha(const void* value, void* clientData) {
	float aux = *(const float*)value;
	alpha = aux * 3.1415 / 180;
	sphericalToCartesian();
}

void TW_CALL getAlpha(void* value, void* clientData) {
	*(float*)value = alpha * 180 / 3.1415;
}

void TW_CALL setBeta(const void* value, void* clientData) {
	float aux = *(const float*)value;
	beta = aux * 3.1415 / 180;
	sphericalToCartesian();
}

void TW_CALL getBeta(void* value, void* clientData) {
	*(float*)value = beta * 180 / 3.1415;
}

void TW_CALL toogleAxes(void* clientData) {
	if (axes == 0) { axes = 1; }
	else { axes = 0; }
}

void TW_CALL exitProgram(void* clientData) {
	Terminate();
	exit(0);
}

void TW_CALL toogleLighting(void* clientData) {
	if (light == 1) { glDisable(GL_LIGHTING); light = 0; }
	else { glEnable(GL_LIGHTING); light = 1; }
}

void TW_CALL polygonModePoint(void* clientData) {
	OPTION = GL_POINT;
}

void TW_CALL polygonModeLine(void* clientData) {
	OPTION = GL_LINE;
}

void TW_CALL polygonModeFill(void* clientData) {
	OPTION = GL_FILL;
}

void menuTweakBar(TwBar* myBar) {
	myBar = TwNewBar("Menu");

	TwDefine(" Menu refresh=0.5 ");

	TwAddVarCB(myBar, "Cam X", TW_TYPE_FLOAT, setCamX, getCamX, NULL, " step=0.01 group='Cartesianas' ");
	TwAddVarCB(myBar, "Cam Y", TW_TYPE_FLOAT, setCamY, getCamY, NULL, " step=0.01 group='Cartesianas' ");
	TwAddVarCB(myBar, "Cam Z", TW_TYPE_FLOAT, setCamZ, getCamZ, NULL, " step=0.01 group='Cartesianas' ");

	TwAddVarCB(myBar, "LookAt X", TW_TYPE_FLOAT, setLX, getLX, NULL, " step=0.01 group='Cartesianas' ");
	TwAddVarCB(myBar, "LookAt Y", TW_TYPE_FLOAT, setLY, getLY, NULL, " step=0.01 group='Cartesianas' ");
	TwAddVarCB(myBar, "LookAt Z", TW_TYPE_FLOAT, setLZ, getLZ, NULL, " step=0.01 group='Cartesianas' ");

	TwDefine(" Menu/Cartesianas  group=Camera \n");

	TwAddVarCB(myBar, "Alpha", TW_TYPE_FLOAT, setAlpha, getAlpha, NULL, " step=0.01 group='Esfericas' ");
	TwAddVarCB(myBar, "Beta", TW_TYPE_FLOAT, setBeta, getBeta, NULL, " min=-90 max=90 step=0.01 group='Esfericas' ");
	TwAddVarRW(myBar, "Radius", TW_TYPE_FLOAT, &radius, " step=0.01 group='Esfericas' ");

	TwDefine(" Menu/Esfericas  group=Camera \n");

	TwAddVarRW(myBar, "Cam Speed", TW_TYPE_FLOAT, &camSpeed, " min=0 step=0.001 group='Camera' ");
	TwAddVarRW(myBar, "Rotation Speed", TW_TYPE_FLOAT, &rotationSpeed, " min=0 step=0.001 group='Camera' ");

	TwAddButton(myBar, "POINT", polygonModePoint, NULL, " group='Polygon Mode' ");
	TwAddButton(myBar, "LINE", polygonModeLine, NULL, " group='Polygon Mode' ");
	TwAddButton(myBar, "FILL", polygonModeFill, NULL, " group='Polygon Mode' ");

	TwDefine(" Menu/'Polygon Mode' opened=false ");

	TwAddButton(myBar, "Axes", toogleAxes, NULL, "");
	TwAddButton(myBar, "Lighting", toogleLighting, NULL, "");

	TwAddButton(myBar, "Exit Program", exitProgram, NULL, "");
}

int main(int argc, char** argv) {
	/*
	if (argc < 2) {
		printf("Path to scene is required.\n");
		exit(1);
	}*/

	//init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1000, 800);
	glutCreateWindow("MyWindow");

	//Required callback registry
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	atexit(Terminate);

	//OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

#ifndef __APPLE__
	glewInit();
#endif

	loadScene("..\\scenes\\scene3.xml");
	glEnable(GL_TEXTURE_2D);

	TwBar* bar;

	TwInit(TW_OPENGL, nullptr);

	menuTweakBar(bar);

	//Callback registration for keyboard processing
	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);

	glutKeyboardFunc(processKeys);
	glutKeyboardUpFunc(processUpKeys);
	glutSpecialFunc(processSpecialKeys);
	glutSpecialUpFunc(processUpSpecialKeys);

	TwGLUTModifiersFunc(glutGetModifiers);

	//enter GLUT's main cycle
	glutMainLoop();

	return 0;
}