#include "depixelizing.h"
#include <iostream>

void Depixelizing::func() {
	const int scale = (int)m_scale;
	cv::Mat img(m_height * scale + 10, m_width * scale + 10, CV_8UC3, cv::Scalar(0,0,0));
	for (auto it = m_vertexes.begin(); it != m_vertexes.end(); ++it) {
		cv::circle(img, cv::Point(cvRound(it->x * scale + 5), cvRound(it->y * scale + 5)), 1, cv::Scalar(200, 255, 0), 2);
		cv::imshow("vertexes", img);
		std::cout << it->edges.size() << std::endl;
		cv::waitKey(0);
	}
}