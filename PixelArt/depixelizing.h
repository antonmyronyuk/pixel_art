#ifndef DEPIXELIZING_H
#define DEPIXELIZING_H

#include <opencv2\opencv.hpp>


class Depixelizing {
public:
	Depixelizing();
	~Depixelizing();

	void depixelize(const cv::Mat& , float); // main function of the class
private:
	int m_width; // image width
	int m_height; // image height
	cv::Mat m_image; // input image
	float m_scale; // scale factor
	std::vector<unsigned char> m_connections; // similarity graph
											  // (i, j) -> (i * m_w + j)

	void buildSimilarityGraph();
	void showSimilarityGraph();
	void deleteFullyConnectedBlocks();
};

#endif //DEPIXELIZING_H