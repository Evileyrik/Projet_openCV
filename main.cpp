#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <vector>
#include "classe.h"
#include "traitement.h"

using namespace std;
using namespace cv;




//variables pour le callback
Point2f* point = new Point2f(0,0);
int click=0;


//callback gerant la souris
void souris1(int event, int x, int y, int flags, void* param){

    if (event==CV_EVENT_LBUTTONDOWN){
        Mat &img = *((Mat*)(param));
        point = new Point2f (x,y);
        circle(img,*point,5,CV_RGB(0,0,255),5,8,0);
        click=1;

    }

}



int main(){



    char key;
    //variable servant pour la pause
    int pause=0;
    // Capture vidéo par une cam
    VideoCapture cam(-1);
    //test si la cam est ouverte
    if (!cam.isOpened())
        cout<<"Cam non ouverte !";
    //creation de la fenetre
    namedWindow("image1");
    namedWindow("image2");
    // Image
    Mat* image1=new Mat;
    Mat* image2=new Mat;


    //callback
    setMouseCallback("image1", souris1, image1);
   //bons matches
    std::vector<DMatch> bonMatches;
    std::vector<KeyPoint> pointcle2,pointcle1;
    //descriptor
    Mat descripteur1,descripteur2;
    //Matrice Homography
    Mat* H=new Mat;

    //a améliorer!!!
    while(key != 'q' && key != 'Q') {

        if (key=='p') {
            pause=(pause+1);
        }
        if (pause==0)
        {
            //on lit une image
            cam.read(*image1);
            //on test si l'image est vide
            if (image1->empty())
                cout<<"image vide !!"<<endl;
            imshow("image1",*image1);
        }

        if(click==1){
            imshow("image1",*image1);
            click=0;
        }


        key = cvWaitKey(5);

    }

    key=cvWaitKey(5);
    while(key!='q'){
        image2=new Mat;
        cam.read(*image2);
        traitement(point,image1,image2);
        key=cvWaitKey(40);
    }


    key=cvWaitKey(5);

    while (key!='q') {
        key=cvWaitKey(5);
    }

    //destruction des fenetres et des cameras
    destroyAllWindows();
    cam.release();


    return 0;
}
