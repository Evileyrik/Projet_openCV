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

void calculCamMat(Mat& P1,Mat& P2,Mat R,Mat T);

Mat calculVecteur (Point, Mat);

Mat calculCentre (Mat);

double distDroite (Mat ,Mat , Mat , Mat );

Mat projection(Mat,Mat,Mat);
