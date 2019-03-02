#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "Ponto.h"
#include <sstream>

using namespace std;

string createBox(float width, float height, float depth,  int stacks, int slices, int depthDivisions){
    if(stacks < 1 && slices < 1) return string();

    stringstream ss;
    ss << stacks * slices * depthDivisions*6*6 << "\n";
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

                ss << toStringPlane(t1,t2,t3,t4);
                ss << toStringPlane(b3,b2,b1,b4);
                ss << toStringPlane(b4,b1,t1,t4);
                ss << toStringPlane(t2,b2,b3,t3);
                ss << toStringPlane(b2,t2,t1,b1);
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
 * Cria e retorna uma string com o numero de vertices e as coordenadas desses vertices.
 *
 * (oX,oY,oZ) são as coordenadas do centro do plano.
 * size(X,Y,Z) são o tamanho do plano no eixo. Se for 0 significa que todas a coordenadas são 0 nesse eixo.
 * direction é a direção do plano, varia entre 1 e -1. 1 é um plano normal e -1 é o inverso.
 */
string createPlane(double oX, double oY, double oZ, double sizeX, double sizeY, double sizeZ, int direction){
    double x, y=0, z;
    stringstream ss;
    int dirX=direction, dirY=direction, dirZ=direction;

    if(sizeX==0){dirX=1;}
    if(sizeY==0){dirY=1;}
    if(sizeZ==0){dirZ=1;}

    /*
     * D--C
     * |  |
     * A--B
     */


    //Triangulo esquerdo

    //A
    x = (oX-sizeX/2)*dirX; y = (oY-sizeY/2)*dirY; z = (oZ+sizeZ/2)*dirZ;
    ss << x << " " << y << " " << z << endl;
    //C
    x = (oX+sizeX/2)*dirX; y = (oY+sizeY/2)*dirY; z = (oZ-sizeZ/2)*dirZ;
    ss << x << " " << y << " " << z << endl;
    //D
    x = oX-sizeX/2; y = oY+sizeY/2; z = oZ-sizeZ/2;
    if(sizeX==0){z = sizeZ/2;}
    ss << x << " " << y << " " << z << endl;

    //Triangulo direito

    //C
    x = (oX+sizeX/2)*dirX; y = (oY+sizeY/2)*dirY; z = (oZ-sizeZ/2)*dirZ;
    ss << x << " " << y << " " << z << endl;
    //A
    x = (oX-sizeX/2)*dirX; y = (oY-sizeY/2)*dirY; z = (oZ+sizeZ/2)*dirZ;
    ss << x << " " << y << " " << z << endl;
    //B
    x = oX+sizeX/2; y = oY-sizeY/2; z = oZ+sizeZ/2;
    if(sizeX==0){z = -sizeZ/2;}
    ss << x << " " << y << " " << z << endl;

    return ss.str();
}

/*
 * Cria e retorna uma string com os pontos de uma Box, criando cada plano.
 *
 * Recebe os tamnahos da Box.
 */
/*string createBox(double sizeX, double sizeY, double sizeZ){
    stringstream ss;

    ss << createPlane(0,-sizeY/2,0,sizeX,0,sizeZ,-1);
    ss << createPlane(0, sizeY/2,0,sizeX,0,sizeZ, 1);
    ss << createPlane(0,0,-sizeZ/2,sizeX,sizeY,0,-1);
    ss << createPlane(0,0, sizeZ/2,sizeX,sizeY,0, 1);
    ss << createPlane(-sizeX/2,0,0,0,sizeY,sizeZ,-1);
    ss << createPlane( sizeX/2,0,0,0,sizeY,sizeZ, 1);

    return ss.str();
}*/

int main(int argc, char** argv){

    if(argc<2){
        printf("Required type of shape.\n");
        exit(EXIT_FAILURE);
    }

    string shape = string(argv[1]);
    ofstream file;

    if(shape.compare("Plane")==0){
        if(argc<3){
            printf("Required dimension.\n");
            exit(EXIT_FAILURE);
        }
        if(argc<4){
            printf("Required file name.\n");
            exit(EXIT_FAILURE);
        }

        file.open(argv[3]);
        file << "4\n";
        file << createPlane(0,0,0,atof(argv[2]),0,atof(argv[2]),1);
    }
    if(shape.compare("Box")==0){
        if(argc == 6){
            file.open(argv[5]);
            file << createBox(atof(argv[2]), atof(argv[3]), atof(argv[4]), 1, 1, 1);
        }
        else{
            if(argc<6){
                printf("Required X,Y,Z dimensions.\n");
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
    }
    return 0;
}