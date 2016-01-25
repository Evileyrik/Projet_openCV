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
    //Prend en entrée un image, et en retourne les points caractéristiques avec leur environnement.
    int nbPoints = 3000;//détermine le nombre de points d'interets

    //detection des points cles
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
    // Pareil que celle d'avant, mais ne retourne que les points clés, sans environnement

    int nbPoints = 3000;//détermine le nombre de points d'interets

    //detection des points cle
    Ptr<FeatureDetector> detecteur = ORB::create();
    std::vector<KeyPoint> pointsCle;
    detecteur->detect( image, pointsCle );

    return pointsCle;

}

 std::vector< DMatch > matcher (Mat descripteur1,Mat descripteur2){
    //renvoie les indices des "bons" points uniquement

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
        if (matches[i].distance<=2*dmin) //2*distance minimale est fixé arbitrairement
            bonMatches.push_back(matches[i]);
    }

    return bonMatches;

}

Mat getHomography(Mat image1,Mat image2){
    // Forme la matrice d'homographie et la retourne

   //bons matches
    std::vector<DMatch> bonMatches;
    std::vector<KeyPoint> pointcle2,pointcle1;
    //descriptor
    Mat descripteur1,descripteur2;
    vector<Point2f> points1,points2;

    pointcle1=getPointCle(image1);
    pointcle2=getPointCle(image2);
    descripteur1=detectionFeature(image1);
    descripteur2=detectionFeature(image2);
    bonMatches=matcher(descripteur1,descripteur2);



    for(int i = 0; i<bonMatches.size(); i++ )
    {
        // image de gauche
        points1.push_back(pointcle1[bonMatches[i].queryIdx].pt); //image1
        // image de droite
        points2.push_back(pointcle2[bonMatches[i].trainIdx].pt); //Image2
    }

    Mat H = findHomography(points1,points2,CV_RANSAC,1,noArray(),4000,0.995);
    return H;
}



void traitement(Point2f* point,Mat* image1,Mat* image2){
    //Affiche le point tracké

    float diff=80;//déplacement maximum toléré
    Point2f point2 = Point2f (point->x,point->y);
    std::vector<Point2f> vecteur1;
    std::vector<Point2f> vecteur2;

    Mat H = getHomography(*image1,*image2);

    vecteur1.clear();
    vecteur1.push_back(point2);
    vecteur2.clear();

    if ((H.rows!=0)&&(H.cols!=0)){

        perspectiveTransform(vecteur1,vecteur2,H);

       // if ((abs(point2.x-vecteur2[0].x)<=diff)&&(abs(point2.y-vecteur2[0].y)<=diff)){
            point=new Point2f;
            *point=vecteur2[0];
       // }

    }

    circle(*image2,*point,5,CV_RGB(0,0,255),5,LINE_8,0);
    imshow("image2",*image2);
    image1=new Mat;
    *image1=*image2;

}



