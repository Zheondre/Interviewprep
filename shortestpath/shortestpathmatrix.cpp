//start is 0,0,
//end is n-1, n -1 

struct mapRecord { 
	int x; 
	int y;
	int dist;
	
	mapRecord(int x, int y, int dist): 
	x(x), y(y), dist(dist)
	{}
}; 

struct record { 
	int val; 
	bool visited;
} 

using namespace std; // bad but w.e.
int shortest_path(vector<vector<int>>& matrix){ 

	int MAXL = matrix.length();
	int MAXW = matrix[0].length(); 
	int targetX = MAXL -1; 
	int targetX = MAXW -1; 
	
	if(MAXL < 1 ||  MAXW < 1) 
		return -1;
	
	if(!matrix[targetX][targetY]) 
		return -1;
	
	//vector<vector<record>> records;

	unorded_map<maprecord*, bool> visited;
	queue<maprecord*> bfs;

	mapRecord *record = new mapRecord(0, 0, 0);

	bfs.push(record); 
	
	int x = 0;
	int y = 0; 
	
	while(!bfs.empty()){ 

		record = bfs.front();
		bfs.pop();
		
		if(visited[record] != visited.end(){ 
			continue;
		}
		
		visited[record] = true; 
		
		x = record->x; 
		y = record->y;
			
		if(x == targetX && y == targetX) 
			return record->dist;
	
		if(x < MAXW -1) 
			if(matrix[x+1][y])
				bfs.push(new mapRecord(x+1, y, record->dist + 1));
			
		if(y < MAXH-1) 
			if(matrix[x][y+1])
				bfs.push(new mapRecord(x, y+1, record->dist + 1));
		
		if(x > 0) 
			if(matrix[x-1][y])
				bfs.push(new mapRecord(x-1, y, record->dist + 1));
		
		if(y > 0) 
			if(matrix[x][y-1])
				bfs.push(new mapRecord(x, y-1), record->dist + 1);
		/*
		 1 0
		 0 1 
		-1 0 
		 0-1
		*/
	} 	

	return -1;
} 
/* 

   *
*/