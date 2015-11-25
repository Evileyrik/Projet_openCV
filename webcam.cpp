//création webcam
//v1.1
//25/11/15


#include <stdio.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"


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

    namedWindow("cam");
    // Image
    Mat image;

    // Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
    while(key != 'q' && key != 'Q') {

        if(key=='p')
            pause=(pause+1)%2;
        if (pause==0)//si on n'est pas en pause, on recupere une nouvelle image
        {
            //on lit une image
            cam.read(image);
            //on test si l'image est vide
            if (image.empty())
                cout<<"image vide !!"<<endl;
        }
        //affiche l'image
        imshow("cam",image);
        //attend 10ms
        key = cvWaitKey(10);

    }

    destroyAllWindows();
    cam.release();//détruit le videocapture. Il faut absolumet quitter le programme avec q ou Q sinon en relançant le programme on va avoir un bug
    return 0;

}
