/*#include <stdio.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"

#include "traitement.h"

using namespace std;
using namespace cv;

//vecteur contenant les coordonnées des points cliqués
vector<Point> points;
//permet de savoir si on est en pause ou non
int pause=0;

//callback gerant la souris. on ne retient des coordonnées que lorsqu'on est en pause (ie qu'on a choisit l'image)
void souris(int event, int x, int y, int flags, void* param){


    if ((event==CV_EVENT_LBUTTONDOWN)&&(pause%2==1))
        points.push_back(Point(x,y));

}

//boucle principale
int hue() {

    // Touche clavier
    char key;
    // Capture vidéo
    VideoCapture cam(0);
    //test si la cam est ouverte
    if (!cam.isOpened())
        cout<<"Cam non ouverte !";

    //creation de la fenetre
    namedWindow("cam");
    // Image
    Mat image1,image2;
    Mat image_matches;
    //descripteurs
    Mat descripteur1,descripteur2;
    //bons points
    vector<KeyPoint> pointCle1,pointCle2;
    //bon match
    vector<DMatch> bonMatches;
    //callback
    setMouseCallback("cam", souris, NULL);


    //initialisation de la 2em image
    cam.read(image2);
    descripteur2=detectionFeature(image2);
    pointCle2=getPointCle(image2);

    // Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
    while(key != 'q' && key != 'Q') {

        if(key=='p')
            pause=(pause+1)%4;
        if (pause==0)
        {
            //on lit une image
            cam.read(image1);
            //on test si l'image est vide
            if (image1.empty())
                cout<<"image vide !!"<<endl;

            descripteur1=detectionFeature(image1);
            pointCle1=getPointCle(image1);
            imshow("cam",image1);

        }
        if (pause==2)
        {

            //on lit une image
            cam.read(image2);
            //on test si l'image est vide
            if (image1.empty())
                cout<<"image vide !!"<<endl;

            descripteur2=detectionFeature(image2);
            pointCle2=getPointCle(image2);
            imshow("cam",image2);
            //on efface les deux premiers points afin de n'avoir que les coordonées des 2 points


        }

        bonMatches=matcher (descripteur1,descripteur2);
        drawMatches( image1, pointCle1, image2, pointCle2,bonMatches, image_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
        imshow("Good Matches",image_matches);
        cout<<points.size()<<endl;

        //attend 10ms
        key = cvWaitKey(10);

    }

    destroyAllWindows();
    cam.release();//détruit le videocapture. Il faut absolumet quitter le programme avec q ou Q sinon en relançant le programme on va avoir un bug
    return 0;

}*/
