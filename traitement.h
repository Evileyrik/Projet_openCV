#ifndef TRAITEMENT_H
#define TRAITEMENT_H

#include <stdio.h>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include "classe.h"

using namespace cv;
using namespace std;


Mat detectionFeature (Mat);

std::vector< DMatch > matcher (Mat,Mat);

std::vector<KeyPoint> getPointCle (Mat );

Mat getHomography(Mat, Mat);

void traitement(Point2f* point,Mat* image1,Mat* image2);

#endif // TRAITEMENT_H
