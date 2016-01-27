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
double focal=817.49;
Point2d pp=Point2d(639.73,264.44);

//var globales pour le callback
Point2f point;
int click = 0;
//callback gerant la souris
void souris(int event, int x, int y, int flags, void* param){

    if (event==CV_EVENT_LBUTTONDOWN){
        Mat &img = *((Mat*)(param));
        point = Point2f (x,y);
        click=1;
        circle(img,point,5,CV_RGB(0,0,255),5,8,0);

    }

}



int main(){
    //matrice intrinseque
    K.at<double>(0,0)=870.06;
    K.at<double>(0,1)=0;
    K.at<double>(0,2)=369.73;
    K.at<double>(1,0)=0;
    K.at<double>(1,1)=817.49;
    K.at<double>(1.2)=264.44;
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
    setMouseCallback("image1", souris, &image1);
    setMouseCallback("image2", souris, &image2);
    int cpt;
    //point 3D
    Mat pointFinal;
    //coordonnée clicks
    Point2d pointImg1,pointImg2;
    //distance entre les 2 droites
    double distance=0;
    //matrices
    Mat P1=Mat(3,4,CV_64F);
    Mat P2=Mat(3,4,CV_64F);
    Mat R=Mat(3,3,CV_64F);
    Mat T=Mat(3,1,CV_64F);


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
            if (click==1)
                pointImg1=point;
        }

        if (pause==1)
        {
            //on lit une image
            cam.read(image2);
            //on test si l'image est vide
            if (image2.empty())
                cout<<"image vide !!"<<endl;
            imshow("image2",image2);
            if (click==1)
                pointImg2=point;

        }

        if (click==1){
            imshow("image1",image1);
            imshow("image2",image2);
            click=0;
        }

        //attend 10ms
        key = cvWaitKey(5);

    }
        //boucle afin d'afficher les bons matchs entre 2 images
    if (pause>=2){
        imshow("image1",image1);
        imshow("image2",image2);
        traitementIntermediaire(image1,image2,K,focal,pp,P1,P2,R,T);


        pointFinal=traitementFinal(image1,image2,K,focal,pp,P1,P2,R,T,pointImg1,pointImg2);
        cout<<pointFinal<<endl;
        Mat X=Mat(4,1,CV_64F);
        X.at<double>(0,0)=pointFinal.at<double>(0,0);
        X.at<double>(1,0)=pointFinal.at<double>(1,0);
        X.at<double>(2,0)=pointFinal.at<double>(2,0);
        X.at<double>(3,0)=1;
        cout<<X<<endl;
        //affichage de la projection du point trouvé sur la première frame
        Mat x = P1*X;
        cout<<"x"<<x<<endl;
        Point2d pointa= Point2d(0,0);
        pointa.x = x.at<double>(0,0);
        pointa.y=x.at<double>(1,0);
        if (x.at<double>(2,0)!=0){
            pointa.x=pointa.x/x.at<double>(2,0);
            pointa.y=pointa.y/x.at<double>(2,0);
        }
        pointa.x=320+pointa.x*817/4;
        pointa.y=240-pointa.y*817/4;
        circle(image1,pointa,5,CV_RGB(255,0,0),5,8,0);
        imshow("image1",image1);
        //affichage de la projection du point trouvé sur la deuxième frame
        x=P2*X;
        pointa.x = x.at<double>(0,0);
        pointa.y=x.at<double>(1,0);
        if (x.at<double>(2,0)!=0){
            pointa.x=pointa.x/x.at<double>(2,0);
            pointa.y=pointa.y/x.at<double>(2,0);
        }
        pointa.x=320+pointa.x;
        pointa.y=240-pointa.y;
        circle(image2,pointa,5,CV_RGB(0,0,255),5,8,0);
        imshow("image2",image2);
    }





    key=cvWaitKey(5);
    while (key!='q'){
        key=cvWaitKey(5);
    }
    destroyAllWindows();
    cam.release();//détruit le videocapture. Il faut absolumet quitter le programme avec q ou Q sinon en relançant le programme on va avoir un bug






    return 0;
}
