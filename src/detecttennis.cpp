/* Mat=tableau de pixels
    IplImage=image chargee en memoire */

#include "detecttennis.h"

using namespace cv; // Specifique a OpenCV
using namespace std;

DetectTennis::DetectTennis()
{
    cap=0;
    erreur=0;
    if(!cap.isOpened()) // Erreur d’ouverture de la camera (pas branchee, pilote non charge...)
    {
        cout << "Impossible d'ouvrir la camera !" << endl;
        erreur=1;
    }
}

DetectTennis::~DetectTennis()
{
}

bool DetectTennis::detect()
{
    Mat img;
    cap >> img; // On prend la photo
    img=rotateArr(img, 180); // La camera est a l’envers donc on pivote de 180°
    imgsize = img.size(); // On regarde la taille de l’image

    IplImage *hsv = cvCreateImage(imgsize, IPL_DEPTH_8U, 3);
    cvtColor(img, (Mat)hsv, CV_BGR2HSV);
    Mat mask = cvCreateMat(imgsize.height, imgsize.width, CV_8UC1); // Masque : seconde image avec en blancs les pixels qui etaient dans une certaine fourchette et en noir les autres
    inRange((Mat)hsv, Scalar(23,41,133), Scalar(40,150,255), mask); // La fourchette en question

    Mat se21 = getStructuringElement(MORPH_RECT, Size(21, 21), Point(10, 10));
    Mat se11 = getStructuringElement(MORPH_RECT, Size(11, 11), Point(5, 5));
    cvClose(mask, mask, se21); // Traitements techniques necessaires a l’algorithme
    cvOpen(mask, mask, se11);

    IplImage *hough_in = cvCreateImage(imgsize, 8, 1);
    mask.copyTo((Mat)hough_in);
    GaussianBlur((Mat)hough_in, (Mat)hough_in, Size(15, 15), 0, 0); // On lisse le masque grace a une fonction gaussienne

    HoughCircles((Mat)hough_in, circles, CV_HOUGH_GRADIENT, 2, imgsize.height/10, 81, 29, imgsize.height/200, 0); // On detecte les cercles

    img.release(); // On detruit les variables pour faire de la place en memoire
    mask.release();
    cvReleaseImage(&hsv);
    cvReleaseImage(&hough_in);

    if(circles.size()==0) // Pas de balle trouvee
    {
        return false;
    }
    else // Une balle trouvee
    {
        return true;
    }
}

vector<pair<int, int>> DetectTennis::listBalles()
{
    unsigned int i;
    float distance;
    int deg;
    vector<pair<int, int>> vectorRetour(circles.size()); // Voir les conteneurs en C++
    for(i = 0; i < circles.size(); i++)
    {
	deg = ((circles[i][0] – ((imgsize.width)/2))/imgsize.width)*30; // angle
	distance = 3.2/tan(circles[i][2]*0.942477796/imgsize.width); // distance par trigonometrie
	vectorRetour[i]=make_pair(distance, deg);
    }
    return vectorRetour;
}

void DetectTennis::cvOpen(Mat src, Mat dst, Mat element)
{
    erode(src, dst, element);
    dilate(src, dst, element);
}

void DetectTennis::cvClose(Mat src, Mat dst, Mat element)
{
    dilate(src, dst, element);
    erode(src, dst, element);
}

Mat DetectTennis::rotateArr(Mat src, double angle)
{
    Mat dst;
    Point2f pt(src.cols/2., src.rows/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, Size(src.cols, src.rows));
    return dst;
}

bool DetectTennis::pairCompare(const pair<int, int>&firstElem, const pair<int, int>&secondElem)
{
    return firstElem.first < secondElem.first;
}
