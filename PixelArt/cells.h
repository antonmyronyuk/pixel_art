#ifndef CELLS_H
#define CELLS_H
#include <list>

struct Edge;

struct Vertex
{
	// coordinates
	float x; 
	float y;
	//additional variable (not important)
	//true - if vertex is already visited
	bool f;

	std::list<std::list<Edge>::iterator> edges;

	void addVertex(float x, float y) {
		this->x = x;
		this->y = y;
	}
};

struct Edge
{
	//vertexes on the edge
	std::list<Vertex>::iterator v1;
	std::list<Vertex>::iterator v2;
	//indexes of pixels-neighbours
	// -1 - if there are no neighbour
	int ind1; 
	int ind2;

	void addEdge(int ind1, int ind2, std::list<Vertex>::iterator v1, std::list<Vertex>::iterator v2) {
		this->ind1 = ind1;
		this->ind2 = ind2;
		this->v1 = v1;
		this->v2 = v2;
	}
};







#endif // CELLS_H