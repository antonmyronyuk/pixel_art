#include "depixelizing.h"

bool Depixelizing::connected(int ind1, int ind2) {

	if (ind1 < 0 || ind2 < 0) {
		return false;
	}

	if (abs(ind1 - ind2) == 1) {
		if (ind1 < ind2 && (m_connections[ind1] & NEIGHBOUR_RIGHT)) {
			return true;
		}

		if (ind2 < ind1 && (m_connections[ind2] & NEIGHBOUR_RIGHT)) {
			return true;
		}

	} else if (abs(ind1 - ind2) == m_width) {
		if (ind1 < ind2 && (m_connections[ind1] & NEIGHBOUR_BOTTOM)) {
			return true;
		}

		if (ind2 < ind1 && (m_connections[ind2] & NEIGHBOUR_BOTTOM)) {
			return true;
		}

	} else {
		if (ind1 < ind2) {
			if (ind1 + m_width < ind2 && (m_connections[ind1] & NEIGHBOUR_BOTTOM_RIGHT)) {
				return true;
			}

			if (ind1 + m_width > ind2 && (m_connections[ind1] & NEIGHBOUR_BOTTOM_LEFT)) {
				return true;
			}

		} else {
			if (ind2 + m_width < ind1 && (m_connections[ind2] & NEIGHBOUR_BOTTOM_RIGHT)) {
				return true;
			}

			if (ind2 + m_width > ind1 && (m_connections[ind2] & NEIGHBOUR_BOTTOM_LEFT)) {
				return true;
			}
		}
	}

	return false;
}

void Depixelizing::combineCells() {

	auto it = m_edges.begin();
	while (it != m_edges.end()) {
		auto currIt = it;
		++it;
		if (connected(currIt->ind1, currIt->ind2)) {
			currIt->v1->edges.remove(currIt);
			currIt->v2->edges.remove(currIt);
			m_edges.erase(currIt);
		}
		
	}
}