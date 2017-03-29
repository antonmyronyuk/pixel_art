#include "depixelizing.h"

void Depixelizing::buildSimilarityGraph() {
	cv::Mat imgYUV(m_image.size(), CV_8UC3);
	cv::cvtColor(m_image, imgYUV, CV_BGR2YUV);

	m_connections.resize(m_width * m_height);
	//building
	for (int i = 0; i < m_height; ++i) {
		for (int j = 0; j < m_width; ++j) {

			cv::Vec3b& currentPixel = imgYUV.at<cv::Vec3b>(i, j);

			//TOP
			if (i > 0) {
				cv::Vec3b& neighbourPixel = imgYUV.at<cv::Vec3b>(i - 1, j);
				if (abs(currentPixel[0] - neighbourPixel[0]) < 48 && abs(currentPixel[1] - neighbourPixel[1]) < 7 && abs(currentPixel[2] - neighbourPixel[2]) < 6) {
					m_connections[i * m_width + j] |= NEIGHBOUR_TOP; 
				}
			}

			//TOP_RIGHT
			if (j < m_width - 1 && i > 0) {
				cv::Vec3b& neighbourPixel = imgYUV.at<cv::Vec3b>(i - 1, j + 1);
				if (abs(currentPixel[0] - neighbourPixel[0]) < 48 && abs(currentPixel[1] - neighbourPixel[1]) < 7 && abs(currentPixel[2] - neighbourPixel[2]) < 6) {
					m_connections[i * m_width + j] |= NEIGHBOUR_TOP_RIGHT; 
				}
			}

			//RIGHT
			if (j < m_width - 1) {
				cv::Vec3b& neighbourPixel = imgYUV.at<cv::Vec3b>(i, j + 1);
				if (abs(currentPixel[0] - neighbourPixel[0]) < 48 && abs(currentPixel[1] - neighbourPixel[1]) < 7 && abs(currentPixel[2] - neighbourPixel[2]) < 6) {
					m_connections[i * m_width + j] |= NEIGHBOUR_RIGHT; 
				}
			}

			//BOTTOM_RIGHT
			if (j < m_width - 1 && i < m_height - 1) {
				cv::Vec3b& neighbourPixel = imgYUV.at<cv::Vec3b>(i + 1, j + 1);
				if (abs(currentPixel[0] - neighbourPixel[0]) < 48 && abs(currentPixel[1] - neighbourPixel[1]) < 7 && abs(currentPixel[2] - neighbourPixel[2]) < 6) {
					m_connections[i * m_width + j] |= NEIGHBOUR_BOTTOM_RIGHT; 
				}
			}

			//BOTTOM
			if (i < m_height - 1) {
				cv::Vec3b& neighbourPixel = imgYUV.at<cv::Vec3b>(i + 1, j);
				if (abs(currentPixel[0] - neighbourPixel[0]) < 48 && abs(currentPixel[1] - neighbourPixel[1]) < 7 && abs(currentPixel[2] - neighbourPixel[2]) < 6) {
					m_connections[i * m_width + j] |= NEIGHBOUR_BOTTOM; 
				}
			}

			//BOTTOM_LEFT
			if (j > 0 && i < m_height - 1) {
				cv::Vec3b& neighbourPixel = imgYUV.at<cv::Vec3b>(i + 1, j - 1);
				if (abs(currentPixel[0] - neighbourPixel[0]) < 48 && abs(currentPixel[1] - neighbourPixel[1]) < 7 && abs(currentPixel[2] - neighbourPixel[2]) < 6) {
					m_connections[i * m_width + j] |= NEIGHBOUR_BOTTOM_LEFT; 
				}
			}

			//LEFT
			if (j > 0) {
				cv::Vec3b& neighbourPixel = imgYUV.at<cv::Vec3b>(i, j - 1);
				if (abs(currentPixel[0] - neighbourPixel[0]) < 48 && abs(currentPixel[1] - neighbourPixel[1]) < 7 && abs(currentPixel[2] - neighbourPixel[2]) < 6) {
					m_connections[i * m_width + j] |= NEIGHBOUR_LEFT; 
				}
			}

			//TOP_LEFT
			if (j > 0 && i > 0)
			{
				cv::Vec3b& neighbourPixel = imgYUV.at<cv::Vec3b>(i - 1, j - 1);
				if (abs(currentPixel[0] - neighbourPixel[0]) < 48 && abs(currentPixel[1] - neighbourPixel[1]) < 7 && abs(currentPixel[2] - neighbourPixel[2]) < 6) {
					m_connections[i * m_width + j] |= NEIGHBOUR_TOP_LEFT; 
				}
			}

		}
	}

}


void Depixelizing::deleteFullyConnectedBlocks() {
	for (int i = 0; i < m_height - 1; ++i) {
		for (int j = 0; j < m_width - 1; ++j) {

			if ((m_connections[i * m_width + j] & NEIGHBOUR_RIGHT) && 
				(m_connections[i * m_width + j] & NEIGHBOUR_BOTTOM) &&
				(m_connections[(i + 1) * m_width + j] & NEIGHBOUR_RIGHT) && 
				(m_connections[i * m_width + (j + 1)] & NEIGHBOUR_BOTTOM)) {

					m_connections[i * m_width + j] &= ~NEIGHBOUR_BOTTOM_RIGHT;
					m_connections[(i + 1) * m_width + (j + 1)] &= ~NEIGHBOUR_TOP_LEFT;
					m_connections[(i + 1) * m_width + j] &= ~NEIGHBOUR_TOP_RIGHT;
					m_connections[i * m_width + (j + 1)] &= ~NEIGHBOUR_BOTTOM_LEFT;
			}
		}
	}
}