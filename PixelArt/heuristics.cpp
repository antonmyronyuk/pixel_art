#include "depixelizing.h"

unsigned char Depixelizing::valence(unsigned char node) {
	unsigned char v = 0;
	if (node & NEIGHBOUR_TOP) {
		++v; 
	}
	if (node & NEIGHBOUR_TOP_LEFT) {
		++v;
	}
	if (node & NEIGHBOUR_LEFT) {
		++v;
	}
	if (node & NEIGHBOUR_BOTTOM_LEFT) {
		++v; 
	}
	if (node & NEIGHBOUR_BOTTOM) {
		++v;
	}
	if (node & NEIGHBOUR_BOTTOM_RIGHT) {
		++v;
	}
	if (node & NEIGHBOUR_RIGHT) {
		++v;
	}
	if (node & NEIGHBOUR_TOP_RIGHT) {
		++v;
	}
	return v;
}


void Depixelizing::resolveHeuristics()
{
	for (int y = 0; y < m_height - 1; y++) {
		for (int x = 0; x < m_width - 1; x++) {
			if ((m_connections[y * m_width + x] & NEIGHBOUR_BOTTOM_RIGHT) && (m_connections[y * m_width + (x + 1)] & NEIGHBOUR_BOTTOM_LEFT)) {

				std::vector <int> m(3);
				m[0] = curvesHeuristic(x, y);
				m[1] = sparsePixelsHeuristic(x, y);
				m[2] = islandsHeuristic(x, y);

				if (m[0] == m[1] && m[1] == m[2]) {
					m_connections[y * m_width + (x + 1)] &= ~NEIGHBOUR_BOTTOM_LEFT;
					m_connections[(y + 1) * m_width + x] &= ~NEIGHBOUR_TOP_RIGHT;
					m_connections[y * m_width + x] &= ~NEIGHBOUR_BOTTOM_RIGHT;
					m_connections[(y + 1) * m_width + (x + 1)] &= ~NEIGHBOUR_TOP_LEFT;
				}
				else {
					int res = INT_MIN;
					int number = 0;
					for (int i = 0; i < 3; i++) {
						if (res <= abs(m[i])) { 
							res = abs(m[i]); 
							number = i;
						}
					}

					if (m[number] > 0) {
						m_connections[y * m_width + (x + 1)] &= ~NEIGHBOUR_BOTTOM_LEFT;
						m_connections[(y + 1) * m_width + x] &= ~NEIGHBOUR_TOP_RIGHT;
					}
					else {
						m_connections[y * m_width + x] &= ~NEIGHBOUR_BOTTOM_RIGHT;
						m_connections[(y + 1) * m_width + (x + 1)] &= ~NEIGHBOUR_TOP_LEFT;
					}
				}
				
			}
		}
	}
}

