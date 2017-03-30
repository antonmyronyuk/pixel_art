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
			(it2->y * m_width + it2->x) != 0 &&
			(it2->y * m_width + it2->x) != m_width &&
			(it2->y * m_width + it2->x) != m_height * m_width &&
			(it2->y * m_width + it2->x) != m_height * m_width + m_width) {

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