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

int main1() {

	Point* coins = new Point[4];
	coins[0].x = 123; coins[0].y = 362;
	coins[1].x = 226; coins[1].y = 357;
	coins[2].x = 226; coins[2].y = 382;
	coins[3].x = 124; coins[3].y = 388;
	
	string source = "input/source.bmp";

	//testExtractionDePlaque(source, "output/plaque.png", coins);

	//testCorrectionRotation("input/plaque_rotation.png", "output/correction_rotation.png", coins);

	//testCorrectionInclinaison("input/binaire.png", "output/correction_inclinaison.png");
	//testCorrectionInclinaisonEliminerBordure("input/rot2.png", "output/correction_inclinaison.png");

	//test();
	//testHistogrammeProjection("input/test.png");


	testSegmentationProjection("input/binaire.png");

	waitKey(0);
	system("pause");
	return 0;
}