void Depixelizing::searchNeighbours(int &y, int &x, const std::vector <std::vector <int>>& a)
{
	int i, j;
	i = y;
	j = x;
	int new_i = -5; // default value
	int new_j = -5; // default value

	if ((m_connections[i * m_width + j] & NEIGHBOUR_BOTTOM) && (a[i + 1][j]) == 0 && (i + 1) < m_height) {
		//a[i + 1][j] = 1;
		new_i = i + 1;
		new_j = j;
	}

	if ((m_connections[i * m_width + j] & NEIGHBOUR_BOTTOM_RIGHT) && (a[i + 1][j + 1]) == 0 && (i + 1) < m_height && (j + 1) < m_width) {
		//a[i + 1][j + 1] = 1;
		new_i = i + 1;
		new_j = j + 1;
	}

	if ((m_connections[i * m_width + j] & NEIGHBOUR_BOTTOM_LEFT) && (a[i + 1][j - 1]) == 0 && (i + 1) < m_height && (j - 1) >= 0)  {
		//a[i + 1][j - 1] = 1;
		new_i = i + 1;
		new_j = j - 1;
	}

	if ((m_connections[i * m_width + j] & NEIGHBOUR_RIGHT) && (a[i][j + 1]) == 0 && (j + 1) < m_width) {
		//a[i][j + 1] = 1;
		new_i = i;
		new_j = j + 1;
	}

	if ((m_connections[i * m_width + j] & NEIGHBOUR_LEFT) && (a[i][j - 1]) == 0 && (j - 1) >= 0) {
		//a[i][j - 1] = 1;
		new_i = i;
		new_j = j - 1;
	}

	if ((m_connections[i * m_width + j] & NEIGHBOUR_TOP_RIGHT) && (a[i - 1][j + 1]) == 0 && (i - 1) >= 0 && (j + 1) < m_width) {
		//a[i - 1][j + 1] = 1;
		new_i = i - 1;
		new_j = j + 1;
	}

	if ((m_connections[i * m_width + j] & NEIGHBOUR_TOP) && (a[i - 1][j]) == 0 && (i - 1) >= 0) {
		//a[i - 1][j] = 1;
		new_i = i - 1;
		new_j = j;
	}

	if ((m_connections[i * m_width + j] & NEIGHBOUR_TOP_LEFT) && (a[i - 1][j - 1]) == 0 && (i - 1) >= 0 && (j - 1) >= 0) {
		//a[i - 1][j - 1] = 1;
		new_i = i - 1;
		new_j = j - 1;
	}
	x = new_j;
	y = new_i;

}
void Depixelizing::search(int y, int x, int sy, int sx, std::vector <std::vector <int>>& a)
{
	if ((m_connections[y * m_width + x] & NEIGHBOUR_BOTTOM) && (a[y + 1][x]) == 0 && (y + 1) < m_height && abs(x - sx) <= 4 && abs(y - sy) <= 4) {
		a[y + 1][x] = a[y][x];
		search(y + 1, x, sy, sx, a);
	}

	if ((m_connections[y * m_width + x] & NEIGHBOUR_BOTTOM_RIGHT) && (a[y + 1][x + 1]) == 0 && (y + 1) < m_height && (x + 1) < m_width && abs(x - sx) <= 4 && abs(y - sy) <= 4) {
	a[y + 1][x + 1] = a[y][x];
		search(y + 1, x + 1, sy, sx, a);
	}

	if ((m_connections[y * m_width + x] & NEIGHBOUR_BOTTOM_LEFT) && (a[y + 1][x - 1]) == 0 && (y + 1) < m_height && (x - 1) >= 0 && abs(x - sx) <= 4 && abs(y - sy) <= 4) {	
		a[y + 1][x - 1] = a[y][x];
		search(y + 1, x - 1, sy, sx, a);
	}

	if ((m_connections[y * m_width + x] & NEIGHBOUR_RIGHT) && (a[y][x + 1]) == 0 && (x + 1) < m_width && abs(x - sx) <= 4 && abs(y - sy) <= 4) {
		a[y][x + 1] = a[y][x];
		search(y, x + 1, sy, sx, a);
	}

	if ((m_connections[y * m_width + x] & NEIGHBOUR_LEFT) && (a[y][x - 1]) == 0 && (x - 1) >= 0 && abs(x - sx) <= 4 && abs(y - sy) <= 4) {
		a[y][x - 1] = a[y][x];
		search(y, x - 1, sy, sx, a);
	}

	if ((m_connections[y * m_width + x] & NEIGHBOUR_TOP_RIGHT) && (a[y - 1][x + 1]) == 0 && (y - 1) >= 0 && (x + 1) < m_width && abs(x - sx) <= 4 && abs(y - sy) <= 4) {
		a[y - 1][x + 1] = a[y][x];
		search(y - 1, x + 1, sy, sx, a);
	}

	if ((m_connections[y * m_width + x] & NEIGHBOUR_TOP) && (a[y - 1][x]) == 0 && (y - 1) >= 0 && abs(x - sx) <= 4 && abs(y - sy) <= 4) {
		a[y - 1][x] = a[y][x];
		search(y - 1, x, sy, sx, a);
	}

	if ((m_connections[y * m_width + x] & NEIGHBOUR_TOP_LEFT) && (a[y - 1][x - 1]) == 0  && (y - 1) >= 0 && (x - 1) >= 0 && abs(x - sx) <= 4 && abs(y - sy) <= 4) {
		a[y - 1][x - 1] = a[y][x];
		search(y - 1, x - 1, sy, sx, a);
	}
}

