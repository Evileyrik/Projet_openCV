#include <stdio.h>
#include <iostream>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;


Mat detectionFeature (Mat image){

    int nbPoints = 400;//détermine le nombre de points d'interets

    //detection des points cle
    OrbFeatureDetector detecteur( nbPoints );
    std::vector<KeyPoint> pointsCle;
    detecteur.detect( image, pointsCle );

    //Calcul les zones cle de description
    OrbDescriptorExtractor extracteur;
    Mat descripteur;
    extracteur.compute(image,pointsCle,descripteur);

    return descripteur;

}
std::vector<KeyPoint> getPointCle (Mat image){

    int nbPoints = 400;//détermine le nombre de points d'interets

    //detection des points cle
    OrbFeatureDetector detecteur( nbPoints );
    std::vector<KeyPoint> pointsCle;
    detecteur.detect( image, pointsCle );

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
        if (matches[i].distance<=2.5*dmin)
            bonMatches.push_back(matches[i]);
    }

    return bonMatches;

}


