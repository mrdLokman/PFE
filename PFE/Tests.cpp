#include "Tests.h"
#include "Pretraitement.h"
#include "Segmentation.h"
#include "classes\Composant.h"

void testExtractionDePlaque(string imgSource, string imgDest, Point* coins) {
	Mat src = imread(imgSource, 1);
	
	Mat dest = extractionDePlaque(src, coins);

	imwrite(imgDest, dest);
}

void testCorrectionRotation(string imgSource, string imgDest, Point* coins) {
	Mat src = imread(imgSource, 1);
	
	Mat correcte = correctionRotation(src, coins);

	imwrite(imgDest, correcte);

}

void testCorrectionInclinaison(string imgSource, string imgDest) {
	Mat src = imread(imgSource, 0);
	Mat correcte = correctionInclinaison(src);
	imwrite(imgDest, correcte);
}

void testcorrectionInclinaisonEliminerBordure(string imgSource, string imgDest) {
	Mat src = imread(imgSource, 0);
	Mat correcte = correctionInclinaisonEliminerBordure(src);
	imwrite(imgDest, correcte);
}

void testSegmentationProjection(string image) {
	string distination = "output/segProj/";
	Mat src = imread(image, 0);
	imwrite("output/segProj/000000.png", src);

	vector<Composant> composants = segmentationProjection(src);

	int s = composants.size();
	for (int i = 0; i < s; i++){
		imwrite(distination + to_string(i) + ".png", composants.at(i).data);
	}
}

void testSegmentationACC(string image) {
	string distination = "output/segACC/";
	Mat src = imread(image, 0);
	imwrite("output/segACC/000000.png", src);

	vector<Composant> composants = segmentationACC(src);

	int s = composants.size();
	for (int i = 0; i < s; i++) {
		imwrite(distination + to_string(i) + ".png", composants.at(i).data);
	}
}

void testHistogrammeProjection(string image) {
	histogrammeImage(image);
}
