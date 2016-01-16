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

Mat intersection (Mat , Mat ,Mat ,Mat );

void traitementIntermediaire(Mat image1,Mat image2, Mat K, double focal, Point2d pp,Mat& P1,Mat& P2, Mat& R, Mat& T);

Mat traitementFinal(Mat image1,Mat image2, Mat K, double focal, Point2d pp,Mat& P1, Mat& P2, Mat& R, Mat& T,Point2d pointImg1,Point2d pointImg2);

