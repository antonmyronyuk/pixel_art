#include "depixelizing.h"

#include  <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <string>

int main()
{
	std::string fileName = "1.png";
	std::cout << "Enter pixel art file name: ";
	std::cin >> fileName;
	cv::Mat img = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);
	cv::Mat input;
	if (img.empty())
	{
		std::cout << "ERROR: failed to load image" << std::endl;
		return -1;
	}

	std::cout << "Enter scale factor: ";
	float scale = 20;
	std::cin >> scale;
	if (scale < 2.f) {
		scale = 2.f;
	}
	cv::resize(img, input, input.size(), scale, scale, CV_INTER_NN); 
	cv::imshow("Input image X scale factor", input);
	cv::waitKey(0);

	Depixelizing pix;
	pix.depixelize(img, scale);

	

	return 0;
}