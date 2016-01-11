
#include <stdio.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <vector>
#include "traitement.h"

using namespace std;
using namespace cv;

int test(){

    /*Mat cameraMatrix=Mat(3,4,CV_64F);
    cameraMatrix.at<double>(0,0)=1;
    cameraMatrix.at<double>(0,1)=0;
    cameraMatrix.at<double>(0,2)=0;
    cameraMatrix.at<double>(0,3)=0;
    cameraMatrix.at<double>(1,0)=0;
    cameraMatrix.at<double>(1,1)=1;
    cameraMatrix.at<double>(1,2)=0;
    cameraMatrix.at<double>(1,3)=0;
    cameraMatrix.at<double>(2,0)=0;
    cameraMatrix.at<double>(2,1)=0;
    cameraMatrix.at<double>(2,2)=1;
    cameraMatrix.at<double>(2,3)=1;


    Mat center = Mat(3,1,CV_64F);
    center = calculCentre(cameraMatrix);
    cout<<center<<endl;*/
    Mat A=Mat(3,1,CV_64F);
    Mat vectdir=Mat(3,1,CV_64F);
    Mat center=Mat(3,1,CV_64F);
    Mat projete=Mat(3,1,CV_64F);

    A.at<double>(0,0)=5;
    A.at<double>(0,1)=1;
    A.at<double>(0,2)=0;


    vectdir.at<double>(0,0)=1;
    vectdir.at<double>(0,1)=0;
    vectdir.at<double>(0,2)=0;


    center.at<double>(0,0)=0;
    center.at<double>(0,1)=0;
    center.at<double>(0,2)=3;

    projete=projection(A,vectdir,center);
    cout<<projete<<endl;


    return 0;
}
