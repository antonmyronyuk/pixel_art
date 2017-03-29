#include "depixelizing.h"

const float pointsShifts[8][2] = {{0.f, 0.f}, {0.f, -0.5f}, {0.f, -1.f}, {0.5f, -1.f}, {1.f, -1.f}, {1.f, -0.5f}, {1.f, 0.f}, {0.5f, 0.f}};
//[index of vertex][k], k = 0 for x coordinate, k = 1 for y coordinate
const int n = 8; //number of vertexes in pixel

void Depixelizing::buildCells() {

	std::list<Vertex> vertexes;
	std::list<Edge> edges;
	std::vector<std::vector<std::list<Vertex>::iterator>> pixelsToCells(m_width * m_height, std::vector<std::list<Vertex>::iterator>(n));

	Vertex v;
	Edge e;

	std::list<Vertex>::iterator vIt;
	std::list<Edge>::iterator eIt;

	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {

			int pixInd = y * m_width + x;
			// Calc coord of 0 point in cell 
			float cx = (float)x; // vertex x
			float cy = (float)y + 1.f; // vertex y

			if (x == 0 && y == 0) {
				
				//Add vertexes 0 - 7
				for (int i = 0; i < n; ++i) {
					v.addVertex(cx + pointsShifts[i][0], cy + pointsShifts[i][1]);
					vertexes.push_back(v);
					vIt = --vertexes.end();
					pixelsToCells[pixInd][i] = vIt;
				}
				//Add edges

				//edge between vertexes 0 and 1
				e.addEdge(-1, pixInd, pixelsToCells[pixInd][0], pixelsToCells[pixInd][1]);
				edges.push_back(e);
				eIt = --edges.end();
				pixelsToCells[pixInd][0]->edges.push_back(eIt);
				pixelsToCells[pixInd][1]->edges.push_back(eIt);

				//edge between vertexes 1 and 2
				e.addEdge(-1, pixInd, pixelsToCells[pixInd][1], pixelsToCells[pixInd][2]);
				edges.push_back(e);
				eIt = --edges.end();
				pixelsToCells[pixInd][1]->edges.push_back(eIt);
				pixelsToCells[pixInd][2]->edges.push_back(eIt);

				//edge between vertexes 2 and 3
				e.addEdge(-1, pixInd, pixelsToCells[pixInd][2], pixelsToCells[pixInd][3]);
				edges.push_back(e);
				eIt = --edges.end();
				pixelsToCells[pixInd][2]->edges.push_back(eIt);
				pixelsToCells[pixInd][3]->edges.push_back(eIt);

				//edge between vertexes 3 and 4
				e.addEdge(-1, pixInd, pixelsToCells[pixInd][3], pixelsToCells[pixInd][4]);
				edges.push_back(e);
				eIt = --edges.end();
				pixelsToCells[pixInd][3]->edges.push_back(eIt);
				pixelsToCells[pixInd][4]->edges.push_back(eIt);

				//edge between vertexes 4 and 5
				e.addEdge(1, pixInd, pixelsToCells[pixInd][4], pixelsToCells[pixInd][5]);
				edges.push_back(e);
				eIt = --edges.end();
				pixelsToCells[pixInd][4]->edges.push_back(eIt);
				pixelsToCells[pixInd][5]->edges.push_back(eIt);

				//edge between vertexes 5 and 6
				e.addEdge(1, pixInd, pixelsToCells[pixInd][5], pixelsToCells[pixInd][6]);
				edges.push_back(e);
				eIt = --edges.end();
				pixelsToCells[pixInd][5]->edges.push_back(eIt);
				pixelsToCells[pixInd][6]->edges.push_back(eIt);

				//edge between vertexes 6 and 7
				e.addEdge(m_width, pixInd, pixelsToCells[pixInd][6], pixelsToCells[pixInd][7]);
				edges.push_back(e);
				eIt = --edges.end();
				pixelsToCells[pixInd][6]->edges.push_back(eIt);
				pixelsToCells[pixInd][7]->edges.push_back(eIt);

				//edge between vertexes 7 and 0
				e.addEdge(m_width, pixInd, pixelsToCells[pixInd][7], pixelsToCells[pixInd][0]);
				edges.push_back(e);
				eIt = --edges.end();
				pixelsToCells[pixInd][7]->edges.push_back(eIt);
				pixelsToCells[pixInd][0]->edges.push_back(eIt);
			}
			else {
				if (x == 0) {
					int pixIndUp = pixInd - m_width;
					//Add vertex 0
					v.addVertex(cx + pointsShifts[0][0], cy + pointsShifts[0][1]);
					vertexes.push_back(v);
					vIt = --vertexes.end();
					pixelsToCells[pixInd][0] = vIt;

					//Add vertex 1
					v.addVertex(cx + pointsShifts[1][0], cy + pointsShifts[1][1]);
					vertexes.push_back(v);
					vIt = --vertexes.end();
					pixelsToCells[pixInd][1] = vIt;

					//Read vertex 2
					pixelsToCells[pixInd][2] = pixelsToCells[pixIndUp][0];

					//edge between vertexes 0 and 1
					e.addEdge(-1, pixInd, pixelsToCells[pixInd][0], pixelsToCells[pixInd][1]);
					edges.push_back(e);
					eIt = --edges.end();
					pixelsToCells[pixInd][0]->edges.push_back(eIt);
					pixelsToCells[pixInd][1]->edges.push_back(eIt);

					//edge between vertexes 1 and 2
					e.addEdge(-1, pixInd, pixelsToCells[pixInd][1], pixelsToCells[pixInd][2]);
					edges.push_back(e);
					eIt = --edges.end();
					pixelsToCells[pixInd][1]->edges.push_back(eIt);
					pixelsToCells[pixInd][2]->edges.push_back(eIt);
				}
				else {
					//Read vertex 0, 1, 2
					pixelsToCells[pixInd][0] = pixelsToCells[pixInd - 1][6];
					pixelsToCells[pixInd][1] = pixelsToCells[pixInd - 1][5];
					pixelsToCells[pixInd][2] = pixelsToCells[pixInd - 1][4];
				}

				if (y == 0) {
					//Add vertex 3
					v.addVertex(cx + pointsShifts[3][0], cy + pointsShifts[3][1]);
					vertexes.push_back(v);
					vIt = --vertexes.end();
					pixelsToCells[pixInd][3] = vIt;

					//Add vertex 4
					v.addVertex(cx + pointsShifts[4][0], cy + pointsShifts[4][1]);
					vertexes.push_back(v);
					vIt = --vertexes.end();
					pixelsToCells[pixInd][4] = vIt;
	
					//edge between vertexes 2 and 3
					e.addEdge(-1, pixInd, pixelsToCells[pixInd][2], pixelsToCells[pixInd][3]);
					edges.push_back(e);
					eIt = --edges.end();
					pixelsToCells[pixInd][2]->edges.push_back(eIt);
					pixelsToCells[pixInd][3]->edges.push_back(eIt);

					//edge between vertexes 3 and 4
					e.addEdge(-1, pixInd, pixelsToCells[pixInd][3], pixelsToCells[pixInd][4]);
					edges.push_back(e);
					eIt = --edges.end();
					pixelsToCells[pixInd][3]->edges.push_back(eIt);
					pixelsToCells[pixInd][4]->edges.push_back(eIt);
				}
				else {
					//Read vertex 3, 4
					pixelsToCells[pixInd][3] = pixelsToCells[pixInd - m_width][7];
					pixelsToCells[pixInd][4] = pixelsToCells[pixInd - m_width][6];
				}

				//Add vertedx 5, 6, 7
				for (int i = 5; i < 8; ++i) {
					v.addVertex(cx + pointsShifts[i][0], cy + pointsShifts[i][1]);
					vertexes.push_back(v);
					vIt = --vertexes.end();
					pixelsToCells[pixInd][i] = vIt;
				}

				//edge between vertexes 4 and 5
				e.addEdge((x < m_width - 1) ? (pixInd + 1) : -1, pixInd, pixelsToCells[pixInd][4], pixelsToCells[pixInd][5]);
				edges.push_back(e);
				eIt = --edges.end();
				pixelsToCells[pixInd][4]->edges.push_back(eIt);
				pixelsToCells[pixInd][5]->edges.push_back(eIt);

				//edge between vertexes 5 and 6
				e.addEdge((x < m_width - 1) ? (pixInd + 1) : -1, pixInd, pixelsToCells[pixInd][5], pixelsToCells[pixInd][6]);
				edges.push_back(e);
				eIt = --edges.end();
				pixelsToCells[pixInd][5]->edges.push_back(eIt);
				pixelsToCells[pixInd][6]->edges.push_back(eIt);

				//edge between vertexes 6 and 7
				e.addEdge((y < m_height - 1) ? (pixInd + m_width) : -1, pixInd, pixelsToCells[pixInd][6], pixelsToCells[pixInd][7]);
				edges.push_back(e);
				eIt = --edges.end();
				pixelsToCells[pixInd][6]->edges.push_back(eIt);
				pixelsToCells[pixInd][7]->edges.push_back(eIt);

				//edge between vertexes 7 and 0
				e.addEdge((y < m_height - 1) ? (pixInd + m_width) : -1, pixInd, pixelsToCells[pixInd][7], pixelsToCells[pixInd][0]);
				edges.push_back(e);
				eIt = --edges.end();
				pixelsToCells[pixInd][7]->edges.push_back(eIt);
				pixelsToCells[pixInd][0]->edges.push_back(eIt);



			}
		}
	}

	m_vertexes.swap(vertexes);
	m_edges.swap(edges);
	m_pixelsToCells.swap(pixelsToCells);
}
