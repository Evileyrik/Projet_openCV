#include <stdio.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <vector>
#include "traitement.h"

using namespace std;
using namespace cv;

//matrice intraseque trouvée grace a camera-calib : application de mrpt (mobile robot programming toolkit). Le 32F signifie que ce sont des float. F pour float 32 pour 32bites
Mat K = Mat(3,3,CV_64F);
Mat W = Mat (3,3,CV_64F);
float focal=817.49;
Point2d pp=Point2d(639.73,264.44);


int main(){

    K.at<double>(0,0)=870.06;
    K.at<double>(0,1)=0;
    K.at<double>(0,2)=369.73;
    K.at<double>(1,0)=0;
    K.at<double>(1,1)=817.49;
    K.at<double>(1.2)=264.44;
    K.at<double>(2,0)=0;
    K.at<double>(2,1)=0;
    K.at<double>(2,2)=1;
    W.at<double>(0,0)=0;
    W.at<double>(0,1)=-1;
    W.at<double>(0,2)=0;
    W.at<double>(1,0)=1;
    W.at<double>(1,1)=0;
    W.at<double>(1,2)=0;
    W.at<double>(2,0)=0;
    W.at<double>(2,1)=0;
    W.at<double>(2,2)=1;


    // Touche clavier
    char key;
    // Capture vidéo
    VideoCapture cam(0);
    //test si la cam est ouverte
    if (!cam.isOpened())
        cout<<"Cam non ouverte !";
    //creation de la fenetre
    namedWindow("image1");
    namedWindow("image2");
    // Image
    Mat image1,image2;
    int pause=0;
    int cpt;
    //matrice
    Mat F,E,R,T;
    Mat positionCam=Mat(3,1,CV_64F);
    positionCam.at<double> (0,0)=0;
    positionCam.at<double> (1,0)=0;
    positionCam.at<double> (2,0)=0;

    //bons matches
    std::vector<DMatch> bonMatches;
    std::vector<KeyPoint> pointcle2,pointcle1;
    //descriptor
    Mat descripteur1,descripteur2;

    while(key != 'q' && key != 'Q') {

        if(key=='p')
            pause=(pause+1);
        if (pause==0)
        {
            //on lit une image
            cam.read(image1);
            //on test si l'image est vide
            if (image1.empty())
                cout<<"image vide !!"<<endl;
        }

            if (pause==1)
        {
            //on lit une image
            cam.read(image2);
            //on test si l'image est vide
            if (image2.empty())
                cout<<"image vide !!"<<endl;
        }



        //attend 10ms
        key = cvWaitKey(10);

    }
        //boucle afin d'afficher les bons matchs entre 2 images
    if (pause>=2){
        imshow("image1",image1);
        imshow("image2",image2);
        pointcle1=getPointCle(image1);
        pointcle2=getPointCle(image2);
        descripteur1=detectionFeature(image1);
        descripteur2=detectionFeature(image2);
        bonMatches=matcher(descripteur1,descripteur2);
        F=matriceFondamentale(pointcle1,pointcle2,bonMatches);
        E=matriceEssentielle(F,K);

        vector<Point2f> points1,points2;

        for(int i = 0; i<bonMatches.size(); i++ )
        {
            // image de gauche
            points1.push_back(pointcle1[bonMatches[i].queryIdx].pt);
            // image de droite
            points2.push_back(pointcle2[bonMatches[i].trainIdx].pt);
        }

        recoverPose(E,points1,points2,R,T,focal,pp);
        cout<<R<<endl<<T<<endl;



    }




    key=cvWaitKey(10000);

    destroyAllWindows();
    cam.release();//détruit le videocapture. Il faut absolumet quitter le programme avec q ou Q sinon en relançant le programme on va avoir un bug







    return 0;
}
