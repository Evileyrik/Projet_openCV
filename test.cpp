#include <stdio.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"

#include "traitement.h"

using namespace std;
using namespace cv;

int main(){
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
    //matrice
    Mat matrice;
    //bons matches
    std::vector<DMatch> bonMatches;
    std::vector<KeyPoint> keypoints_2,keypoints_1;
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

    //boucle afin d'afficher les bons matchs entre 2 images
    if (pause>=2){
        imshow("image1",image1);
        imshow("image2",image2);
        keypoints_1=getPointCle(image1);
        keypoints_2=getPointCle(image2);
        descripteur1=detectionFeature(image1);
        descripteur2=detectionFeature(image2);
        bonMatches=matcher(descripteur1,descripteur2);
        matrice=matriceFondamentale(keypoints_1,keypoints_2,bonMatches);
        cout<<matrice<<endl;
    }

        //attend 10ms
        key = cvWaitKey(10);

    }





    cout<<bonMatches.size()<<endl;

    key=cvWaitKey(10000);

    destroyAllWindows();
    cam.release();//détruit le videocapture. Il faut absolumet quitter le programme avec q ou Q sinon en relançant le programme on va avoir un bug




    return 0;
}
