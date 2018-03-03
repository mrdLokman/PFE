#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Histogramme.h"
#include "classes/Selector.h"
#include "Pretraitement.h"
#include "Tests.h"
#include"display.h"
#include <vector>
#include <iostream>
using namespace std;
using namespace cv;

//*******************************************************************img source
 string cars = "C:/Users/lool/Desktop/PFE/dataset image/cars/*.bmp";
//*****************************************************************
 std::vector<cv::Mat> MarkCountours(Mat binaryImg)
 {
	 vector<vector<Point> > contours;
	 Mat test = binaryImg.clone();
	 vector<Vec4i> hierarchy;
	 RNG rng(12345);
	 findContours(test, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	 vector<vector<Point> > contours_poly(contours.size());
	 vector<Rect> boundRect(contours.size());
	 vector<Point2f>center(contours.size());
	 vector<float>radius(contours.size());
	 int size = contours.size();
	 for (int i = 0; i <size ; i++)
	 {
		 approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		 boundRect[i] = boundingRect(Mat(contours_poly[i]));
	 }
	 Mat drawing = Mat::zeros(binaryImg.size(), CV_8UC3);
	 
	
	 for (int i = 0; i < size; i++)
	 {
		 Scalar color = Scalar(255, 0, 255);
		 drawContours(drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		 rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
		 
	 }
	 std::vector<cv::Mat> contour_rois;
	 for (int i = 0; boundRect.size(); i++)
	 {
		 cv::Mat roi(binaryImg, boundRect[i]);
		 contour_rois.push_back(roi);
	 }

	 return contour_rois;
 }
 int main()
 {
	 int largest_area = 0;
	 int largest_contour_index = 0;
	 Rect bounding_rect;

	 Mat src = imread("C:/Users/lool/PFE/PFE/output/segACC/000000.png", 1); //Load source image
	 Mat thr(src.rows, src.cols, CV_8UC1);
	 Mat dst(src.rows, src.cols, CV_8UC1, Scalar::all(0));
	 cvtColor(src, thr, CV_BGR2GRAY); //Convert to gray
	 threshold(thr, thr, 25, 255, THRESH_BINARY); //Threshold the gray

	 vector<vector<Point>> contours; // Vector for storing contour
	 vector<Vec4i> hierarchy;

	 findContours(thr, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); // Find the contours in the image

	 for (int i = 0; i< contours.size(); i++) // iterate through each contour. 
	 {
		 double a = contourArea(contours[i], false);  //  Find the area of contour
		 if (a>largest_area) {
			 largest_area = a;
			 largest_contour_index = i;                //Store the index of largest contour
			 bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
		 }

	 }

	 Scalar color(255, 255, 255);
	 drawContours(dst, contours, largest_contour_index, color, CV_FILLED, 8, hierarchy); // Draw the largest contour using previously stored index.
	 rectangle(src, bounding_rect, Scalar(0, 255, 0), 1, 8, 0);
	 imshow("src", src);
	 imshow("largest Contour", dst);
	 waitKey(0);
 }
 int main2()
 {
	 Mat image;
	 image = imread("C:/Users/lool/PFE/PFE/output/segACC/000000.png", 1);
	 
	 namedWindow("Display window", CV_WINDOW_AUTOSIZE);
	 imshow("Display window", image);
	 Mat gray;
	 cvtColor(image, gray, CV_BGR2GRAY);
	 Canny(gray, gray, 100, 200, 3);
	 MarkCountours(gray);
	 cout << MarkCountours(gray).size();
	/// Find contours   
	 vector<vector<Point> > contours;
	 vector<Vec4i> hierarchy;
	 RNG rng(12345);
	 findContours(gray, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	 /// Draw contours
	 Mat drawing = Mat::zeros(gray.size(), CV_8UC3);
	 for (int i = 0; i< contours.size(); i++)
	 {
		 Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		 drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
	 }

	 imshow("Result window", drawing);
	 waitKey(0);
	 return 0;
 }
 
/*
 int main() {
	 cv::String path(cars); //select only bmp
	 vector<cv::String> fn;
	 vector<cv::Mat>windows;
	 cv::glob(path, fn, true); // recurse
	 for (size_t k = 0; k < fn.size(); ++k)
	 {
		 cv::Mat src = cv::imread(fn[k],0);
		 windows.clear();
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
		 windows.push_back(normalisee);
		 //imshow("normalisee", normalisee);

		 imwrite("input/binaire.png", binaire_gauss);
		 
		 //Mat binaire_otsu = binarisation(ajustee, B_A_GAUSS, Blur_GAUSS);
		 //imshow("otsu", binaire_otsu);
		
		// testSegmentationProjection("input/binaire.png");
		 imshow("segmentation projection", display_images(testSegmentationProjection(binaire_gauss),50,1));
		 //testSegmentationACC("input/binaire.png");
		 imshow("segmentation CCA", display_images(testSegmentationACC(binaire_gauss), 50, 1));
		 imshow("pretraitement", display_images(windows, 400, 6));
		 waitKey(0);
		 cvDestroyWindow("pretraitement");
		 cvDestroyWindow("segmentation CCA");
		 cvDestroyWindow("segmentation projection");
		 cvDestroyWindow("plaque");
		/* cvDestroyWindow("plaque");
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
	 

 }*/





