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

    int nbPoints = 1000;//détermine le nombre de points d'interets

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

    int nbPoints = 3000;//détermine le nombre de points d'interets

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

    double dmax = 0; double dmin= 100;

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
        if (matches[i].distance<=3*dmin)
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

    Mat matrice = findFundamentalMat(points1,points2,FM_RANSAC, 4, 0.99);//méthode de calcul utilisant RANSAC, avec une erreur tolérée de 3 avec une proba de 99%
    return matrice;
}



Mat matriceEssentielle (Mat F, Mat K){
    Mat Kt;
    transpose(K,Kt);
    Mat E = Kt*F*K;
    return E;


}


//on calcule les matrice cam selon formules de H&Z
void calculCamMat(Mat& P1,Mat& P2,Mat R,Mat T){

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



}

//on calcule un vecteur directeur à l'aide de la relation dans H&Z x=PX que l'on peut mettre sous forme AX'=b ou A est 3x3 et X' = (X,Y,Z)
Mat calculVecteur(Point point,Mat mat){

    Mat matriceExtraite = Mat(3,3,CV_64F);
    Mat solution;
    //on extrait une matrice 3x3 de mat
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            matriceExtraite.at<double>(i,j)=mat.at<double>(i,j);
        }
    }

    Mat b = Mat(3,1,CV_64F);
    b.at<double>(0,0)= point.x-mat.at<double>(0,3);
    b.at<double>(1,0)=point.y-mat.at<double>(1,3);
    b.at<double>(2,0)=1-mat.at<double>(2,3);

    solve(matriceExtraite,b,solution,DECOMP_SVD);

    return solution;
}

//on calcule le cntre de la caméra à l'aide de la relation de H&Z
Mat calculCentre(Mat cameraMatrix){

    Mat m=Mat(3,3,CV_64F);
    Mat p=Mat(3,1,CV_64F);

    //extraction de la matrice de rotation
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


//on calcule la distance entre deux droites.
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
    Mat C=center+0.5*vectdir;
    double a = C.at<double>(0,0)-B.at<double>(0,0);
    double b = C.at<double>(1,0)-B.at<double>(1,0);
    double c = C.at<double>(2,0)-B.at<double>(2,0);

    Mat A=Mat(3,3,CV_64F);
    Mat D = Mat(3,1,CV_64F);

    A.at<double>(0,0)=a;
    A.at<double>(0,1)=b;
    A.at<double>(0,2)=c;
    A.at<double>(1,0)=b;
    A.at<double>(1,1)=-a;
    A.at<double>(1,2)=0;
    A.at<double>(2,0)=-c;
    A.at<double>(2,1)=0;
    A.at<double>(2,2)=a;

    D.at<double>(0,0)=a*point.at<double>(0,0)+b*point.at<double>(1,0)+c*point.at<double>(2,0);
    D.at<double>(1,0)=b*B.at<double>(0,0)-a*B.at<double>(1,0);
    D.at<double>(2,0)=a*B.at<double>(2,0)-c*B.at<double>(0,0);

    solve(A,D,projete,DECOMP_SVD);

    return projete;
}


