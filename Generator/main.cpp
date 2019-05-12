#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Ponto.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

//Patch variables
int nPatches=0;
vector<vector<int>> indexes;
int nPoints=0;
vector<string> points;
int resU,resV;


string createBox(float width, float height, float depth,  int stacks, int slices, int depthDivisions){
    if(stacks < 1 && slices < 1) return string();

    stringstream ss;
    ss << (slices * stacks * 2 + stacks * depthDivisions * 2 + depthDivisions * slices * 2) * 6 << "\n";
    float boxWidth, boxHeight, boxDepth;
    boxWidth = width/slices; boxHeight = height/stacks; boxDepth = depth/depthDivisions;
    Ponto t1(-width/2, height/2, -depth/2);
    Ponto t2, t3, t4, b1, b2, b3, b4;

    for(int i = 0; i < stacks; i++){
        b1 = t1.translate(0,-boxHeight, 0);
        Ponto b1Saved(b1);
        for(int k = 0; k < depthDivisions; k++){
            t2 = t1.translate(0, 0, boxDepth);
            Ponto t2Saved(t2);
            for(int j = 0; j < slices; j++){
                t2 = t1.translate(0, 0, boxDepth);
                t3 = t1.translate(boxWidth, 0, boxDepth);
                t4 = t1.translate(boxWidth, 0, 0);
                b1 = t1.translate(0,-boxHeight, 0);
                b2 = t1.translate(0,-boxHeight, boxDepth);
                b3 = t1.translate(boxWidth,-boxHeight, boxDepth);
                b4 = t1.translate(boxWidth,-boxHeight, 0);

                if(i == 0)
                    ss << toStringPlane(t1,t2,t3,t4);
                if(i == stacks-1)
                    ss << toStringPlane(b3,b2,b1,b4);
                if(k == 0)
                    ss << toStringPlane(b4,b1,t1,t4);
                if(k == depthDivisions-1)
                    ss << toStringPlane(t2,b2,b3,t3);
                if(j == 0)
                    ss << toStringPlane(b2,t2,t1,b1);
                if(j == slices-1)
                    ss << toStringPlane(t3,b3,b4,t4);

                t1 = t1.translate(boxWidth,0,0);
            }
            t1 = Ponto(t2Saved);
        }
        t1 = Ponto(b1Saved);
    }
    return ss.str();
}

/*
 * Calcula as coordenadas dos vertices de um plano.
 *
 * Recebe:
 * (oX,oY,oZ) são as coordenadas do centro do plano.
 * size(X,Y,Z) são o tamanho do plano no eixo. Se for 0 significa que todas a coordenadas são 0 nesse eixo.
 * direction é a direção do plano, varia entre 1 e -1. 1 é um plano normal e -1 é o inverso.
 *
 * Retorna uma string com os vertice do plano
 *
 * D - C
 * |   |
 * A - B
 */
