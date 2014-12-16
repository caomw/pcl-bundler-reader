# PCL Bundler Reader

This project reads a Bundler file (*.out) and creates a PCL scene containing a the set of cameras described in the Bundler file, plus the shape-from-motion based point cloud.

### What is Bundler?

Bundler is a structure-from-motion system for unordered image collections. You can find further information [here] (https://github.com/snavely/bundler_sfm).


### Why this converter?

Bundler is used for reconstructing 3D scenes in many scenarios. Its output file contains a point cloud and a set of parameters corresponding to all the cameras used for the reconstruction. On the other hand, PCL (Point Cloud Library) is becoming a standard when dealing with point clouds; however, PCL does not provide any parser for Bundler files.

### Running the code

The system takes one parameter: the Bundler file containing the 3D scene.

### PointXYZRGBNormalCam??

Yeah… This has a reason.
The system gives you the possibility of estimating point normals through a least-square plane fitting estimation problem. However, there is no mathematical way to solve the sign of the estimated normal, so we have to do something else. As Bundler provides a list of all the cameras which “see” every point, we select the best one and store it as a property of the point (together with position, color, and normal). This way, we can determine if the normal is facing the camera or not and, of course, correct it.


 
