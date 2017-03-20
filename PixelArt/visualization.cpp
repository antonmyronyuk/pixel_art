#include "depixelizing.h"

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