string createPlane(float oX, float oY, float oZ, float sizeX, float sizeY, float sizeZ, int direction){
    float x, y=0, z;
    stringstream ss;
    int dirX=direction, dirY=direction, dirZ=direction;

    if(sizeX==0){dirX=1;}
    if(sizeY==0){dirY=1;}
    if(sizeZ==0){dirZ=1;}


    //Triangulo esquerdo

    //A
    x = (oX-sizeX/2)*dirX; y = (oY-sizeY/2)*dirY; z = (oZ+sizeZ/2)*dirZ;
    ss << x << " " << y << " " << z << endl;
    ss << x << " " << direction << " " << z << endl;
    ss << 0 << " " << 0 << endl;

    //C
    x = (oX+sizeX/2)*dirX; y = (oY+sizeY/2)*dirY; z = (oZ-sizeZ/2)*dirZ;
    ss << x << " " << y << " " << z << endl;
    ss << x << " " << direction << " " << z << endl;
    ss << 1 << " " << 1 << endl;

    //D
    x = oX-sizeX/2; y = oY+sizeY/2; z = oZ-sizeZ/2;
    if(sizeX==0){z = sizeZ/2;}
    ss << x << " " << y << " " << z << endl;
    ss << x << " " << direction << " " << z << endl;
    ss << 0 << " " << 1 << endl;

    //Triangulo direito

    //C
    x = (oX+sizeX/2)*dirX; y = (oY+sizeY/2)*dirY; z = (oZ-sizeZ/2)*dirZ;
    ss << x << " " << y << " " << z << endl;
    ss << x << " " << direction << " " << z << endl;
    ss << 1 << " " << 1 << endl;

    //A
    x = (oX-sizeX/2)*dirX; y = (oY-sizeY/2)*dirY; z = (oZ+sizeZ/2)*dirZ;
    ss << x << " " << y << " " << z << endl;
    ss << x << " " << direction << " " << z << endl;
    ss << 0 << " " << 0 << endl;

    //B
    x = oX+sizeX/2; y = oY-sizeY/2; z = oZ+sizeZ/2;
    if(sizeX==0){z = -sizeZ/2;}
    ss << x << " " << y << " " << z << endl;
    ss << x << " " << direction << " " << z << endl;
    ss << 1 << " " << 0 << endl;

    return ss.str();
}

/*
 * Transforma coordenadas esfericas em cartesianas
 *
 * Recebe o angulo alpha e beta e o raio da esfera
 *
 * Retorna a string com o ponto em coordenadas cartesianas
 */
string createSpherePoint(float alpha, float beta, float radius){
    float x,y,z;
    stringstream ss;

    z = radius * cos(beta) * cos(alpha);
    x = radius * cos(beta) * sin(alpha);
    y = radius * sin(beta);

    ss << x << " " << y << " " << z << endl;

    return ss.str();
}

/*
 * Calcula os pontos dos tringulos que formam uma esfera.
 *
 * Recebe o raio da esfera, o numero de fatias da esfera e o numeros de andares da esfera.
 *
 * Retorna uma string com todos os pontos da esfera.
 * 
 * A - B
 * |   |
 * C - D
 */
string createSphere(float radius, int slices, int stacks){
    stringstream ss;

    if(slices < 3 || stacks < 2){
        return string();
    }

    int nPontos = (6 * slices * stacks) - (6 * slices);

    ss << nPontos << endl;

    float oneSlice = (float) (2 * M_PI)/slices, oneStack = (float) (M_PI)/stacks;
    float textSlice = 1.0f / slices, textStack = 1.0f / stacks;

    for(float i = -(stacks/2.0f); i < (stacks/2.0f); i++){
        for(int j = 0; j < slices; j++){
            if(i != (stacks/2.0f)-1){
                ss << createSpherePoint(oneSlice*j, oneStack*i, radius);
                ss << createSpherePoint(oneSlice*j, oneStack*i, 1);
                ss << (j)*textSlice << " " << 1-(i+(stacks/2.0f))*textStack << endl;

                ss << createSpherePoint(oneSlice*(j+1), oneStack*(i+1), radius);
                ss << createSpherePoint(oneSlice*(j+1), oneStack*(i+1), 1);
                ss << (j+1)*textSlice << " " << 1-(i+1+(stacks/2.0f))*textStack << endl;

                ss << createSpherePoint(oneSlice*j, oneStack*(i+1), radius);
                ss << createSpherePoint(oneSlice*j, oneStack*(i+1), 1);
                ss << (j)*textSlice << " " << 1-(i+1+(stacks/2.0f))*textStack << endl;
            }

            if(i != -(stacks/2.0f)){
                ss << createSpherePoint(oneSlice*(j+1), oneStack*(i+1), radius);
                ss << createSpherePoint(oneSlice*(j+1), oneStack*(i+1), 1);
                ss << (j+1)*textSlice << " " << 1-(i+1+(stacks/2.0f))*textStack << endl;

                ss << createSpherePoint(oneSlice*j, oneStack*i, radius);
                ss << createSpherePoint(oneSlice*j, oneStack*i, 1);
                ss << (j)*textSlice << " " << 1-(i+(stacks/2.0f))*textStack << endl;

                ss << createSpherePoint(oneSlice*(j+1), oneStack*i, radius);
                ss << createSpherePoint(oneSlice*(j+1), oneStack*i, 1);
                ss << (j+1)*textSlice << " " << 1-(i+(stacks/2.0f))*textStack << endl;
            }
        }
    }

    return ss.str();
}

