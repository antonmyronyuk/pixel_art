#include "depixelizing.h"

void Depixelizing::remove2ValenceNodes()
{
	auto it = m_vertexes.begin();

	//vertexes to delete
	std::vector<std::list<Vertex>::iterator> vert;
	vert.reserve(2);

	int i = 0;
	while (it != m_vertexes.end()) {

		auto it2 = it; //current vertex
		++it;

		//remove all 2 valence vertexes except of edge vertexes of image
		if  (it2->edges.size() == 2 &&
			(it2->y != 0 || it2->x != 0) &&
			(it2->y != 0 || it2->x != m_width) &&
			(it2->y != m_height || it2->x != 0) &&
			(it2->y != m_height || it2->x != m_width)) {

			vert.clear();
			int ind1 = -1; //default value
			int ind2 = -1; //default value
			for (auto ite = it2->edges.begin(); ite != it2->edges.end(); ++ite) {//for all edges connected with current vertex
				if ((*ite)->v1 == it2) {
					vert.push_back((*ite)->v2);
					(*ite)->v2->edges.remove(*ite);

					// TODO: select right indexes
					ind1 = (*ite)->ind1;
					ind2 = (*ite)->ind2;

					m_edges.erase(*ite);
				} else {
					vert.push_back((*ite)->v1);
					(*ite)->v1->edges.remove(*ite);

					// TODO: select right indexes
					ind1 = (*ite)->ind1;
					ind2 = (*ite)->ind2;

					m_edges.erase(*ite);
				}
			}
			
			if (vert.size() != 2) {
				continue;
			}
			
			m_vertexes.erase(it2);

			//add new edge
			Edge e;
			e.addEdge(ind1, ind2, vert[0], vert[1]);
			m_edges.push_back(e);

			auto eit = --m_edges.end();
			vert[0]->edges.push_back(eit);
			vert[1]->edges.push_back(eit);
		}
	}
}


void Depixelizing::reconstructPixelsToCells() {

	const float r = 0.8f;

	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {

			size_t ind = y * m_width + x;
			m_pixelsToCells[ind].clear();
			m_pixelsToCells[ind].reserve(10);
			

			//center
			float cx = x + 0.5f;
			float cy = y + 0.5f;

			// add all veretexes in cirlce with radius = 0.8 pixel 
			// and center in (cx, cy) to m_pixelsToCells[ind]

			auto vit = m_vertexes.end();
			for (auto it = m_vertexes.begin(); it != m_vertexes.end(); ++it) {
				if (sqrtf((cx - it->x) * (cx - it->x) + ((cy - it->y) * (cy - it->y))) < r) {
					vit = it;
					break;
				}
			}

			if (vit == m_vertexes.end()) {
				continue;
			}

			m_pixelsToCells[ind].push_back(vit);

			bool hasNext = true;

			while (hasNext) {

				hasNext = false;

				auto vit2 = m_vertexes.end();
				for (auto eit = vit->edges.begin(); eit != vit->edges.end(); ++eit) {

					if ((*eit)->v1 == vit) {
						if (sqrtf((cx - (*eit)->v2->x) * (cx - (*eit)->v2->x) + ((cy - (*eit)->v2->y) * (cy - (*eit)->v2->y))) < r &&
							std::find(m_pixelsToCells[ind].begin(), m_pixelsToCells[ind].end(), (*eit)->v2) == m_pixelsToCells[ind].end()) {

							vit2 = (*eit)->v2;
							break;
						}
					}
					else {
						if (sqrtf((cx - (*eit)->v1->x) * (cx - (*eit)->v1->x) + ((cy - (*eit)->v1->y) * (cy - (*eit)->v1->y))) < r &&
							std::find(m_pixelsToCells[ind].begin(), m_pixelsToCells[ind].end(), (*eit)->v1) == m_pixelsToCells[ind].end()) {
							vit2 = (*eit)->v1;
							break;
						}
					}
				}

				if (vit2 == m_vertexes.end()) {
					break;
				}

				hasNext = true;
				m_pixelsToCells[ind].push_back(vit2);
				vit = vit2; // for searching next vertex
			}
		}
	}
}