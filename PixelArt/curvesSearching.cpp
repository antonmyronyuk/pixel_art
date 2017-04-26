#include "depixelizing.h"

std::list<Vertex>::iterator Depixelizing::searchCurve(
	std::list<Vertex>::iterator it,
	std::list<Vertex>::iterator it2) {

	std::list<Vertex>::iterator newIt = it;
	for (auto vit = m_edges.begin(); vit != m_edges.end(); ++vit) {
		if (vit->v1->edges.size() == 2 && vit->v1->f == false && vit->v2 == it) {
			vit->v1->f = true;
			vit->v2->f = true;
			newIt = vit->v1;
			break;

		} else if (vit->v1->edges.size() > 2 && vit->v2 == it && vit->v1 != it2) {
			vit->v1->f = true;
			vit->v2->f = true;
			newIt = vit->v1;
			break;

		} else if (vit->v2->edges.size() == 2 && vit->v2->f == false && vit->v1 == it) {
			vit->v2->f = true;
			vit->v1->f = true;
			newIt = vit->v2;
			break;

		} else if (vit->v2->edges.size() > 2 && vit->v1 == it &&  vit->v2 != it2) {
			vit->v2->f = true;
			vit->v1->f = true;
			newIt = vit->v2;
			break;

		}
		//else continue;
		//std::cout<<it->edges.()<<std::endl;
	}
	return newIt;

}

void Depixelizing::findAllCurves() {

	for (auto it = m_vertexes.begin(); it != m_vertexes.end(); ++it) {
		it->f = false; //all vertexes are not visited at the beginning
	}
	m_curvesVertexes.clear();
	m_curvesVertexes.push_back(std::list<std::list<Vertex>::iterator>());

	for (auto it = m_vertexes.begin(); it != m_vertexes.end(); ++it) {

		if (it->edges.size() == 2 && it->f == false) {	

			std::list<Vertex>::iterator vit1 = it;
			std::list<Vertex>::iterator vit2 = it;
			vit2 = searchCurve(it, it);
			if (vit2 != vit1) {

				vit1->f = true;
				vit2->f = true;
				m_curvesVertexes.back().push_front(vit1);
				m_curvesVertexes.back().push_back(vit2);

				//cv::circle(img, cv::Point(cvRound(vit1 -> x * scale + 5), cvRound(vit1 -> y * scale + 5)), 1, cv::Scalar(110, 110, k * 10), 2);
				//cv::circle(img, cv::Point(cvRound(vit2 -> x * scale + 5), cvRound(vit2 -> y * scale + 5)), 1, cv::Scalar(110, 110, k * 10), 2);

				bool found = true;
				if (vit1->edges.size() == 2) {
					while (found) {
						auto preVit = vit1;
						vit1 = searchCurve(vit1, vit2);
						if (vit1 != preVit && vit1->edges.size() == 2) {
							m_curvesVertexes.back().push_front(vit1);
							//cv::circle(img, cv::Point(cvRound(vit1 -> x * scale + 5), cvRound(vit1 -> y * scale + 5)), 1, cv::Scalar(110, 110, k * 10), 2);

						} else if (vit1 != preVit && vit1->edges.size() > 2) {
							m_curvesVertexes.back().push_front(vit1);
							found = false;

						} else {
							found = false; 
						}
					}
				}

				found = true;
				if (vit2->edges.size()==2) {
					while (found) {
						auto preVit = vit2;
						vit2 = searchCurve(vit2, vit1);
						if (vit2 != preVit && vit2->edges.size() == 2) {
							m_curvesVertexes.back().push_back(vit2);
							//cv::circle(img, cv::Point(cvRound(vit2 -> x * scale + 5), cvRound(vit2 -> y * scale + 5)), 1, cv::Scalar(110, 110, k * 100), 2);

						} else if (vit2 != preVit && vit2->edges.size() > 2) {
							m_curvesVertexes.back().push_back(vit2);
							found = false;

						} else {
							found = false; 
						}
					}
				}

				m_curvesVertexes.push_back(std::list<std::list<Vertex>::iterator>());
			}
		}
	}

	
	for (auto vit = m_edges.begin(); vit != m_edges.end(); ++vit) {
		if (vit->v1->edges.size() > 2 && vit->v2->edges.size() > 2) { 
			//curves that contain only two vertexes
			m_curvesVertexes.back().push_back(vit->v1);
			m_curvesVertexes.back().push_back(vit->v2);
			m_curvesVertexes.push_back(std::list<std::list<Vertex>::iterator>());
		}
	}
	m_curvesVertexes.pop_back(); //empty list
}