string createReverseSphere(float radius, int slices, int stacks){
    stringstream ss;

    if(slices < 3 || stacks < 2){
        return string();
    }

    int nPontos = (6 * slices * stacks) - (6 * slices);

    ss << nPontos << endl;

    float oneSlice = (float) (2 * M_PI)/slices, oneStack = (float) (M_PI)/stacks;
    float textSlice = 1.0f / slices, textStack = 1.0f / stacks;

    for(float i = -(stacks/2.0f); i < (stacks/2.0f); i++){
        for(int j = 0; j < slices; j++){
            if(i != (stacks/2.0f)-1){
                ss << createSpherePoint(oneSlice*j, oneStack*i, radius);
                ss << createSpherePoint(oneSlice*j, oneStack*i, 1);
                ss << (j)*textSlice << " " << 1-(i+(stacks/2.0f))*textStack << endl;

                ss << createSpherePoint(oneSlice*j, oneStack*(i+1), radius);
                ss << createSpherePoint(oneSlice*j, oneStack*(i+1), 1);
                ss << (j)*textSlice << " " << 1-(i+1+(stacks/2.0f))*textStack << endl;

                ss << createSpherePoint(oneSlice*(j+1), oneStack*(i+1), radius);
                ss << createSpherePoint(oneSlice*(j+1), oneStack*(i+1), 1);
                ss << (j+1)*textSlice << " " << 1-(i+1+(stacks/2.0f))*textStack << endl;
            }

            if(i != -(stacks/2.0f)){
                ss << createSpherePoint(oneSlice*(j+1), oneStack*(i+1), radius);
                ss << createSpherePoint(oneSlice*(j+1), oneStack*(i+1), 1);
                ss << (j+1)*textSlice << " " << 1-(i+1+(stacks/2.0f))*textStack << endl;

                ss << createSpherePoint(oneSlice*(j+1), oneStack*i, radius);
                ss << createSpherePoint(oneSlice*(j+1), oneStack*i, 1);
                ss << (j+1)*textSlice << " " << 1-(i+(stacks/2.0f))*textStack << endl;

                ss << createSpherePoint(oneSlice*j, oneStack*i, radius);
                ss << createSpherePoint(oneSlice*j, oneStack*i, 1);
                ss << (j)*textSlice << " " << 1-(i+(stacks/2.0f))*textStack << endl;
            }
        }
    }

    return ss.str();
}

/*
 * Tranforma coordenadas polares em cartesianas.
 *
 * Recebe o alpha, o raio e a altura do ponto.
 *
 * Retorna uma string com a coordenadas cartesianas do ponto.
 */
string createConePoint(float alpha, float radius, float height){
    float x,y,z;
    stringstream ss;

    x = radius * sin(alpha);
    y = height;
    z = radius * cos(alpha);

    ss << x << " " << y << " " << z << endl;

    return ss.str();
}

void normalize(float *a) {

    float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}

void cross(float *a, float *b, float *res) {

    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}

