#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <queue>
#include <algorithm>
#include <stack>
using namespace std;

class cell
{
public:
	cell(int a = 0, int b = 0){ x = a; y = b; is_in = 0; is_out = 0; is_tar = 0; is_obs = 0; is_visit = 0;};
	~cell(){};
	int x, y, d;
	bool is_in, is_out, is_tar, is_obs, is_visit;
	cell* parent;
	vector<cell*> neighbor;
};

void initializeSingleSource(vector<vector<cell*> > , cell* );
void relax(cell* , cell* , int );
int weight(cell* , cell*);
void minheapify(vector<cell*>&, int);
cell* extract_min(vector<cell*>& A);
void decrease_key(vector<cell*>& A, cell* v, int key);
void print_path(cell* s, cell* v);

int main (int argc, char* argv[])
{
	/*----------For Parsing-----------*/
	string line;
	stringstream ss;
	int lineCount = 1, lineCellNum;
	fstream fin(argv[1]);
	if (!fin.is_open())
	{
		cout << "No Imput Files.\n";
		return 1;
	} 

	/*----------Map Information-----------*/
	int m, n, in_x, in_y, out_x, out_y;
	vector<vector<cell*> > c;

	std::vector<cell*> Q;
	std::vector<cell*> vcInOut;
	std::vector<cell*> vtar, vobs;

	while(getline(fin, line))
	{
		istringstream token(line);
		if (lineCount == 1) // Deal With M. N. Cin.x Cout.y
		{
			token >> m;
			token >> n;
			for (int i = 1; i < m+1; ++i)
			{
				vector<cell*> vcCol;
				for (int j = 1; j < n+1; ++j)
				{
					cell *tmpc = new cell(i, j);
					vcCol.push_back(tmpc);
					Q.push_back(tmpc);

					/*----------Adding All Cells' neighbor-----------*/
					if(i - 1 != 0)
					{
						tmpc->neighbor.push_back(c[i - 2][j - 1]);
						c[i - 2][j - 1]->neighbor.push_back(tmpc);
					}
					if(j - 1 != 0)
					{
						std::vector<cell*>::reverse_iterator rit = vcCol.rbegin();
						tmpc->neighbor.push_back(*(++rit));
						(*rit)->neighbor.push_back(tmpc);
					}
					/*----------Adding All Cells' neighbor-----------*/
				}
				c.push_back(vcCol);
			}
			/*----------Cinput-----------*/
			token >> in_x;
			token >> in_y;
			c[in_x - 1][in_y - 1]->is_in = true;
			vcInOut.push_back(c[in_x - 1][in_y - 1]);
			/*----------Coutput-----------*/
			token >> out_x;
			token >> out_y;
			c[out_x - 1][out_y - 1]->is_out = true;
			vcInOut.push_back(c[out_x - 1][out_y - 1]);
		}
		else // Deal With Targets and Obstacles
		{
			token >> lineCellNum;

			for (int i = 0; i < lineCellNum; ++i)
			{
				int tmpx, tmpy;
				token >> tmpx;
				token >> tmpy;
				if(lineCount == 2) c[tmpx - 1][tmpy - 1]->is_tar = true;
				else c[tmpx - 1][tmpy - 1]->is_obs = true;

				/*------------Not Sure------------*/
				if(lineCount == 2) vtar.push_back(c[tmpx - 1][tmpy - 1]);
				else vobs.push_back(c[tmpx - 1][tmpy - 1]);
			}
		}
		lineCount++;
	}

	vector<cell*>::iterator it;
	initializeSingleSource(c, c[in_x - 1][in_y - 1]);
	cell* u;

	/*------------Build Min Heap------------*/
	for(int i = Q.size()/2; i >= 0; --i) minheapify(Q, i+1);

	/*------------Dijkstra Algorithm------------*/
	while(!Q.empty())
	{
		u = extract_min(Q);
		if(u == c[out_x - 1][out_y - 1]) break;
		u->is_visit = true;
		for(it = u->neighbor.begin(); it != u->neighbor.end(); ++it)
		{
			if((*it)->is_obs == true || (*it)->is_visit == true) continue;
			relax(u, (*it), weight(u, (*it)));
		}
	}
	print_path(c[in_x - 1][in_y - 1], c[out_x - 1][out_y - 1]);
	cout << endl;
		
	
	/*cout << extract_min(Q)->d << " (" << Q.front()->x << "," << Q.front()->y << ")" << endl;
	cout << "size : " << Q.size() << endl;
	decrease_key(Q, c[out_x - 1][out_y - 1], 10);
	cout << extract_min(Q)->d << " (" << Q.front()->x << "," << Q.front()->y << ")" << endl;
	cout << "size : " << Q.size() << endl;*/		/*-----------Testing Min_Queue-----------------*/

	/*for(int i = 0; i < c.size(); ++i)
	{
		for(int j = 0 ; j < c[i].size(); ++j)
		{
			cout << "C (" << c[i][j]->x << "," << c[i][j]->y << ") : ";
			cout << c[i][j]->neighbor.size() << endl;
		}
	}*/		/*--------------All Cells' Coordinate and Their #(neighbor)--------------------*/

	/*for(std::vector<cell*>::iterator it = vobs.begin(); it != vobs.end(); ++it)
	{		
		cout << "C (" << (*it)->x << "," << (*it)->y << ")\n"; 
		cout << "Target  ? " << (*it)->is_tar << endl;
		cout << "Obstacle? " << (*it)->is_obs << endl;
	}		
	for(std::vector<cell*>::iterator it = vtar.begin(); it != vtar.end(); ++it)
	{		
		cout << "C (" << (*it)->x << "," << (*it)->y << ")\n"; 
		cout << "Target  ? " << (*it)->is_tar << endl;
		cout << "Obstacle? " << (*it)->is_obs << endl;
	}*/		/*--------------All Obs&Tar Cells' Coordinate and Their Information--------------------*/
	
	/*for(std::vector<cell*>::iterator it = vcInOut.begin(); it != vcInOut.end(); ++it)
	{
		cout << "C   (" << (*it)->x << "," << (*it)->y << ")\n"; 
		cout << "Input ? " << (*it)->is_in << endl;
		cout << "Output? " << (*it)->is_out << endl;
	}*/		/*--------------Input and Output Cells' Coordinate--------------------*/
}

