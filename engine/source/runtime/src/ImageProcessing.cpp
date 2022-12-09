#include "../include/ImageProcessing.h"
namespace EasyEngine {
Mat ImageProcessing::readImageByPath(const string& imagePath, int colorFlag) {
	Mat img = imread(imagePath, colorFlag);
	if (img.empty()) {
		cout << "read image failed: " + imagePath << endl;
	}
	return img;
}
}