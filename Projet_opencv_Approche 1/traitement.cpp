#include <stdio.h>
#include <iostream>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include "traitement.h"

using namespace cv;
using namespace std;




Mat detectionFeature (Mat image){

    int nbPoints = 7000;//détermine le nombre de points d'interets

    //detection des points cle
    Ptr<FeatureDetector> detecteur = ORB::create();
    std::vector<KeyPoint> pointsCle;
    detecteur->detect( image, pointsCle );

    //Calcul les zones cle de description
    Ptr<DescriptorExtractor> extracteur = ORB::create();
    Mat descripteur;
    extracteur->compute(image,pointsCle,descripteur);

    return descripteur;

}

std::vector<KeyPoint> getPointCle (Mat image){

    int nbPoints = 7000;//détermine le nombre de points d'interets

    //detection des points cle
    Ptr<FeatureDetector> detecteur = ORB::create();
    std::vector<KeyPoint> pointsCle;
    detecteur->detect( image, pointsCle );

    return pointsCle;

}

 std::vector< DMatch > matcher (Mat descripteur1,Mat descripteur2){

    //on recupere les zones en communs
    FlannBasedMatcher matcher;
    std::vector<DMatch> matches;

    if(descripteur1.type()!=CV_32F) {
        descripteur1.convertTo(descripteur1, CV_32F);//pb de types -< le match prend des descriptors float et non binaires
    }

    if(descripteur2.type()!=CV_32F) {
        descripteur2.convertTo(descripteur2, CV_32F);
    }

    matcher.match( descripteur1, descripteur2, matches );

    double dmax = 0; double dmin= 1000;

    //calcul des distances max et min entre les matches
    for( int i = 0; i < matches.size(); i++ )
    {
        double d = matches[i].distance;
        if( d<dmin )
            dmin=d;
        if( d>dmax)
            dmax=d;
    }

    //on veut ne garder que les "bonnes zones communes" -> définir "bonnes zones" !!!
    std::vector<DMatch> bonMatches;
    for (int i=0;i<matches.size();i++)
    {
        if (matches[i].distance<=4*dmin)
            bonMatches.push_back(matches[i]);
    }

    return bonMatches;

}

Mat matriceFondamentale (vector<KeyPoint> pointcle1,vector<KeyPoint> pointcle2,vector<DMatch> bonMatches){

    vector<Point2f> points1,points2;

    for(int i = 0; i<bonMatches.size(); i++ )
    {
        // image de gauche
        points1.push_back(pointcle1[bonMatches[i].queryIdx].pt);
        // image de droite
        points2.push_back(pointcle2[bonMatches[i].trainIdx].pt);
    }

    Mat matrice = findFundamentalMat(points1,points2,FM_RANSAC, 3, 0.90);//méthode de calcul utilisant RANSAC, avec une erreur tolérée de 3 avec une proba de 99%
    return matrice;
}



Mat matriceEssentielle (Mat F, Mat K){
    Mat L;
    transpose(K,L);
    cout<<"essential matrx : transpose ok"<<endl;

    Mat E = L*F;
    E=E*F;
    return E;


}


//on calcule les matrice cam selon formules de H&Z
void calculCamMat(Mat& P1,Mat& P2,Mat R,Mat T,Mat K){

    for (int i=0;i<3;i++)
    {
        for (int j=0;j<4;j++)
        {
            if (i==j)
                P1.at<double>(i,j)=1;
            else
                P1.at<double>(i,j)=0;

        }
    }

    for (int i=0;i<3;i++)
    {
        for (int j=0;j<4;j++)
        {
            if (j<3)
                P2.at<double>(i,j)=R.at<double>(i,j);
            else
                P2.at<double>(i,j)=T.at<double>(i);

        }
    }

P1=K*P1;
P2=K*P2;


}

//on calcule un vecteur directeur à l'aide de la relation dans H&Z x=KPX que l'on peut mettre sous forme AX'=b ou A est 3x3 et X' = (X,Y,Z)
//on a PseudoInverse*x et C qui sont sur la mm droite
Mat calculVecteur(Point2d point,Mat mat){

    Mat center = Mat(3,1,CV_64F);
    center=calculCentre(mat);
    Mat matPseudo = mat.inv(DECOMP_SVD);
    Mat temp = Mat(3,1,CV_64F);
    Mat vect = Mat (4,1,CV_64F);
    Mat vectdir = Mat(3,1,CV_64F);
    temp.at<double>(0,0)=point.x;
    temp.at<double>(1,0)=point.y;
    temp.at<double>(2,0)=1;

    vect=matPseudo*temp;

    if (vect.at<double>(3,0)!=0){
        vect.at<double>(0,0)=vect.at<double>(0,0)/vect.at<double>(3,0);
        vect.at<double>(1,0)=vect.at<double>(1,0)/vect.at<double>(3,0);
        vect.at<double>(2,0)=vect.at<double>(2,0)/vect.at<double>(3,0);
        vect.at<double>(3,0)=vect.at<double>(3,0)/vect.at<double>(3,0);
    }


    vectdir.at<double>(0,0)=vect.at<double>(0,0);
    vectdir.at<double>(1,0)=vect.at<double>(1,0);
    vectdir.at<double>(2,0)=vect.at<double>(2,0);
    vectdir=vectdir-center;

    return vectdir;
}

