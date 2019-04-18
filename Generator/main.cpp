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
 */
string createPlane(float oX, float oY, float oZ, float sizeX, float sizeY, float sizeZ, int direction){
    float x, y=0, z;
    stringstream ss;
    int dirX=direction, dirY=direction, dirZ=direction;

    if(sizeX==0){dirX=1;}
    if(sizeY==0){dirY=1;}
    if(sizeZ==0){dirZ=1;}


    //Triangulo esquerdo
    ss << "##Strips" << endl;


    //A
    x = (oX-sizeX/2)*dirX; y = (oY-sizeY/2)*dirY; z = (oZ+sizeZ/2)*dirZ;
    Ponto a = Ponto(x,y,z);
    ss << a.toString() << endl;
    //C
    //x = (oX+sizeX/2)*dirX; y = (oY+sizeY/2)*dirY; z = (oZ-sizeZ/2)*dirZ;
    //Ponto c = Ponto(x,y,z);
    //ss << c.toString() << endl;
    //D
    x = oX-sizeX/2; y = oY+sizeY/2; z = oZ-sizeZ/2;
    if(sizeX==0){z = sizeZ/2;}
    Ponto d = Ponto(x,y,z);
    ss << d.toString() << endl;

    //Triangulo direito

    //B
    x = oX+sizeX/2; y = oY-sizeY/2; z = oZ+sizeZ/2;
    if(sizeX==0){z = -sizeZ/2;}
    Ponto b = Ponto(x,y,z);
    ss << b.toString() << endl;
    //C
    x = (oX+sizeX/2)*dirX; y = (oY+sizeY/2)*dirY; z = (oZ-sizeZ/2)*dirZ;
    Ponto c = Ponto(x,y,z);
    ss << c.toString() << endl;
    //A
    //x = (oX-sizeX/2)*dirX; y = (oY-sizeY/2)*dirY; z = (oZ+sizeZ/2)*dirZ;
    //a = Ponto(x,y,z);
    //ss << a.toString() << endl;
   

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
    Ponto a = Ponto(x,y,z);

    //ss << a.toString() << endl;
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

    ss << "##Strips" << endl;

    float oneSlice = (float) (2 * M_PI)/slices, oneStack = (float) (M_PI)/stacks;

    for(float i = -(stacks/2.0f); i < (stacks/2.0f); i++){
        for(int j = 0; j < slices; j++){

                //algoritmo da strip 
                ss << createSpherePoint(oneSlice*j, oneStack*(i+1), radius);//A
                ss << createSpherePoint(oneSlice*j, oneStack*i, radius);//C
        }

        ss << createSpherePoint(0, oneStack*(i+1), radius);//A
        ss << createSpherePoint(0, oneStack*i, radius);//C
            
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

    Ponto a = Ponto(x,y,z);

    ss << a.toString() << endl;

    return ss.str();
}

/*
 * Calcula os pontos de um cone construido por triangulos
 *
 * Recebe o raio da base, a altura, o numero de fatias e o numero de andares do cone.
 *
 * Retorna uma string com todos os pontos do cone.
 */
string createCone(double radius, double height, int slices, int stacks){
    if(slices < 3 || stacks < 3) return string();

    stringstream ss;
    int nPontos = (6 * slices * stacks);

    ss << nPontos << endl;//Numero de vertices
    
    double oneSlice=(2*M_PI)/slices, oneStack=height/stacks, oneRadius=radius/stacks, smallRadius, radiusAux=radius;
    int j = 0, i = 0;

    ss << "##FAN";
    //Origem da base

    //fan
	ss << " 0 0 0" << endl;
	for(i=0;i<slices;i++){
		ss << createConePoint(oneSlice*i, radiusAux, 0);	
	}
	createConePoint(0,radiusAux,0);	

    ss << "##STRIP";

    for (int j = 0; j < stacks; j++){
        smallRadius= radiusAux-oneRadius;
        for(i = 0; i < slices; i++){
           
            //Face
            
                //A
                ss << createConePoint(oneSlice*i, smallRadius, oneStack*(j+1));
                //C
                ss << createConePoint(oneSlice*i, radiusAux, oneStack*j);

            }
           
            //A
            ss << createConePoint(oneSlice*i, radiusAux, oneStack*(j+1));
            //B
            ss << createConePoint(oneSlice*i, radiusAux, oneStack*j);
            
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

    float bigM[] = {  1, 0, 0,0,
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
    ss << bigBX[0] << " " << bigBY[0] << " " << bigBZ[0];

    return ss.str();
}

string createBezierPatches(){
    stringstream ss1;
    stringstream ss2;
    int pointscount = 0;
    ss2 << "##Triangles" << endl;

    for (int p = 0; p < nPatches; p++) {
        for (int u = 0; u < resU-1; u++) {
            for (int v = 0; v < resV-1; v++) {
                ss2 << getBezier(u, v, p) << endl;
                ss2 << getBezier(u, v+1, p) << endl;
                ss2 << getBezier(u+1, v, p) << endl;

                ss2 << getBezier(u, v+1, p) << endl;
                ss2 << getBezier(u+1, v+1, p) << endl;
                ss2 << getBezier(u+1, v, p) << endl;
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
        cout << "Unable to open patch file";
    }

    return "";
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
    }
    else{
        if(argc<3){
            printf("Required Bezier Patches file and tessellation level.\n");
            exit(EXIT_FAILURE);
        }

        if(argc<4){
            printf("Required output file name.\n");
            exit(EXIT_FAILURE);
        }
        file.open(argv[3]);
        file << loadPatch(argv[1], atoi(argv[2]));
    }
    return 0;
}