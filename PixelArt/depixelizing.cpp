#include "depixelizing.h"
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

Depixelizing::Depixelizing() :
	m_scale(20) {
}

Depixelizing::~Depixelizing() {
}

void Depixelizing::depixelize(const cv::Mat& image, float scaleFactor) {
	//initialization
	m_width = image.cols;
	m_height = image.rows;
	m_image = image.clone();
	m_scale = scaleFactor;

	//depixelizing
	buildSimilarityGraph();
	showSimilarityGraph();
	deleteFullyConnectedBlocks();
	showSimilarityGraph();
}

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

void Depixelizing::showSimilarityGraph() {
	int scale = int(m_scale);
	cv::Mat graph(scale * m_height, scale * m_width, CV_8UC3, cvScalar(0,0,0));

	for (int i = 0; i < m_height; ++i) {
		for (int j = 0; j < m_width; ++j) {
			if (m_connections[i * m_width + j] & NEIGHBOUR_BOTTOM) {
				cv::line(graph, cvPoint( (scale + 1) / 2 + j * scale, (scale + 1) / 2 + i * scale), cvPoint( (scale + 1) / 2 + j * scale, (scale + 1) / 2 + (i + 1) * scale), cvScalar(255,255,255));

			}

			if (m_connections[i * m_width + j] & NEIGHBOUR_BOTTOM_RIGHT) {
				cv::line(graph, cvPoint( (scale + 1) / 2 + j * scale, (scale + 1) / 2 + i * scale), cvPoint((scale + 1) / 2 + (j + 1) * scale, (scale + 1) / 2 + (i + 1) * scale), cvScalar(255,255,255));
				
			}

			if (m_connections[i * m_width + j] & NEIGHBOUR_BOTTOM_LEFT) {	
				cv::line(graph, cvPoint( (scale + 1) / 2 + j * scale, (scale + 1) / 2 + i * scale), cvPoint((scale + 1) / 2 + (j - 1) * scale, (scale + 1) / 2 + (i + 1) * scale), cvScalar(255,255,255));
				
			}

			if (m_connections[i * m_width + j] & NEIGHBOUR_RIGHT) {
				cv::line(graph, cvPoint( (scale + 1) / 2 + j * scale, (scale + 1) / 2 + i * scale), cvPoint((scale + 1) / 2 + (j + 1) * scale, (scale + 1) / 2 + i * scale), cvScalar(255,255,255));
				
			}

			if (m_connections[i * m_width + j] & NEIGHBOUR_LEFT) {
				cv::line(graph, cvPoint( (scale + 1) / 2 + j * scale, (scale + 1) / 2 + i * scale), cvPoint((scale + 1) / 2 + (j - 1) * scale, (scale + 1) / 2 + i * scale), cvScalar(255,255,255));
				
			}

			if (m_connections[i * m_width + j] & NEIGHBOUR_TOP_RIGHT) {
				cv::line(graph, cvPoint( (scale + 1) / 2 + j * scale, (scale + 1) / 2 + i * scale), cvPoint((scale + 1) / 2 + (j + 1) * scale, (scale + 1) / 2 + (i - 1) * scale), cvScalar(255,255,255));
				
			}

			if (m_connections[i * m_width + j] & NEIGHBOUR_TOP) {
				cv::line(graph, cvPoint( (scale + 1) / 2 + j * scale, (scale + 1) / 2 + i * scale), cvPoint((scale + 1) / 2 + j * scale, (scale + 1) / 2 + (i - 1) * scale), cvScalar(255,255,255));
			
			}

			if (m_connections[i * m_width + j] & NEIGHBOUR_TOP_LEFT) {
				cv::line(graph, cvPoint( (scale + 1) / 2 + j * scale, (scale + 1) / 2 + i * scale), cvPoint((scale + 1) / 2 + (j - 1) * scale, (scale + 1) / 2 + (i - 1) * scale), cvScalar(255,255,255));
				
			}

		}
	}

	for (int i = 0; i < m_height; ++i) {
		for (int j = 0; j < m_width; ++j) {
				cv::circle(graph, cv::Point(cvRound((scale + 1) / 2 + j * scale), cvRound((scale + 1) / 2 + i * scale)), 1, cv::Scalar(0, 255, 255), 2);
		}
	}

	cv::imshow("SimilarityGraph", graph);
	cv::waitKey(0);


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