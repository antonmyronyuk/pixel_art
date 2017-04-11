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

void Depixelizing::showCellGraph() {
	const int scale = (int)m_scale;
	cv::Mat img(m_height * scale + 10, m_width * scale + 10, CV_8UC3, cv::Scalar(0,0,0));

	for (auto it = m_edges.begin(); it != m_edges.end(); ++it) {
		cv::line(img,
			cv::Point(cvRound(it->v1->x * scale + 5), cvRound(it->v1->y * scale + 5)),
			cv::Point(cvRound(it->v2->x * scale + 5), cvRound(it->v2->y * scale + 5)),
			cv::Scalar(255, 255, 255), 1);

		cv::circle(img, cv::Point(cvRound(it->v1->x * scale + 5), cvRound(it->v1->y * scale + 5)), 1, cv::Scalar(200, 255, 0), 2);
		cv::circle(img, cv::Point(cvRound(it->v2->x * scale + 5), cvRound(it->v2->y * scale + 5)), 1, cv::Scalar(200, 255, 0), 2);

	}

	cv::imshow("CellGraph", img);
	cv::waitKey(0);
}

void Depixelizing::showCellImage() {

	const float scale = m_scale;
	cv::Mat cellImage(m_height * scale, m_width * scale, CV_8UC3, cv::Scalar(0,0,0));

	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {

			size_t ind = y * m_width + x; // index of current cell

			std::vector <cv::Point> points;
			for (size_t i = 0; i < m_pixelsToCells[ind].size(); ++i) {

				points.push_back(cv::Point(static_cast<int>(m_pixelsToCells[ind][i]->x * scale),
										   static_cast<int>(m_pixelsToCells[ind][i]->y * scale)));
			}

			int vertexCount;
			vertexCount = m_pixelsToCells[ind].size();
			cv::Scalar c(m_image.at<cv::Vec3b>(y, x)[0], m_image.at<cv::Vec3b>(y, x)[1], m_image.at<cv::Vec3b>(y, x)[2]);
			cv::fillPoly(cellImage, (const cv::Point**)&points, &vertexCount, 1, c);

			points.clear();
		}
	}

	cv::imshow("cell_image", cellImage);
	cv::imwrite("cell_image.png", cellImage);
	cv::waitKey();

}