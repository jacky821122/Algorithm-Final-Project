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
	cell(int a = 0, int b = 0){x = a; y = b; is_in = 0; is_out = 0; is_tar = 0; is_obs = 0; is_visit = 0; color = 'w';};
	~cell(){};
	int x, y, d;
	char color;
	bool is_in, is_out, is_tar, is_obs, is_visit;
	cell* dfsParent;
	vector<cell*> parent;
	vector<cell*> neighbor;
};

int m, n, in_x, in_y, out_x, out_y;
int weight(cell* , cell*);
cell* extract_min(vector<cell*>& );
void initializeSingleSource(vector<vector<cell*> > , cell*, bool);
void relax(cell* , cell* , int , vector<cell*>&);
void minheapify(vector<cell*>&, int);
void decrease_key(vector<cell*>& , cell* , int);
void store_path(cell* , cell* , vector<vector<cell*> >&, vector<cell*>&, vector<cell*>&);
void dfs_visit(cell* );
void dfs_iterative(cell* );
void dijkstra(vector<vector<cell*> > c, cell* source, cell*, vector<cell*> Q, bool);
void path_select(cell* );

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

	vector<vector<cell*> > vPath;
	vector<cell*>::iterator it;
	vector<cell*> Path;

	dijkstra(c, c[in_x - 1][in_y - 1], c[out_x - 1][out_y - 1], Q, 1);
	// store_path(c[in_x - 1][in_y - 1], c[out_x - 1][out_y - 1], vPath, Path, vtar);
	dfs_iterative(c[out_x - 1][out_y - 1]);

	// for(int i = 0; i < vPath.size(); ++i)
	// {
	// 	for(int j = 0; j < vPath[i].size(); ++j)
	// 	{
	// 		cout << "(" << vPath[i][j]->x << "," << vPath[i][j]->y << ")";
	// 	}
	// 	cout << endl;
	// }


	// store_path(c[in_x - 1][in_y - 1], c[out_x - 1][out_y - 1], vPath);
	// store_path(c[in_x - 1][in_y - 1], tmp, vPath);

	// dfs_iterative(c[out_x - 1][out_y - 1]);

	
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

void path_select(cell* target)
{
	vector<stack<cell*> > vPath;
	cout << target->parent.size() << endl;
	for(vector<cell*>::iterator it = target->parent.begin(); it != target->parent.end(); ++it)
	{
		cout << "(" << (*it)->x << "," << (*it)->y << ")" << (*it)->parent.size() << endl;
		stack<cell*> S;
		cell* tmpc;
		tmpc = (*it);
		cout << "(" << tmpc->x << "," << tmpc->y << ")";
		/*while(!tmpc->parent.empty())
		{
			S.push(tmpc);
			tmpc = tmpc->parent.back();
			cout << "(" << tmpc->x << "," << tmpc->y << ")";
		}
		cout << endl;*/
		while(!tmpc->parent.empty())
		{
			S.push(tmpc);
			tmpc = tmpc->parent.back();
		}
	}
}

void initializeSingleSource(vector<vector<cell*> > G, cell* s, bool reset)
{
	for(int i = 0; i < G.size(); ++i)
	{
		for(int j = 0; j < G[i].size(); ++j)
		{
			G[i][j]->d = 1e9;
			G[i][j]->parent.clear();
			if(reset == 1)
			G[i][j]->is_visit = false;
		}
	}
	s->d = 0;
}

