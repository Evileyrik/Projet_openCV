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


//calcule les matrice cam selon formules de H&Z
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

//on calcul un vecteur directeur à l'aide de la relation dans H&Z x=PX.
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