void initializeSingleSource(vector<vector<cell*> > G, cell* s)
{
	for(int i = 0; i < G.size(); ++i)
	{
		for(int j = 0; j < G[i].size(); ++j)
		{
			G[i][j]->d = 1e9;
			G[i][j]->parent = NULL;
		}
	}
	s->d = 0;
}
// cin.ignore(cin.rdbuf()->in_avail()+1);

void relax(cell* u, cell* v, int w)
{
	if(v->d > u->d + w)
	{
		v->d = u->d + w;
		v->parent = u;
		// if(v->is_tar == true)
	}
}

int weight(cell* u, cell* v)
{
	return (u->is_tar == true || v->is_tar == true)? 1 : 50;
}

void minheapify(vector<cell*>& A, int i)
{
	int l = i*2, r = i*2+1;
	int smallest;
	if(l <= A.size() && A[l - 1]->d < A[i - 1]->d)
		smallest = l;
	else
		smallest = i;
	if(r <= A.size() && A[r - 1]->d < A[smallest - 1]->d)
		smallest = r;
	if(smallest != i)
	{
		cell* tmpc = new cell();
		tmpc = A[i - 1];
		A[i - 1] = A[smallest - 1];
		A[smallest - 1] = tmpc;
		minheapify(A, smallest);
	}
}

cell* extract_min(vector<cell*>& A)
{
	if(A.size() < 1)
	{
		cout << "Error : Heap underflow.\n";
		return 0;
	}
	cell* min = A[0];
	A[0] = A[A.size() - 1];
	A.pop_back();
	minheapify(A, 1);
	return min;
}

void decrease_key(vector<cell*>& A, cell* v, int key)
{
	if(key > v->d)
	{
		cout << "Error : New key < current key.\n";
		return void();
	}
	v->d = key;
	for(int i = A.size()/2; i >= 0; --i) minheapify(A, i+1);
}

void print_path(cell* s, cell* v)
{
	if(v == s)
		cout << "(" << s->x << "," << s->y << ")" << " ";
	else if(v->parent == NULL)
	{
		cout << "No path from (" << s->x << "," << s->y << ") to ";
		cout << "(" << v->x << "," << v->y << ") exists.\n";
	}
	else
	{
		print_path(s, v->parent);
		cout << "(" << v->x << "," << v->y << ")" << " ";
	}
}