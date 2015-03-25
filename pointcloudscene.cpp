#define PCL_NO_PRECOMPILE // This is needed for using custom Point types in PCL

#include <pcl/features/normal_3d.h>
#include <boost/tokenizer.hpp>

#include "pointcloudscene.h"

PointCloudScene::PointCloudScene(){

    cameras_.clear();
    nCameras_ = 0;
    normalsFlag_ = false;

}

PointCloudScene::~PointCloudScene(){

}


void PointCloudScene::bundlerReader(const std::string& _fileName){

    std::cerr << "Reading Bundler file..." << std::endl;

    std::ifstream inputFile(_fileName);
    std::string line;

    int nPoints = 0;
    PointCloud<PointXYZRGBNormalCam>::Ptr cloud (new PointCloud<PointXYZRGBNormalCam>);

    if (inputFile.is_open()){

        // We avoid all possible comments in the firsts lines, and also empty lines
        do {
            std::getline(inputFile, line);
            while (line.empty()) std::getline(inputFile, line);
        } while (line.at(0) == '#');

        // First, number of cameras and number of points in the input point cloud
        boost::tokenizer<> tokens(line);
        boost::tokenizer<>::iterator tit = tokens.begin();
        std::stringstream ss;
        ss << *tit;
        ss >> nCameras_;
        ++tit;

        // stringstream is cleared. It could also be ss.str("")
        ss.str(std::string());
        ss.clear();

        ss << *tit;
        ss >> nPoints;

        ss.str(std::string());
        ss.clear();

        // Now we read the camera information
        for (unsigned int i = 0; i < nCameras_; i++){
            Camera camera;
            camera.readCamera(inputFile);

            cameras_.push_back(camera);
        }

        // Now we read geometry information
        for (unsigned int i = 0; i < nPoints; i++){

            PointXYZRGBNormalCam point;
            bundlerPointReader(point, inputFile);

            cloud->push_back(point);

        }

        pointCloud_ = cloud;

        inputFile.close();

    } else {
        std::cerr << "Unable to open Bundle file" << std::endl;
    }

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

void PointCloudScene::writeMesh(const std::string& _fileName){

    io::savePLYFile(_fileName, *pointCloud_);

}

bool PointCloudScene::calculateNormals(){
    return normalsFlag_;
}

void PointCloudScene::activateNormalsFlag(){
    normalsFlag_ = true;
}

void PointCloudScene::deactivateNormalsFlag(){
    normalsFlag_ = false;
}

void PointCloudScene::estimateNormals(){

    std::cerr << "Estimating normals..." << std::endl;

    NormalEstimation<PointXYZRGBNormalCam, PointXYZRGBNormalCam> ne;
    ne.setInputCloud(pointCloud_);

    search::KdTree<PointXYZRGBNormalCam>::Ptr tree (new search::KdTree<PointXYZRGBNormalCam> ());
    ne.setSearchMethod (tree);
    ne.setRadiusSearch (0.5);

    ne.compute (*pointCloud_);

}

void PointCloudScene::fixNormals(){

    std::cerr << "Fixing wrong normal orientation..." << std::endl;

    for (unsigned int i = 0; i < pointCloud_->points.size(); i++){
        PointXYZRGBNormalCam current = pointCloud_->points[i];
        const Eigen::Vector3f cur_pos = current.getArray3fMap();
        const Eigen::Vector3f cam_pos = cameras_[current.camera].getCameraPosition();

        const Eigen::Vector3f cam_dir = cam_pos - cur_pos;
        const Eigen::Vector3f normal  = current.getNormalVector3fMap();

        if (cam_dir.dot(normal) < 0){
            pointCloud_->points[i].normal_x = -current.normal_x;
            pointCloud_->points[i].normal_y = -current.normal_y;
            pointCloud_->points[i].normal_z = -current.normal_z;
        }
    }

}
