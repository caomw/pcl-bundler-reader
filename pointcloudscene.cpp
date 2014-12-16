#include "pointcloudscene.h"

PointCloudScene::PointCloudScene(){

    pointClouds_.clear();
    nClouds_ = 0;

    cameras_.clear();
    nCameras_ = 0;

}

PointCloudScene::~PointCloudScene(){

}


void PointCloudScene::bundlerReader(std::string _fileName){

}

void PointCloudScene::bundlerPointReader(PointXYZRGBNormalCam& _point, std::ifstream& _stream){

    std::stringstream ss;
    std::string line;

    for (unsigned int point_line = 0; point_line < 3; point_line++){

        std::getline(_stream, line);
        while (line.empty()) std::getline(_stream, line);

        boost::tokenizer<boost::char_separator<char> > point_tokens(line, boost::char_separator<char>(" "));
        boost::tokenizer<boost::char_separator<char> >::iterator ptit = point_tokens.begin();

        // This line has the position of the point
        if (point_line == 0){
            unsigned int index = 0;
            float xyz[3];
            for (; ptit  != point_tokens.end(); ++ptit, index++){
                float value;
                ss << *ptit;
                ss >> value;
                ss.str(std::string());
                ss.clear();
                xyz[index] = value;
            }
            _point.x = xyz[0];
            _point.y = xyz[1];
            _point.z = xyz[2];
        }
        // This line has the color of the point
        else if (point_line == 1){
            unsigned int index = 0;
            unsigned char rgb[3];
            for (; ptit != point_tokens.end(); ++ptit, index++){
                float value;
                ss << *ptit;
                ss >> value;
                ss.str(std::string());
                ss.clear();
                rgb[index] = value;
            }
            _point.r = rgb[0];
            _point.g = rgb[1];
            _point.b = rgb[2];
        }

        // This line sets the camera correspondances with each point in the cloud
        else {
            unsigned int nCam;
            ss << *ptit;
            ss >> nCam;
            ss.str(std::string());
            ss.clear();
            ++ptit;

            if (nCam > 0){
                int cam_value;
                ss << *ptit;
                ss >> cam_value;
                ss.str(std::string());
                ss.clear();
                _point.camera = cam_value;
            } else {
                _point.camera = -1;
            }
        }
    }
}

void PointCloudScene::writeMesh(std::string _fileName){

}