string createConeNormal(int i, int j, double oneSlice, double oneStack, float radius, float smallRadius){
    float a[3] = {0,0,0};
    float v1[3] = {0,0,0};
    float v2[3] = {0,0,0};
    stringstream ss;

    v1[0] = radius*sin(oneSlice*(i+1)) - radius*sin(oneSlice*(i-1));
    v1[1] = 0;
    v1[2] = radius*cos(oneSlice*(i+1)) - radius*cos(oneSlice*(i-1));

    v2[0] = smallRadius*sin(oneSlice*i) - radius*sin(oneSlice*i);
    v2[1] = oneStack*(j+1)-oneStack*j;
    v2[2] = smallRadius*cos(oneSlice*i) - radius*cos(oneSlice*i);

    cross(v1,v2,a);

    normalize(a);
    ss << a[0] << " " << a[1] << " " << a[2] << endl;

    return ss.str();
}

/*
 * Calcula os pontos de um cone construido por triangulos
 *
 * Recebe o raio da base, a altura, o numero de fatias e o numero de andares do cone.
 *
 * Retorna uma string com todos os pontos do cone.
 *
 * D--C
 * |  |
 * A--B
 */
string createCone(double radius, double height, int slices, int stacks){
    if(slices < 3 || stacks < 1) return string();

    stringstream ss;
    int nPontos = (6 * slices * stacks);

    ss << nPontos << endl;//Numero de vertices

    double oneSlice=(2*M_PI)/slices, oneStack=height/stacks, oneRadius=radius/stacks, smallRadius, radiusAux=radius;
    float textSlice = 1.0f / slices, textStack = 1.0f / (stacks+1);
    int j = 0, i = 0;

    for (j = 0; j < stacks; ++j){
        smallRadius= radiusAux-oneRadius;
        for(i = 0; i < slices; i++){
            if(j == 0){
                //Base
                //A
                ss << createConePoint(oneSlice*i, radiusAux, oneStack*j);
                ss << "0 -1 0" << endl;
                ss << i*textSlice << " " << 1-(j+1)*textStack << endl;

                //Origem
                ss << "0 0 0" << endl;
                ss << "0 -1 0" << endl;
                ss << i*textSlice << " 1" << endl;

                //B
                ss << createConePoint(oneSlice*(i+1), radiusAux, oneStack*j);
                ss << "0 -1 0" << endl;
                ss << (i+1)*textSlice << " " << 1-(j+1)*textStack << endl;
            }
            //Face
            if(j!=stacks-1){
                //A
                ss << createConePoint(oneSlice*i, radiusAux, oneStack*j);
                ss << createConeNormal(i, j, oneSlice, oneStack, radiusAux, smallRadius);
                ss << i*textSlice << " " << 1-(j+1)*textStack << endl;


                //C
                ss << createConePoint(oneSlice*(i+1), smallRadius, oneStack*(j+1));
                ss << createConeNormal(i+1, j+1, oneSlice, oneStack, radiusAux, smallRadius);
                ss << (i+1)*textSlice << " " << 1-(j+2)*textStack << endl;

                //D
                ss << createConePoint(oneSlice*i, smallRadius, oneStack*(j+1));
                ss << createConeNormal(i, j+1, oneSlice, oneStack, radiusAux, smallRadius);
                ss << (i)*textSlice << " " << 1-(j+2)*textStack << endl;
            }
            //C
            ss << createConePoint(oneSlice*(i+1), smallRadius, oneStack*(j+1));
            ss << createConeNormal(i+1, j+1, oneSlice, oneStack, radiusAux, smallRadius);
            ss << (i+1)*textSlice << " " << 1-(j+2)*textStack << endl;

            //A
            ss << createConePoint(oneSlice*i, radiusAux, oneStack*j);
            ss << createConeNormal(i, j, oneSlice, oneStack, radiusAux, smallRadius);
            ss << i*textSlice << " " << 1-(j+1)*textStack << endl;

            //B
            ss << createConePoint(oneSlice*(i+1), radiusAux, oneStack*j);
            ss << createConeNormal(i+1, j, oneSlice, oneStack, radiusAux, smallRadius);
            ss << (i+1)*textSlice << " " << 1-(j+1)*textStack << endl;
        }
        radiusAux = smallRadius;
    }

    return ss.str();
}

