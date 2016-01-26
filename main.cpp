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




//variables pour le callback
Point2f* point = new Point2f(0,0);
int click=0;
string annotation;


//callback gerant la souris
void souris1(int event, int x, int y, int flags, void* param){

    if (event==CV_EVENT_LBUTTONDOWN){
        Mat &img = *((Mat*)(param));
        point = new Point2f (x,y);
        Point centreTxt;
        cout<<"Annotation : "<<endl;
        cin>>annotation;
        circle(img,*point,5,CV_RGB(0,0,255),5,8,0);
        if ((point->x<620)&&(point->y<400)){
            centreTxt.x=point->x+10;
            centreTxt.y=point->y+10;
        }
        else{
            centreTxt.x=point->x-10;
            centreTxt.y=point->y-10;
        }
        putText(img,annotation,centreTxt,FONT_HERSHEY_COMPLEX,0.8,CV_RGB(255,0,0),3,LINE_8,false);
        click=1;

    }

}



int main(){

    //Initialisation
    char key;
    //variable servant pour la pause
    int pause=0;
    // Capture vidéo par une cam
    VideoCapture cam(-1);
    //test si la cam est ouverte
    if (!cam.isOpened())
        cout<<"Cam non ouverte !";
    //creation des fenetres
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
    //boucle d'execution du programme
    while(key != 'q' && key != 'Q') { //La boucle s'execute tans qu'on n'appuie pas sur q

        if (key=='p') {
            pause=(pause+1); // le bouton p permet de faire pause sur l'image pour sélectionner un point
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

        if(click==1){ //Selection de l'image
            imshow("image1",*image1);
            click=0;
        }


        key = cvWaitKey(5);

    }

    //boucle principale de traitement
    key=cvWaitKey(5);
    while(key!='q'){
        image2=new Mat;
        cam.read(*image2);
        cout<<"test"<<point->x<<endl;
        traitement(point,image1,image2,annotation);
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