//on calcule le cntre de la caméra à l'aide de la relation de H&Z
Mat calculCentre(Mat cameraMatrix){

    Mat m=Mat(3,3,CV_64F);
    Mat p=Mat(3,1,CV_64F);

    //extraction de la matrice de rotation et la matrice de translation
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            m.at<double>(i,j)=cameraMatrix.at<double>(i,j);
        }
        p.at<double>(i,0)=cameraMatrix.at<double>(i,3);
    }
    Mat center = Mat(3,1,CV_64F);
    center = -m.inv(DECOMP_SVD)*p;
    return center;
}


//on calcule la distance entre deux droites à l'aide d'une formule mathématique.
double distDroite (Mat center1,Mat center2, Mat vectDir1, Mat vectDir2){

    Mat A = center1+vectDir1;
    Mat B = center2+vectDir2;
    Mat C = B-A;
    Mat M =Mat(3,3,CV_64F);

    for (int i=0;i<3;i++){
        M.at<double>(i,0)=C.at<double>(i,0);
        M.at<double>(i,1)=vectDir1.at<double>(i,0);
        M.at<double>(i,2)=vectDir2.at<double>(i,0);
    }

    double norme =norm( vectDir1.cross(vectDir2));

    if (norme ==0)//les droites sont parallelles
        return norm(center2-center1);
    else
        return abs(determinant(M)/norme);

}

//on calcule la projection orthogonale d'un point sur une droite
Mat projection(Mat point,Mat vectdir,Mat center){

    Mat projete=Mat(3,1,CV_64F);
    //Soit M le projeté de A sur la droite D. Soient B et C des points de D. On a alors : dot(AM,BC) = 0 et cross(BM,BC) =0
    // on peut le ramener sous la forme matricielle classique Ax=b ou A est une matrice 3x3
    Mat B=center+vectdir;
    Mat C=center+5*vectdir;
    double a = C.at<double>(0,0)-B.at<double>(0,0);
    double b = C.at<double>(1,0)-B.at<double>(1,0);
    double c = C.at<double>(2,0)-B.at<double>(2,0);

    Mat A=Mat(4,3,CV_64F);
    Mat D=Mat(4,1,CV_64F);

    A.at<double>(0,0)=a;
    A.at<double>(0,1)=b;
    A.at<double>(0,2)=c;
    A.at<double>(1,0)=b;
    A.at<double>(1,1)=-a;
    A.at<double>(1,2)=0;
    A.at<double>(2,0)=-c;
    A.at<double>(2,1)=0;
    A.at<double>(2,2)=a;
    A.at<double>(3,0)=0;
    A.at<double>(3,1)=c;
    A.at<double>(3,2)=-b;


    D.at<double>(0,0)=a*point.at<double>(0,0)+b*point.at<double>(1,0)+c*point.at<double>(2,0);
    D.at<double>(1,0)=b*B.at<double>(0,0)-a*B.at<double>(1,0);
    D.at<double>(2,0)=a*B.at<double>(2,0)-c*B.at<double>(0,0);
    D.at<double>(3,0)=c*B.at<double>(1,0)-b*B.at<double>(2,0);

    solve(A,D,projete,DECOMP_SVD);

    return projete;
}

