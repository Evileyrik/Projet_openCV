#include <stdio.h>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;


Mat detectionFeature (Mat);

std::vector< DMatch > matcher (Mat,Mat);

std::vector<KeyPoint> getPointCle (Mat );

Mat matriceFondamentale (vector<KeyPoint> ,vector<KeyPoint> ,vector<DMatch>);

Mat matriceEssentielle (Mat F, Mat K);

Mat newPosition(Mat camPosition, Mat E, Mat W);