void multMatrixVector(float *m, int l, float *v, float *res) {
    for(int j = 0; j < l; ++j){
        res[j] = 0;
        for(int k = 0; k < 4; ++k){
            res[j] += v[k] * m[j * 4 + k];
        }
    }
}

void multVectorMatrix(float *v, float *m, float *res) {
    for(int j = 0; j < 4; ++j){
        res[j] = 0;
        for(int k = 0; k < 4; ++k){
            res[j] += v[k] * m[k * 3 + j + k];
        }
    }
}

void fillBigP(int p, float bigPX[], float bigPY[], float bigPZ[]){
    vector<int> indexAux = indexes.at(p);
    for (int i = 0; i < 16; ++i) {
        istringstream ss(points.at(indexAux.at(i)));
        string s;

        getline(ss, s, ',');
        bigPX[i]=strtof(s.c_str(),0);

        getline(ss, s, ',');
        bigPY[i]=strtof(s.c_str(),0);

        getline(ss, s, ',');
        bigPZ[i]=strtof(s.c_str(),0);
    }
}

string getBezier(float ru, float rv, int p){
    float v = 1.0 * rv / (resV-1);
    float u = 1.0 * ru / (resU-1);

    float bigPX[4*4];
    float bigPY[4*4];
    float bigPZ[4*4];

    float bigBX[1];
    float bigBY[1];
    float bigBZ[1];

    float part1[4], part2[4];
    float bigU[] = {1, u, u*u, u*u*u};
    float bigV[] = {1, v, v*v, v*v*v};

    float bigM[] = {   1, 0, 0,0,
                      -3, 3, 0,0,
                       3,-6, 3,0,
                      -1, 3,-3,1};
    float bigMTrans[] = {  1,-3, 3,-1,
                           0, 3,-6, 3,
                           0, 0, 3,-3,
                           0, 0, 0, 1};

    multVectorMatrix(bigU, bigM, part1);
    multMatrixVector(bigMTrans, 4, bigV, part2);

    fillBigP(p, bigPX, bigPY, bigPZ);

    float auxX[4];
    float auxY[4];
    float auxZ[4];

    multMatrixVector(bigPX, 4, part2, auxX);
    multMatrixVector(bigPY, 4, part2, auxY);
    multMatrixVector(bigPZ, 4, part2, auxZ);

    multMatrixVector(part1, 1, auxX, bigBX);
    multMatrixVector(part1, 1, auxY, bigBY);
    multMatrixVector(part1, 1, auxZ, bigBZ);

    stringstream ss;
    ss << bigBX[0] << " " << bigBY[0] << " " << bigBZ[0] << endl;

    //Normal
    float bigUDeriv[] = {0, 1, 2*u, 3*u*u};
    float bigVDeriv[] = {0, 1, 2*v, 3*v*v};

    //Derivada U
    float bigBXDerivU[1];
    float bigBYDerivU[1];
    float bigBZDerivU[1];

    multVectorMatrix(bigUDeriv, bigM, part1);
    multMatrixVector(bigMTrans, 4, bigV, part2);

    multMatrixVector(bigPX, 4, part2, auxX);
    multMatrixVector(bigPY, 4, part2, auxY);
    multMatrixVector(bigPZ, 4, part2, auxZ);

    multMatrixVector(part1, 1, auxX, bigBXDerivU);
    multMatrixVector(part1, 1, auxY, bigBYDerivU);
    multMatrixVector(part1, 1, auxZ, bigBZDerivU);

    //Derivada V
    float bigBXDerivV[1];
    float bigBYDerivV[1];
    float bigBZDerivV[1];

    multVectorMatrix(bigU, bigM, part1);
    multMatrixVector(bigMTrans, 4, bigVDeriv, part2);

    multMatrixVector(bigPX, 4, part2, auxX);
    multMatrixVector(bigPY, 4, part2, auxY);
    multMatrixVector(bigPZ, 4, part2, auxZ);

    multMatrixVector(part1, 1, auxX, bigBXDerivV);
    multMatrixVector(part1, 1, auxY, bigBYDerivV);
    multMatrixVector(part1, 1, auxZ, bigBZDerivV);

    //Cross
    float bigBDeriv[3];
    float derivU[3];
    float derivV[3];

    derivU[0] = bigBXDerivU[0];
    derivU[1] = bigBYDerivU[0];
    derivU[2] = bigBZDerivU[0];

    derivV[0] = bigBXDerivV[0];
    derivV[1] = bigBYDerivV[0];
    derivV[2] = bigBZDerivV[0];

    cross(derivU,derivV, bigBDeriv);
    ss << bigBDeriv[0] << " " << bigBDeriv[1] << " " << bigBDeriv[2] << endl;

    //Textura
    ss << u << " " << v << endl;

    return ss.str();
}

