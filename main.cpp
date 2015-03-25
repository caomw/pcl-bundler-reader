#include <boost/program_options.hpp>
#include "pointcloudscene.h"


int main(int argc, char * argv[]){

    PointCloudScene scene;


    // Parsing the command line arguments
    try {

        namespace po = boost::program_options;

        po::options_description desc("Options");
        desc.add_options()
                ("help,h", "Print this help message")
                ("normals,n", "Activate normal estimation and correction");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cerr << desc << std::endl;
            return 0;
        }

        if (vm.count("normals")) {
            std::cerr << "Normals will be estimated" << std::endl;
            scene.activateNormalsFlag();
        }


    }

    catch(std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        return 1;
    }

    catch(...) {
        std::cerr << "Exception of unknown type!" << std:: endl;
    }


    // Last argument is the input file name
    scene.bundlerReader(argv[argc-1]);

    if (scene.calculateNormals()){
        scene.estimateNormals();
        scene.fixNormals();
    }

    scene.writeMesh("output.ply");


    return 0;
}