Mat intersection (Mat vectdir1, Mat vectdir2,Mat camMatrix1,Mat camMatrix2){
    //on va trouver le milieu du segment le plus petit reliant les deux droites et faire l'approximation que ce milieu correspond au point 3d cherché.
    Mat center1 = calculCentre(camMatrix1);
    Mat center2 = calculCentre(camMatrix2);
    double distance = distDroite(center1,center2,vectdir1,vectdir2);
    double epsilon;
    double alpha;
    double d1,d2;
    Mat pointa=Mat (3,1,CV_64F);
    Mat projetea=Mat (3,1,CV_64F);
    Mat pointb=Mat (3,1,CV_64F);
    Mat projeteb=Mat (3,1,CV_64F);
    Mat vectoppose=-vectdir1;
    //définition du pas et du seuil de tolérance
    if (distance == 0){
        epsilon=(double)1/(double)1000;
        alpha=(double)1/(double)100;
    }
    else if(distance!=0){
        epsilon=distance/(double)1000;//a voir
        alpha=distance/(double)100;
    }
    int k=1;

    pointa=k*alpha*vectdir1+center1;
    projetea=projection(pointa,vectdir2,center2);
    d1=norm(projetea-pointa);

    pointb=center1+k*alpha*vectoppose;
    projeteb=projection(pointb,vectdir2,center2);
    d2=norm(projeteb-pointb);


    //parcours de la droite provenant de la 1ere image dans les deux sens
   while ((abs(distance-d1)>epsilon)&&(abs(distance-d2)>epsilon)){


        k++;
        pointa=(k*alpha)*vectdir1+center1;
        projetea=projection(pointa,vectdir2,center2);
        d1=norm(projetea-pointa);

        pointb=center1+(k*alpha)*vectoppose;
        projeteb=projection(pointb,vectdir2,center2);
       d2=norm(projeteb-pointb);

    }
    Mat testa = k*alpha*vectdir1+center1;
    Mat testb=center1+k*alpha*vectoppose;
    Mat testaProjete = projection(testa,vectdir2,center2);
    Mat testbProjete = projection(testb,vectdir2,center2);
    cout<<testa<<endl;
    double dtest1 = norm(testaProjete-testa);
    double dtest2 = norm(testbProjete-testb);

    if (abs(distance-dtest1)<epsilon)
        return ((testa+testaProjete)/2);

    else if (abs(distance-dtest2)<epsilon)
        return ((testb+testbProjete)/2);

    else if (abs(distance-dtest1)<abs(distance-dtest2))
        return ((testa+testaProjete)/2);
    else
         return ((testb+testbProjete)/2);
}



//traitement intermediaire de l'image, on s'arrete au calcul des matrices de cameras (on en a besoin plus tard)
void traitementIntermediaire(Mat image1,Mat image2, Mat K, double focal, Point2d pp,Mat& P1, Mat& P2, Mat& R, Mat& T){

        //matrice
        Mat F,E;
        Mat center1=Mat (3,1,CV_64F);
        Mat center2=Mat(3,1,CV_64F);
        //bons matches
        std::vector<DMatch> bonMatches;
        std::vector<KeyPoint> pointcle2,pointcle1;
        vector<Point2f> points1,points2;
        //descriptor
        Mat descripteur1,descripteur2;

        //on trouve les pointsCles
        pointcle1=getPointCle(image1);
        pointcle2=getPointCle(image2);
        //on trouve les descripteurs
        descripteur1=detectionFeature(image1);
        descripteur2=detectionFeature(image2);
        //on trouve les bons matchs
        bonMatches=matcher(descripteur1,descripteur2);
        //calcul de la matrice fondamentale
        F=matriceFondamentale(pointcle1,pointcle2,bonMatches);
        //convertion de cette matrice en matrice de double
        Mat F2;
        F.convertTo(F2,CV_64F);
        cout<<"fondamental matrix ok"<<endl;
        //calcul de la matrice essentielle
        E=matriceEssentielle(F2,K);
        cout<<"essential ok"<<endl;
        //on garde les bons matchs des images 1 et 2
        for(int i = 0; i<bonMatches.size(); i++ )
        {
            // image de gauche
            points1.push_back(pointcle1[bonMatches[i].queryIdx].pt);
            // image de droite
            points2.push_back(pointcle2[bonMatches[i].trainIdx].pt);
        }

        //on calcule les matrices de rotations et translations
        recoverPose(E,points1,points2,R,T,focal,pp,noArray());
        cout<<"pose ok"<<endl;
        //on calcule P et P' selon H&Z
        calculCamMat(P1,P2,R,T,K);
        cout<<"intermediaire fini"<<endl;

}

Mat traitementFinal(Mat image1,Mat image2, Mat K, double focal, Point2d pp,Mat& P1, Mat& P2, Mat& R, Mat& T,Point2d pointImg1,Point2d pointImg2){

    Mat vect1=Mat(3,1,CV_64F);
    Mat vect2=Mat(3,1,CV_64F);
    cout<<"vectdir ok"<<endl;
    traitementIntermediaire(image1,image2, K, focal, pp, P1, P2, R, T);

    //calcul des vecteurs directeurs
    vect1=calculVecteur(pointImg1,P1);
    vect2=calculVecteur(pointImg2,P2);
    cout<<"vectdir ok"<<endl;

    return (intersection (vect1,vect2,P1,P2));

}

