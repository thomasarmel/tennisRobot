#ifndef DETECTTENNIS_H_INCLUDED
#define DETECTTENNIS_H_INCLUDED

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

/** Compiler avec les options -lopencv_core -lopencv_highgui -lopencv_imgproc */

class DetectTennis
{
public:
    DetectTennis();
    ~DetectTennis();
    bool detect();
    std::vector<std::pair<int, int>> listBalles();
    static bool pairCompare(const std::pair<int, int>&firstElem, const std::pair<int, int>&secondElem); // On trie les balles par ordre décroissant de distance

private:
    void cvOpen(cv::Mat src, cv::Mat dst, cv::Mat element); // Adoucissement des traits
    void cvClose(cv::Mat src, cv::Mat dst, cv::Mat element);
    cv::Mat rotateArr(cv::Mat src, double angle); // On pivote l’image

    cv::VideoCapture cap; // Prendre une photo
    unsigned short erreur; // Une eventuelle erreur
    std::vector<cv::Vec3f> circles; // Liste des balles
    CvSize imgsize; // Taille de l’image
};

#endif // DETECTTENNIS_H_INCLUDED
