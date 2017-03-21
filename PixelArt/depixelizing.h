#ifndef DEPIXELIZING_H
#define DEPIXELIZING_H

#include <opencv2\opencv.hpp>
#include <vector>
#include <list>
#include <math.h>
#include <time.h>

#define NEIGHBOUR_TOP 1
#define NEIGHBOUR_TOP_RIGHT 2
#define NEIGHBOUR_RIGHT 4
#define NEIGHBOUR_BOTTOM_RIGHT 8
#define NEIGHBOUR_BOTTOM 16
#define NEIGHBOUR_BOTTOM_LEFT 32
#define NEIGHBOUR_LEFT 64
#define NEIGHBOUR_TOP_LEFT 128


class Depixelizing {

public:

	Depixelizing();
	~Depixelizing();

	void depixelize(const cv::Mat& , float); // main method of the class

private:

	int m_width; // image width
	int m_height; // image height
	cv::Mat m_image; // input image
	float m_scale; // scale factor
	std::vector<unsigned char> m_connections; // similarity graph
											  // (i, j) -> (i * m_w + j)

	//building
	void buildSimilarityGraph();
	void deleteFullyConnectedBlocks();

	//visualization
	void showSimilarityGraph();
};

#endif //DEPIXELIZING_H