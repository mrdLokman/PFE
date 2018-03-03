#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Histogramme.h"
#include "classes/Selector.h"
#include "Pretraitement.h"
#include "Tests.h"
#include"display.h"

#include <iostream>
using namespace std;
using namespace cv;

//*******************************************************************img source
 string cars = "C:/Users/lool/Desktop/PFE/dataset image/cars/*.bmp";
//*****************************************************************


 int main() {
	 cv::String path(cars); //select only bmp
	 vector<cv::String> fn;
	 vector<cv::Mat>windows;
	 cv::glob(path, fn, true); // recurse
	 for (size_t k = 0; k < fn.size(); ++k)
	 {
		 cv::Mat src = cv::imread(fn[k],0);
		 
		 if (src.empty()) continue; //only proceed if sucsessful		 
		 //cvtColor(src, src, CV_BGR2GRAY);
		 //-----------------------------------------------------------------
		 Selector selector;
		 //---------------------------------------------------crop the img src to get plaque
		 cv::Point* points = selector.getPlaqueCorners(src);
		 if (points == NULL)return -1;
		 int s = 0;
		 for (size_t i = 0; i < 4; i++)
		 {
			 s += points[i].x;
			 cout << points[i].x << "," << points[i].y << endl;
		 }
		 if (s == 0)continue;

		 Mat p = imread(fn[k], 0);
		 line(p, points[0], points[1], Scalar(0, 0, 255));
		 line(p, points[1], points[2], Scalar(0, 0, 255));
		 line(p, points[2], points[3], Scalar(0, 0, 255));
		 line(p, points[3], points[0], Scalar(0, 0, 255));
		 imshow("plaque", p);

		 // traitement de la plaque 

		 //1-extraction de la Plaque
		 Mat crop = extractionDePlaque(src, points);
		 //imshow("crop", crop);
		 windows.push_back(crop);
		 //2-rotation plaque
		 Mat rotCrop = correctionRotation(crop, points);
		// imshow("rot", rotCrop);
		 windows.push_back(rotCrop);
		 //3-ajuter la plaque (rotation)
		 Mat ajustee = correctionInclinaisonEliminerBordure(rotCrop);
		// imshow("ajustee", ajustee);
		 windows.push_back(ajustee);

		 //4-elimination bordure


		 //5-normalisation egalisation d'histogramme
			//calcule histogramme source
		 MatND hist;
		 Mat histImg;
		 int nbins = 256;
		 int hsize[] = { nbins };
		 float range[] = { 0, 255 };
		 const float *ranges[] = { range };
		 int chnls[] = { 0 };

		 calcHist(&ajustee, 1, chnls, Mat(), hist, 1, hsize, ranges);
		 histImg = imgHist(hist, 3, 3);
		// imshow("hist source", histImg);
		 windows.push_back(histImg);
			//normalisation
		 Mat normCrop = NormImage(ajustee);
		 //imshow("norm", normCrop);
		 windows.push_back(normCrop);
			//calcule histogramme image normalisée
		 calcHist(&normCrop, 1, chnls, Mat(), hist, 1, hsize, ranges);
		 histImg = imgHist(hist, 3, 3);
		// imshow("hist resultant", histImg);
		 windows.push_back(histImg);
		 
		 //6-binarisation
		 Mat binaire_eq_hist_gauss = binarisation(normCrop,B_A_GAUSS,Blur_GAUSS);
		// imshow("hist-gauss", binaire_eq_hist_gauss);
		 windows.push_back(binaire_eq_hist_gauss);
		 Mat hist_normalisee = normalisation(binaire_eq_hist_gauss, 50);
		 //imshow("hist-normalisee", hist_normalisee);

		 //Mat binaire_eq_hist_otsu = binarisation(normCrop, B_G_OTSU, Blur_GAUSS);
		 //imshow("hist-otsu", binaire_eq_hist_otsu);

		 Mat binaire_gauss = binarisation(ajustee, B_A_GAUSS, Blur_GAUSS);
		 //imshow("gauss", binaire_gauss);
		 windows.push_back(binaire_gauss);
		 Mat normalisee = normalisation(binaire_gauss, 50);
		 //imshow("normalisee", normalisee);

		 imwrite("input/binaire.png", binaire_gauss);
		 
		 //Mat binaire_otsu = binarisation(ajustee, B_A_GAUSS, Blur_GAUSS);
		 //imshow("otsu", binaire_otsu);
		
		 testSegmentationProjection("input/binaire.png");
		 testSegmentationACC("input/binaire.png");
		 imshow("pretraitement", display_images(windows, 400, 6));
		 waitKey(0);
		 cvDestroyWindow("plaque");
		 cvDestroyWindow("crop");
		 cvDestroyWindow("rot");
		 cvDestroyWindow("ajustee");
		 cvDestroyWindow("norm");
		 cvDestroyWindow("hist source");
		 cvDestroyWindow("hist resultant");
		 cvDestroyWindow("hist-gauss");
		 //cvDestroyWindow("hist-otsu");
		 //cvDestroyWindow("hist-normalisee");
		 cvDestroyWindow("gauss");
		 //cvDestroyWindow("otsu");
		 //cvDestroyWindow("normalisee");
		 
	 }
	 

 }





