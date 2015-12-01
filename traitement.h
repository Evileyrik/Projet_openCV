#include <stdio.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"

using namespace cv;


Mat detectionFeature (Mat);

 std::vector< DMatch > matcher (Mat,Mat);

 std::vector<KeyPoint> getPointCle (Mat );

 Mat matriceFondamentale (vector<KeyPoint> ,vector<KeyPoint> ,vector<DMatch>);