string createBezierPatches(){
    stringstream ss1;
    stringstream ss2;
    int pointscount = 0;

    for (int p = 0; p < nPatches; p++) {
        for (int u = 0; u < resU-1; u++) {
            for (int v = 0; v < resV-1; v++) {
                ss2 << getBezier(u, v, p);
                ss2 << getBezier(u, v+1, p);
                ss2 << getBezier(u+1, v, p);

                ss2 << getBezier(u, v+1, p);
                ss2 << getBezier(u+1, v+1, p);
                ss2 << getBezier(u+1, v, p);

                pointscount += 6;
            }
        }
    }

    ss1 << pointscount << endl;
    ss1 << ss2.str();

    return ss1.str();
}

string loadPatch(char* file, int tessLevel){
    string line;
    ifstream myFile(file);

    if (myFile.is_open()){
        getline(myFile,line);
        const char* str = line.c_str();
        nPatches = atoi(str);

        for(int i = 0; i < nPatches; ++i){
            vector<int> indexPatch;
            getline(myFile,line);
            istringstream ss(line);
            string s;

            while(getline(ss,s,',')){
                indexPatch.push_back(atoi(s.c_str()));
            }
            indexes.push_back(indexPatch);
        }

        getline(myFile,line);
        str = line.c_str();
        nPoints = atoi(str);

        for(int i = 0; i < nPoints; ++i){
            getline(myFile,line);
            points.push_back(line);
        }
        myFile.close();

        resU=tessLevel;
        resV=tessLevel;

        return createBezierPatches();
    }
    else{
        cout << "Unable to open patch file" << endl;
    }

    return "";
}

/*
 *   C-D
 *  /   \
 * A-----B
 */
string createRing(float innerRadius, float outerRadius, int slices){
    stringstream ss;

    ss << 6*slices << endl;

    double oneSlice=(2*M_PI)/slices;
    float textSlice = 1.0f / slices;

    for (int i = 0; i < slices; ++i){
        //A
        ss << createConePoint(oneSlice*i, outerRadius, 0);
        ss << "0 1 0" << endl;
        ss << "1 0" << endl;
        //D
        ss << createConePoint(oneSlice*(i+1), innerRadius, 0);
        ss << "0 1 0" << endl;
        ss << "0 1" << endl;
        //C
        ss << createConePoint(oneSlice*i, innerRadius, 0);
        ss << "0 1 0" << endl;
        ss << "0 0" << endl;

        //A
        ss << createConePoint(oneSlice*i, outerRadius, 0);
        ss << "0 1 0" << endl;
        ss << "1 0" << endl;
        //B
        ss << createConePoint(oneSlice*(i+1), outerRadius, 0);
        ss << "0 1 0" << endl;
        ss << "1 1" << endl;
        //D
        ss << createConePoint(oneSlice*(i+1), innerRadius, 0);
        ss << "0 1 0" << endl;
        ss << "0 1" << endl;
    }

    return ss.str();
}

