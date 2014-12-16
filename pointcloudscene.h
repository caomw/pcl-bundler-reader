#ifndef POINTCLOUDSCENE_H
#define POINTCLOUDSCENE_H

#include <iostream>
#include <stdio.h>

#include <pcl/io/ply_io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/features/normal_3d.h>


#include "pointXYZRGBNormalCam.h"
#include "camera.h"


class PointCloudScene{

public:
    PointCloudScene();
    ~PointCloudScene();


    // I/O functions
    void bundlerReader(std::string _fileName);
    void bundlerPointReader(PointXYZRGBNormalCam& _point, std::ifstream& _stream);
    void writeMesh(std::string _fileName);

    // Estimate the normals of a cloud and store them
    void estimateNormals();

    // Fix the normal orientation using the camera position
    void fixNormals();



private:
    PointCloud<PointXYZRGBNormalCam>::Ptr pointCloud_;
    std::vector<Camera> cameras_;
    unsigned int nCameras_;
};

#endif // POINTCLOUDSCENE_H
