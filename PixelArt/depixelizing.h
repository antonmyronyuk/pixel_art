#ifndef DEPIXELIZING_H
#define DEPIXELIZING_H

#include <opencv2\opencv.hpp>
#include <vector>
#include <list>
#include <math.h>
#include <time.h>
#include "cells.h"

#define NEIGHBOUR_TOP 1
#define NEIGHBOUR_TOP_RIGHT 2
#define NEIGHBOUR_RIGHT 4
#define NEIGHBOUR_BOTTOM_RIGHT 8
#define NEIGHBOUR_BOTTOM 16
#define NEIGHBOUR_BOTTOM_LEFT 32
#define NEIGHBOUR_LEFT 64
#define NEIGHBOUR_TOP_LEFT 128


class Depixelizing {

public:

	Depixelizing();
	~Depixelizing();

	void depixelize(const cv::Mat& , float); // main method of the class

private:

	int m_width; // image width
	int m_height; // image height
	cv::Mat m_image; // input image
	float m_scale; // scale factor
	std::vector<unsigned char> m_connections; // similarity graph
											  // (i, j) -> (i * m_w + j)
	std::list<Vertex> m_vertexes;
	std::vector<std::list<std::list<Vertex>::iterator>> m_curvesVertexes;
	std::list<Edge> m_edges;
	std::vector<std::vector<std::list<Vertex>::iterator>> m_pixelsToCells;

	//building
	void buildSimilarityGraph();
	void deleteFullyConnectedBlocks();

	//visualization
	void showSimilarityGraph();
	void showCellGraph();
	void showCellImage(); // show and save in cell_image.png

	//heuristics
	void resolveHeuristics();
	void searchNeighbours(int &x, int &y, const std::vector <std::vector <int>>& a); // for curves heuristic
	void search(int y, int x, int sy, int sx, std::vector <std::vector <int>>& a); // for sparse pixels heuristic
	int curvesHeuristic(int x, int y);
	int sparsePixelsHeuristic(int x, int y);
	int islandsHeuristic(int x, int y);
	unsigned char valence(unsigned char node);

	//building cells
	void buildCells();
	void combineCellsDiagonals();

	//reconstructing cells
	void remove2ValenceNodes();
	void reconstructPixelsToCells();

};

#endif //DEPIXELIZING_H