int Depixelizing::curvesHeuristic(int x, int y) {

	std::vector <std::vector <int>> a; // it's an image map
	a.resize(m_height);
	for (size_t i = 0; i < a.size(); ++i) {
		a[i].resize(m_width);
		memset(&a[i][0], 0, sizeof(int) * m_width);
	}
	
	a[y][x] = 1; 
	a[y + 1][x + 1] = 1; 
	a[y][x + 1] = 2;
	a[y + 1][x] = 2;
	int count_top_left = 0;
	int count_top_right = 0;
	int count_bottom_left = 0;
	int count_bottom_right = 0;


	//top_left
	int j = x;
	int i = y;
	bool found = true;

	while (found) {
		if (valence(m_connections[i * m_width + j]) == 2) {
			searchNeighbours(i, j, a);
			if (j >= 0 && i >= 0 && j < m_width && i < m_height && a[i][j] == 0) {
				a[i][j] = 1; 
				count_top_left++;
			}
			else found = false; 
		}
		else found = false; 
	}
	//bottom_right
	j = x + 1;
	i = y + 1;
	found = true;
	while (found) {
		if (valence(m_connections[i * m_width + j]) == 2) {
			searchNeighbours(i, j, a);
			if (j >= 0 && i >= 0 && j < m_width && i < m_height && a[i][j] == 0) {
				a[i][j] = 1; 
				count_bottom_right++;
			}
			else found = false; 
		}
		else found = false; 
	}

	//top_right
	j = x + 1;
	i = y;
	found = true;
	while (found) {
		if (valence(m_connections[i * m_width + j]) == 2) {
			searchNeighbours(i, j, a);
			if (j >= 0 && i >= 0 && j < m_width && i < m_height && a[i][j] == 0) {
				a[i][j] = 2; 
				count_top_right++;
			}
			else found = false; 
		}
		else found = false; 
	}


	//bottom_left
	j = x;
	i = y + 1;
	found = true;
	while (found) {
		if (valence(m_connections[i * m_width + j]) == 2) {
			searchNeighbours(i, j, a);
			if (j >= 0 && i >= 0 && j < m_width && i < m_height && a[i][j] == 0) {
				a[i][j]=2; 
				count_bottom_left++;
			}
			else found = false; 
		}
		else found = false; 
	}

	//deleting map
	for (size_t i = 0; i < a.size(); ++i) {
		a[i].clear();
	}
	a.clear();

	return (count_top_left + count_bottom_right - count_top_right - count_bottom_left);
}

int Depixelizing::sparsePixelsHeuristic(int x, int y) {
	
	std::vector <std::vector <int>> a; // it's an image map
	a.resize(m_height);
	for (size_t i = 0; i < a.size(); ++i) {
		a[i].resize(m_width);
		memset(&a[i][0], 0, sizeof(int) * m_width);
	}
	
	int sx = x;
	int sy = y;
	a[y][x] = 1; 
	a[y + 1][x + 1] = 1; 
	a[y][x + 1] = 2;
	a[y + 1][x] = 2;
	search(y, x, sy, sx, a);
	search(y + 1, x, sy + 1, sx, a);
	search(y, x + 1, sy, sx + 1, a);
	search(y + 1, x + 1, sy + 1, sx + 1, a);
	int weight1 = 0;
	int weight2 = 0;
	for (int i = std::max(0, sy - 4); i <= std::min(m_height - 1, sy + 4); ++i) {
		for (int j = std::max(0, sx - 4); j <= std::min(m_width - 1, sx + 4); ++j) {
			if (a[i][j] == 1) { 
				++weight1;
			}
			if (a[i][j] == 2) {
				++weight2;
			}
		}

	}

	//deleting image map
	for (size_t i = 0; i < a.size(); ++i) {
		a[i].clear();
	}
	a.clear();

	return (weight2 - weight1);
}

int Depixelizing::islandsHeuristic(int x, int y) {
	int weight1 = 0;
	int weight2 = 0;

	if (valence(m_connections[y * m_width + x]) == 1 || valence(m_connections[(y + 1) * m_width + (x + 1)]) == 1) {
		weight1 = 5;
	}
	if (valence(m_connections[(y + 1) * m_width + x]) == 1 || valence(m_connections[y * m_width + (x + 1)]) == 1) {
		weight2 = 5;
	}
	return (weight1 - weight2);
}