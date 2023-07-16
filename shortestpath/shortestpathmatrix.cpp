/* 
Problem: Given a matrix of size m x n filled with 0s and 1s, 
where 0 represents an obstacle and 1 represents a path,
find the shortest path from the top-left corner to the bottom-right corner,
moving only in right or downward direction. If no path exists, return -1.
*/


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <deque> 
#include <unordered_map>
#include <algorithm>
#include <cassert>

struct Record { 
	int x; 
	int y;
	Record(): x(0), y(0)
	{}
	Record(int x, int y): 
	x(x), y(y)
	{}
}; 

struct his { 

	bool visited; 
	int dist; 

	his() { 
		visited = false;
		dist = 0; 
	}

	his(int val): dist(val){}
};

using namespace std; // bad but w.e.

/*
	1 0
	0 1 
   -1 0 
	0-1
*/

int shortest_path(vector<vector<int>>& matrix){ 

	int MAXR = matrix.size();
	int MAXC = matrix[0].size(); 

	vector<vector<his>> distmap(MAXR, vector<his>(MAXC));

	int targetx = MAXR-1; 
	int targety = MAXC-1; 
	
	if(MAXR < 2 || MAXC < 2) 
		return -1;
	
	if(!matrix[targetx][targety]) 
		return -1;
	
	//vector<vector<record>> records;

	queue<Record*> bfs;
	queue<his> test; 

	// test.push(his(1));
	// test.push(his(2));
	// cout << " size " << test.size() << endl; 

	// test.pop(); 
	// cout << " size " << test.size() << endl; 

	Record *record;

	bfs.push(new Record(0,0)); 
	
	int x = 0;
	int y = 0; 

	while(!bfs.empty()){ 

		record = bfs.front();
		bfs.pop();

		//cout << record->x << " " << record->y << " " << record <<  endl;

		x = record->x; 
		y = record->y; 
		
		if(x < 0 || x >= MAXR || y < 0 || y >= MAXC)
			continue;

		if(distmap[x][y].visited){
			continue;
		}

		distmap[x][y].visited = true; 

		//cout << x << " " << y << " " << record <<  endl;
			
		if(x == targetx && y == targety) 
			return distmap[x][y].dist;
	
		if(x < MAXR -1) 
			if(matrix[x+1][y])
				if(!distmap[x+1][y].visited){
 					bfs.push(new Record(x+1,y));
					distmap[x+1][y].dist = distmap[x][y].dist +1;
				}
		
		if(x > 0) 
			if(matrix[x-1][y])
				if(!distmap[x-1][y].visited){
					bfs.push(new Record(x-1,y));
					distmap[x-1][y].dist = distmap[x][y].dist +1;
				}

		if(y < MAXC-1) 
			if(matrix[x][y+1])
				if(!distmap[x][y+1].visited){
					bfs.push(new Record(x,y+1));
					distmap[x][y+1].dist = distmap[x][y].dist +1;
				}

		if(y > 0) 
			if(matrix[x][y-1])
				if(!distmap[x][y-1].visited){
					bfs.push(new Record(x,y-1));
					distmap[x][y-1].dist = distmap[x][y].dist +1;
				}

		delete(record); 
		record = NULL;
	} 	

	return -1;
} 

/* 
0,0 0,1 0,2
1,0 1,1 1,2
2,0 2,1 2,2
*/

int main(int argc, char *argv[]){ 

	vector<vector<int>> matrix = { 
		{1, 0, 1},
		{1, 0, 1},
		{1, 0, 1}
    };

	vector<vector<int>> matrix2 = { 
		{1, 0, 1},
		{0, 1, 1},
		{1, 1, 1}
    };

	vector<vector<int>> matrix3 = { 
		{1, 0},
		{0, 1},
    };

	vector<vector<int>> matrix4 = { 
		{1, 1, 1},
		{1, 0, 1},
		{1, 1, 1}
    };

	vector<vector<int>> matrix5 = { 
		{1, 1, 1, 1},
		{1, 0, 1, 1},
		{1, 1, 0, 0},
		{0, 1, 1, 1},
		{0, 1, 1, 1}
    };

	vector<vector<int>> matrix6 = { 
		{1, 0, 1, 1, 1},
		{1, 0, 1, 0, 1},
		{1, 0, 1, 0, 1},
		{1, 0, 1, 0, 1},
		{1, 1, 1, 0, 1}
    };

	vector<vector<int>> matrix7 = { 
		{1, 1, 1, 1, 0, 1, 1},
        {1, 0, 0, 1, 1, 1, 0},
        {1, 1, 1, 1, 0, 1, 1},
        {1, 0, 1, 0, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 1}
    };

	assert(-1 == shortest_path(matrix));
	assert(-1 == shortest_path(matrix2));
	assert(-1 == shortest_path(matrix3)); 

	assert( 4 == shortest_path(matrix4)); 
	assert( 7 == shortest_path(matrix5));
	assert(16 == shortest_path(matrix6));
	assert(10 == shortest_path(matrix7));

	cout << "Tests completed" << endl;

	return 0;
}