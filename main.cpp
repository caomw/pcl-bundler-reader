#include "pointcloudscene.h"

int main(int argc, char * argv[]){

    if (argc != 2){
        std::cerr << "Wrong number of input parameteres" << std::endl;
    }

    PointCloudScene scene;
    scene.bundlerReader(argv[1]);

    scene.writeMesh("test.ply");


    return 0;
}