void relax(cell* u, cell* v, int w, vector<cell*>& Q)
{
	if(v->d >= u->d + w)
	{
		v->d = u->d + w;
		// v->parent.clear();
		v->parent.push_back(u);
		decrease_key(Q, v, v->d);
	}
	else if(v->d == u->d + w && v->is_out == true) v->parent.push_back(u);
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

void store_path(cell* s, cell* v, vector<vector<cell*> >& vPath, vector<cell*>& Path, vector<cell*>& vtar)
{
	if(v == s)
	{
		// cout << "(" << s->x << "," << s->y << ")" << " ";
		/*if(v->is_tar == true)
		{
			for(vector<cell*>::iterator it2 = vtar.begin(); it2 != vtar.end(); ++it2)
			{
				if((*it2) == v)
				{
					vtar.erase(it2);
					break;
				}
			}
			return void();
		}*/
		Path.push_back(v);
	}
	else if(v->parent.empty() && s->is_in == true && v->is_out == true)
	{
		cout << "No path from (" << s->x << "," << s->y << ") to ";
		cout << "(" << v->x << "," << v->y << ") exists.\n";
		Path.clear();
		return void();
	}
	else if(v->parent.empty())
	{
		Path.clear();
		return void();
	}
	else
	{
		for(vector<cell*>::iterator it = v->parent.begin(); it != v->parent.end(); ++it)
		{
			// cout << "(" << (*it)->x << "," << (*it)->y << ")" << " ";
			store_path(s, (*it), vPath, Path, vtar);
			// cout << "(" << v->x << "," << v->y << ")" << " ";
			
			// if(v->is_tar == true)
			// {
				// for(vector<cell*>::iterator it2 = vtar.begin(); it2 != vtar.end(); ++it2)
				// {
					// cout << (*it2)->x << " " << (*it2)->y << endl;
					// if((*it2) == v)
					// {
						// vtar.erase(it2);
						// break;
					// }
				// }
			// }
			Path.push_back(v);
			if(v->is_out == true)
			{
				vPath.push_back(Path);
				Path.clear();
				return void();
				// cout << endl;
			}
		}
	}
}

void dfs_visit(cell* u)
{
	u->color = 'g';
	for(vector<cell*>::iterator it = u->parent.begin(); it != u->parent.end(); ++it)
	{
		if((*it)->color == 'w')
		{
			(*it)->dfsParent = u;
			dfs_visit((*it));
		}
	}
	u->color = 'b';
	cout << u->x << "," << u->y << endl;
}

void dfs_iterative(cell* v)
{
	stack<cell*> S;
	cell* u;
	S.push(v);
	v->dfsParent = NULL;
	while(!S.empty())
	{
		v = S.top();
		S.pop();
		if(!v->parent.empty())
		{
			for(vector<cell*>::iterator it = v->parent.begin(); it != v->parent.end(); ++it)
			{
				S.push((*it));
				(*it)->dfsParent = v;
			}
		}
		else
		{
			cout << "(" << v->x << "," << v->y << ")";
			stack<cell*> S;
			cell* tmpc;
			tmpc = v;
			while(tmpc->dfsParent != NULL)
			{
				S.push(tmpc);
				tmpc = tmpc->dfsParent;
				cout << "(" << tmpc->x << "," << tmpc->y << ")";
			}
			cout << endl;
		}
	}
}

void dijkstra(vector<vector<cell*> > c, cell* source, cell* target, vector<cell*> Q, bool reset)
{
	initializeSingleSource(c, source, reset);

	vector<cell*>::iterator it;
	cell* u;
	/*------------Build Min Heap------------*/
	for(int i = Q.size()/2; i >= 0; --i) minheapify(Q, i+1);
	/*cout << extract_min(Q)->d << " (" << Q.front()->x << "," << Q.front()->y << ")" << endl;
	cout << "size : " << Q.size() << endl;
	decrease_key(Q, c[out_x - 1][out_y - 1], 10);
	cout << extract_min(Q)->d << " (" << Q.front()->x << "," << Q.front()->y << ")" << endl;
	cout << "size : " << Q.size() << endl;*/

	while(!Q.empty())
	{
		u = extract_min(Q);

		u->is_visit = true;
		if(u == target) 
			{
				// return void();
			}
		for(it = u->neighbor.begin(); it != u->neighbor.end(); ++it)
		{
			if((*it)->is_obs == true || (*it)->is_visit == true) continue;
			relax(u, (*it), weight(u, (*it)), Q);
		}
	}
}