int main(int argc, char** argv){

    if(argc<2){
        printf("Required type of shape.\n");
        exit(EXIT_FAILURE);
    }

    string shape = string(argv[1]);
    ofstream file;

    if(shape.compare("Plane")==0 || shape.compare("plane")==0){
        if(argc<3){
            printf("Required dimension.\n");
            exit(EXIT_FAILURE);
        }
        if(argc<4){
            printf("Required file name.\n");
            exit(EXIT_FAILURE);
        }

        file.open(argv[3]);
        file << "6\n";//Numero de vertices
        file << createPlane(0,0,0,atof(argv[2]),0,atof(argv[2]),1);

        return 0;
    }

    if(shape.compare("Box")==0 || shape.compare("box") ==0 ){
        if(argc == 6){
            file.open(argv[5]);
            file << createBox(atof(argv[2]), atof(argv[3]), atof(argv[4]), 1, 1, 1);
        }
        else{
            if(argc<6){
                printf("Use \"Box <X> <Y> <Z> <<slice> <stacks> <depthDivisions>>? \".\n");
                exit(EXIT_FAILURE);
            }
            if(argc == 9){
                file.open(argv[8]);
                file << createBox(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));
            } else {
                if(argc<8){
                    printf("Required slices,stacks,depthDivisions numbers.\n");
                    exit(EXIT_FAILURE);
                }
                if(argc==8){
                    printf("Required file name.\n");
                    exit(EXIT_FAILURE);
                }
            }
        }

        return 0;
    }

    if(shape.compare("Sphere")==0 || shape.compare("sphere")==0){
        if(argc<5){
            printf("Required Radius, Slices and Stacks.\n");
            exit(EXIT_FAILURE);
        }
        if(argc<6){
            printf("Required file name.\n");
            exit(EXIT_FAILURE);
        }

        file.open(argv[5]);
        file << createSphere(atof(argv[2]), atof(argv[3]), atof(argv[4]));

        return 0;
    }

    if(shape.compare("ReverseSphere")==0 || shape.compare("reversesphere")==0){
        if(argc<5){
            printf("Required Radius, Slices and Stacks.\n");
            exit(EXIT_FAILURE);
        }
        if(argc<6){
            printf("Required file name.\n");
            exit(EXIT_FAILURE);
        }

        file.open(argv[5]);
        file << createReverseSphere(atof(argv[2]), atof(argv[3]), atof(argv[4]));

        return 0;
    }

    if(shape.compare("Cone")==0 || shape.compare("cone")==0){
        if(argc<6){
            printf("Required Radius, Height, Slices and Stacks.\n");
            exit(EXIT_FAILURE);
        }
        if(argc<7){
            printf("Required file name.\n");
            exit(EXIT_FAILURE);
        }

        file.open(argv[6]);
        file << createCone(atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));

        return 0;
    }

    if(shape.compare("Bezier")==0 || shape.compare("bezier")==0){
        if(argc<4){
            printf("Required Bezier Patches file and tessellation level.\n");
            exit(EXIT_FAILURE);
        }

        if(argc<5){
            printf("Required output file name.\n");
            exit(EXIT_FAILURE);
        }
        file.open(argv[4]);
        file << loadPatch(argv[2], atoi(argv[3]));

        return 0;
    }

    if(shape.compare("Ring")==0 || shape.compare("ring")==0){ //.Generator ring inner outter slices file
        if(argc<5){
            printf("Required inner Radius, outer radius and Slices.\n");
            exit(EXIT_FAILURE);
        }

        if(argc<6){
            printf("Required output file name.\n");
            exit(EXIT_FAILURE);
        }

        file.open(argv[5]);
        file << createRing(atof(argv[2]), atof(argv[3]), atof(argv[4]));

        return 0;
    }
    else{
        printf("Unrecognized shape type.\n");
        exit(EXIT_FAILURE);
    }
}