#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>

#include <vector>
#include "traitement.h"

using namespace std;
using namespace cv;

//matrice intraseque trouvée grace a camera-calib : application de mrpt (mobile robot programming toolkit). Le 32F signifie que ce sont des float. F pour float 32 pour 32bites
Mat K = Mat(3,3,CV_64F);
double focal=817.4911;
Point2d pp=Point2d(369.7327,264.436);
//640*480
//focal = 4mm
//pixel size : 2.8umx2.8um ????



//var globales pour le callback
Point2d point1,point2;
int click = 0;

//callback gerant la souris
void souris1(int event, int x, int y, int flags, void* param){

    if (event==CV_EVENT_LBUTTONDOWN){
        Mat &img = *((Mat*)(param));
        point1 = Point2d(x,y);
        click=1;
        circle(img,point1,5,CV_RGB(0,0,255),5,8,0);

    }

}
void souris2(int event, int x, int y, int flags, void* param){

    if (event==CV_EVENT_LBUTTONDOWN){
        Mat &img = *((Mat*)(param));
        point2 = Point2d(x,y);
        click=1;
        circle(img,point2,5,CV_RGB(0,0,255),5,8,0);

    }

}



int main(){

    K.at<double>(0,0)=870.0636;
    K.at<double>(0,1)=0;
    K.at<double>(0,2)=369.7327;
    K.at<double>(1,0)=0;
    K.at<double>(1,1)=817.4911;
    K.at<double>(1.2)=264.436;
    K.at<double>(2,0)=0;
    K.at<double>(2,1)=0;
    K.at<double>(2,2)=1;


    // Touche clavier
    char key;
    int pause=0;
    // Capture vidéo
    VideoCapture cam(-1);
    //test si la cam est ouverte
    if (!cam.isOpened())
        cout<<"Cam non ouverte !";
    //creation de la fenetre
    namedWindow("image1");
    namedWindow("image2");
    // Image
    Mat image1,image2;
    //callback
    setMouseCallback("image1", souris1, &image1);
    setMouseCallback("image2", souris2, &image2);
    int cpt;
   /* //matrice
    Mat F,E,R,T;
    Mat P1=Mat(3,4,CV_64F);
    Mat P2=Mat(3,4,CV_64F);
    Mat vect1=Mat(4,1,CV_64F);
    Mat vect2=Mat(4,1,CV_64F);
    Mat center1=Mat (3,1,CV_64F);
    Mat center2=Mat(3,1,CV_64F);*/
    //coordonnée clicks
   // Point2d pointImg1,pointImg2;
    Mat R,T;
    Mat P1=Mat(3,4,CV_64F);
    Mat P2=Mat(3,4,CV_64F);
    Mat intersection;
    Mat X = Mat(4,1,CV_64F);
    Mat x = Mat(3,1,CV_64F);
    Point2d pointFinal;

   /* //bons matches
    std::vector<DMatch> bonMatches;
    std::vector<KeyPoint> pointcle2,pointcle1;
    //descriptor
    Mat descripteur1,descripteur2;*/


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

            imshow("image1",image1);

        }

        if (pause==1)
        {
            //on lit une image
            cam.read(image2);
            //on test si l'image est vide
            if (image2.empty())
                cout<<"image vide !!"<<endl;
            imshow("image2",image2);

        }

        if (click==1){
            imshow("image1",image1);
            imshow("image2",image2);
            click=0;
        }

        //attend 5ms
        key = cvWaitKey(5);

    }
   //on calcule trouve le point correspondant
    if (pause>=2){
        system("clear");
        imshow("image1",image1);
        imshow("image2",image2);
      /*  pointcle1=getPointCle(image1);
        pointcle2=getPointCle(image2);
        descripteur1=detectionFeature(image1);
        descripteur2=detectionFeature(image2);
        bonMatches=matcher(descripteur1,descripteur2);
        F=matriceFondamentale(pointcle1,pointcle2,bonMatches);
        Mat F2;
        F.convertTo(F2,CV_64F);
        E=matriceEssentielle(F2,K);

        vector<Point2f> points1,points2;

        for(int i = 0; i<bonMatches.size(); i++ )
        {
            // image de gauche
            points1.push_back(pointcle1[bonMatches[i].queryIdx].pt);
            // image de droite
            points2.push_back(pointcle2[bonMatches[i].trainIdx].pt);
        }

        //on calcule les matrices de rotations et translations
        recoverPose(E,points1,points2,R,T,focal,pp,noArray());
        //on calcule P et P' selon H&Z
        calculCamMat(P1,P2,R,T);
        //calcul des vecteurs directeurs
        vect1=calculVecteur(pointImg1,P1);
        vect2=calculVecteur(pointImg2,P2);

*/


        //point1 et point2 en pixels ! + origine en haut a gauche
        //dans nos calculs : en metrique + origine au centre !!
        //changement repère
cout<<"point 1 "<<point1<<endl<<"point 2"<<point2<<endl;
        point1.x=point1.x-320;
        point1.y=-(point1.y-240);
        point2.x=point2.x-320;
        point2.y=-(point2.y-240);
       //changement unité
        point1.x=point1.x*4/817.49;
        point1.y=point1.y*4/817.49;

        point2.x=point2.x*4/817.49;
        point2.y=point2.y*4/817.49;


        intersection=traitementFinal(image1,image2,K,focal,pp,P1,P2,R,T,point1,point2);
        X.at<double>(0,0)= intersection.at<double>(0,0);
        X.at<double>(1,0)= intersection.at<double>(1,0);
        X.at<double>(2,0)= intersection.at<double>(2,0);
        X.at<double>(3,0)= 1;
        cout<<"X :"<<X<<endl;

    }

    key=cvWaitKey(5);







        x=P1*X;
        //attention : x en qqchose et centré  !!
        x.at<double>(0,0)=x.at<double>(0,0)/x.at<double>(2,0);
        x.at<double>(1,0)=x.at<double>(1,0)/x.at<double>(2,0);
        x.at<double>(2,0)=x.at<double>(2,0)/x.at<double>(2,0);
        cout<<"x"<<x<<endl;
        pointFinal.x=320+x.at<double>(0,0)*817.49/4;
        pointFinal.y=240-x.at<double>(1,0)*817.49/4;

