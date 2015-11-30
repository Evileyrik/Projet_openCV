#include <stdio.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"

#include "traitement.h"

using namespace std;
using namespace cv;

int main() {

    // Touche clavier
    char key;
    // Capture vidéo
    VideoCapture cam(1);
    //test si la cam est ouverte
    if (!cam.isOpened())
        cout<<"Cam non ouverte !";
    //permet de savoir si on est en pause ou non
    int pause=0;
    //creation de la fenetre
    namedWindow("cam");
    // Image
    Mat image1,image2;
    //descripteurs
    Mat descripteur1,descripteur2;

    cam.read(image2);
    descripteur2=detectionFeature(image2);
    // Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
    while(key != 'q' && key != 'Q') {

        if(key=='p')
            pause=(pause+1)%2;
        if (pause==0)
        {
            //on lit une image
            cam.read(image1);
            //on test si l'image est vide
            if (image1.empty())
                cout<<"image vide !!"<<endl;
        }

        //attend 10ms
        key = cvWaitKey(10);

    }

    destroyAllWindows();
    cam.release();//détruit le videocapture. Il faut absolumet quitter le programme avec q ou Q sinon en relançant le programme on va avoir un bug
    return 0;

}
