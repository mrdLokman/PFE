#include "Tests.h"
#include "Pretraitement.h"


void test() {
	Mat src = imread("input/test.png", 0);
	vector<int> h = histogrammeProjectionHorizontal(src);
	
	int taille = h.size();
	cout << "taille = " << taille <<endl;
	int moy = moyenne(h);
	cout << "moyenne = " << moy << endl;
	int minG = minGauche(h, moy);
	cout << "minGauche = " << minG << endl;
	int minD = minDroit(h, moy);
	cout << "minDroit = " << minD << endl;

	int w = src.cols;
	for (int i = 0; i < w; i++)
	{
		src.at<unsigned char>(minG, i) = 128;
		src.at<unsigned char>(minD, i) = 128;
	}

	imshow("resultat", src);
	imwrite("input/resultat.png",src);
}
int testContours(string imageName) {
	Mat img;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	img = imread(imageName, CV_8UC1);

	/// Find contours
	findContours(img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	cout << contours.size();
	
	/// Draw contours
	Mat drawing = Mat::zeros(img.size(), CV_8UC3);
	RNG rng ;
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		Mat contourOut = Mat::zeros(img.size(), CV_8UC3);
		drawContours(contourOut, contours, i, color, 1, 8, hierarchy, 0, Point());
		drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());

		stringstream ss;
		ss << "output/contour" << i << ".png";
		imwrite(ss.str(), contourOut);
		contourOut = Mat::zeros(img.size(), CV_8UC3);
	}


	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);

	waitKey(0);
	return 0;
}



int main()
{
	testContours("C:/Users/lool/PFE/PFE/output/segACC/000000.png");
//	Mat src= imread("C:/Users/lool/PFE/PFE/output/segACC/000000.png", 0);
	
	
	return 0;
}
