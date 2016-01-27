
#include <stdio.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <vector>
#include "traitement.h"


using namespace std;
using namespace cv;


int test(){
   /* Mat cameraMatrix=Mat(3,4,CV_64F);
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
    cout<<center<<endl;

    Mat A=Mat(3,1,CV_64F);
    Mat vectdir=Mat(3,1,CV_64F);
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

    cameraMatrix.at<double>(0,0)=1;
    cameraMatrix.at<double>(0,1)=0;
    cameraMatrix.at<double>(0,2)=0;
    cameraMatrix.at<double>(0,3)=3;
    cameraMatrix.at<double>(1,0)=0;
    cameraMatrix.at<double>(1,1)=1;
    cameraMatrix.at<double>(1,2)=0;
    cameraMatrix.at<double>(1,3)=4;
    cameraMatrix.at<double>(2,0)=0;
    cameraMatrix.at<double>(2,1)=0;
    cameraMatrix.at<double>(2,2)=1;
    cameraMatrix.at<double>(2,3)=0;
    Point2d x;
    x.x=2;
    x.y=3;

    cout<<calculVecteur(x,cameraMatrix)<<endl;

    Mat cameraMatrix1=Mat(3,4,CV_64F);
    cameraMatrix1.at<double>(0,0)=1;
    cameraMatrix1.at<double>(0,1)=0;
    cameraMatrix1.at<double>(0,2)=0;
    cameraMatrix1.at<double>(0,3)=0;
    cameraMatrix1.at<double>(1,0)=0;
    cameraMatrix1.at<double>(1,1)=1;
    cameraMatrix1.at<double>(1,2)=0;
    cameraMatrix1.at<double>(1,3)=0;
    cameraMatrix1.at<double>(2,0)=0;
    cameraMatrix1.at<double>(2,1)=0;
    cameraMatrix1.at<double>(2,2)=1;
    cameraMatrix1.at<double>(2,3)=0;

    Mat cameraMatrix2=Mat(3,4,CV_64F);
    cameraMatrix2.at<double>(0,0)=1;
    cameraMatrix2.at<double>(0,1)=0;
    cameraMatrix2.at<double>(0,2)=0;
    cameraMatrix2.at<double>(0,3)=2;
    cameraMatrix2.at<double>(1,0)=0;
    cameraMatrix2.at<double>(1,1)=0;
    cameraMatrix2.at<double>(1,2)=-1;
    cameraMatrix2.at<double>(1,3)=0;
    cameraMatrix2.at<double>(2,0)=0;
    cameraMatrix2.at<double>(2,1)=1;
    cameraMatrix2.at<double>(2,2)=0;
    cameraMatrix2.at<double>(2,3)=1;
    Mat center1=calculCentre(cameraMatrix1);
    Mat center2 = calculCentre(cameraMatrix2);
    cout<<center1<<endl<<center2<<endl;

    Mat vectdir1 = Mat(3,1,CV_64F);
    vectdir1.at<double>(0,0)=1;
    vectdir1.at<double>(1,0)=0;
    vectdir1.at<double>(2,0)=0;

    Mat vectdir2 = Mat(3,1,CV_64F);
    vectdir2.at<double>(0,0)=0;
    vectdir2.at<double>(1,0)=1;
    vectdir2.at<double>(2,0)=0;

    double distance = distDroite(center1,center2,vectdir1,vectdir2);
    cout<<distance<<endl;

    cout<<"intersection : "<<endl<<intersection(vectdir1,vectdir2,cameraMatrix1,cameraMatrix2)<<endl;
*/
   Mat cameraMatrix1 = Mat (3,4,CV_64F);
    Mat cameraMatrix2=Mat(3,4,CV_64F);
    Mat vectdir1 = Mat (3,1,CV_64F);
    Mat vectdir2= Mat (3,1,CV_64F);
    Mat center1 = Mat (3,1,CV_64F);
    Mat center2 = Mat (3,1,CV_64F);
    cameraMatrix1.at<double>(0,0)=1;
    cameraMatrix1.at<double>(0,1)=0;
    cameraMatrix1.at<double>(0,2)=0;
    cameraMatrix1.at<double>(0,3)=3;
    cameraMatrix1.at<double>(1,0)=0;
    cameraMatrix1.at<double>(1,1)=1;
    cameraMatrix1.at<double>(1,2)=0;
    cameraMatrix1.at<double>(1,3)=3;
    cameraMatrix1.at<double>(2,0)=0;
    cameraMatrix1.at<double>(2,1)=0;
    cameraMatrix1.at<double>(2,2)=1;
    cameraMatrix1.at<double>(2,3)=3;

    cameraMatrix2.at<double>(0,0)=1;
    cameraMatrix2.at<double>(0,1)=0;
    cameraMatrix2.at<double>(0,2)=0;
    cameraMatrix2.at<double>(0,3)=2;
    cameraMatrix2.at<double>(1,0)=0;
    cameraMatrix2.at<double>(1,1)=0;
    cameraMatrix2.at<double>(1,2)=-1;
    cameraMatrix2.at<double>(1,3)=4;
    cameraMatrix2.at<double>(2,0)=0;
    cameraMatrix2.at<double>(2,1)=1;
    cameraMatrix2.at<double>(2,2)=0;
    cameraMatrix2.at<double>(2,3)=3;
    center1=calculCentre(cameraMatrix1);
    center2 = calculCentre(cameraMatrix2);
    cout<<center1<<endl<<center2<<endl;

    vectdir1.at<double>(0,0)=1;
    vectdir1.at<double>(1,0)=0;
    vectdir1.at<double>(2,0)=0;

    vectdir2.at<double>(0,0)=0;
    vectdir2.at<double>(1,0)=0;
    vectdir2.at<double>(2,0)=1;

    double distance = distDroite(center1,center2,vectdir1,vectdir2);
    cout<<distance<<endl;

    cout<<"Projection "<<endl<<projection(center1,vectdir2,center2)<<endl;

    cout<<"intersection : "<<endl<<intersection(vectdir1,vectdir2,cameraMatrix1,cameraMatrix2)<<endl;


return 0;
}
