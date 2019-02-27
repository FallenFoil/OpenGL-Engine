#include <iostream>
#include <fstream>


void createpoints(float  x,float y, float z, std::ofstream &outfile){


    outfile << "(" << -x << "," << 0 << "," << z << ")\n" <<



    "(" << x << "," << 0 << "," << z << ")\n" <<



    "(" << x << "," << 0 << "," << -z << ")\n" <<



    "(" << -x << "," << 0 << "," << -z << ")\n" <<



    "(" << -x << "," << y << "," << z << ")\n" <<



    "(" << x << "," << y << "," << z << ")\n" <<


    "(" << x << "," << y << "," << -z << ")\n" <<


    "(" << -x << "," << y << "," << -z << ")\n";

}




int main(int argc, char** argv){
    if(argc<2){
        perror("Wrong input!\nSpecify the shape you want to draw.\n");
        __error();
    }

    std::string str(argv[1]);

    if(str.compare("Box")==0){
        if(argc<6){
            perror("Requires X, Y and Z dimensions and the file path.\n");
            __error();
        }
    float x = atof(argv[2])/2;
    float y = atof(argv[3])/2;
    float z = atof(argv[4])/2;


    //create the file path
    std::ofstream outfile(argv[5]);
    outfile << "3\nxyz" << std::endl;

    createpoints(x,y,z,outfile);

    outfile.close();
    }



    return 0;
}