cout<<"pointfinal 1 "<<pointFinal<<endl;

        //on affiche le projeté du point 3D sur l'image de la cam
        circle(image1,pointFinal,5,CV_RGB(255,0,0),5,8,0);

        x=P2*X;
        //attention : x en qqchose et centré  !!
        x.at<double>(0,0)=x.at<double>(0,0)/x.at<double>(2,0);
        x.at<double>(1,0)=x.at<double>(1,0)/x.at<double>(2,0);
        x.at<double>(2,0)=x.at<double>(2,0)/x.at<double>(2,0);
        cout<<"x2 : "<<x<<endl;
        pointFinal.x=320+x.at<double>(0,0)*817.49/4;
        pointFinal.y=240-x.at<double>(1,0)*817.49/4;
cout<<"pointfinal 2 "<<pointFinal<<endl;


        //on affiche le projeté du point 3D sur l'image de la cam
        circle(image2,pointFinal,5,CV_RGB(255,0,0),5,8,0);

        imshow("image1",image1);
        imshow("image2",image2);



/*key=cvWaitKey(10);
    while (key!='p'){
        cam.read(image2);
    key=cvWaitKey(10);
    }
    traitementIntermediaire(image1,image2,K,focal,pp,P1,P2,R,T);
    x=P2*X;
 //attention : x en qqchose et centré  !!
        x.at<double>(0,0)=x.at<double>(0,0)/x.at<double>(2,0);
        x.at<double>(1,0)=x.at<double>(1,0)/x.at<double>(2,0);

        pointFinal.x=320+(x.at<double>(0,0)*817.49)/4.0;
        pointFinal.y=240-(x.at<double>(1,0)*817.49)/4.0;
        circle(image2,pointFinal,5,CV_RGB(255,0,0),5,8,0);
        cout<<pointFinal<<endl;
        imshow("cam",image2);*/

 key=cvWaitKey(10);
    while (key!='q'){

    key=cvWaitKey(10);
    }

    destroyAllWindows();
    cam.release();//détruit le videocapture. Il faut absolumet quitter le programme avec q ou Q sinon en relançant le programme on va avoir un bug






    return 0;
}
