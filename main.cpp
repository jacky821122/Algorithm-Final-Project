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
	cell(int a = 0, int b = 0){x = a; y = b; is_in = 0; is_out = 0; is_tar = 0; is_obs = 0; is_visit = 0; color = 'w'; is_path = 0;};
	~cell(){};
	int x, y, d;
	char color;
	bool is_in, is_out, is_tar, is_obs, is_visit, is_path;
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
bool dijkstra(vector<vector<cell*> > c, cell* source, cell*, vector<cell*> Q, bool);
void insertion(vector<cell*>&, int, int);
vector<cell*> dfs_iterative(cell* );
bool dd(vector<vector<cell*> >, cell*, cell*, cell*, vector<cell*>, vector<cell*>&, vector<cell*>&);

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
	vector<vector<cell*> > c, ctest;

	std::vector<cell*> Q;
	std::vector<cell*> vcInOut;
	std::vector<cell*> vtar, vobs;

	fin >> m;
	fin >> n;
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
	fin >> in_x;
	fin >> in_y;
	c[in_x - 1][in_y - 1]->is_in = true;
	vcInOut.push_back(c[in_x - 1][in_y - 1]);
	/*----------Coutput-----------*/
	fin >> out_x;
	fin >> out_y;
	c[out_x - 1][out_y - 1]->is_out = true;
	vcInOut.push_back(c[out_x - 1][out_y - 1]);
	
	fin >> lineCellNum;
	for (int i = 0; i < lineCellNum; ++i)
	{
		int tmpx, tmpy;
		fin >> tmpx;
		fin >> tmpy;
		if((tmpx == in_x && tmpy == in_y) || (tmpx == out_x && tmpy == out_y)) continue;
		c[tmpx - 1][tmpy - 1]->is_tar = true;

		/*------------Not Sure------------*/
		vtar.push_back(c[tmpx - 1][tmpy - 1]);			
	}

	fin >> lineCellNum;
	for (int i = 0; i < lineCellNum; ++i)
	{
		int tmpx, tmpy;
		fin >> tmpx;
		fin >> tmpy;
		
		c[tmpx - 1][tmpy - 1]->is_obs = true;

		/*------------Not Sure------------*/
		vobs.push_back(c[tmpx - 1][tmpy - 1]);		
	}
	
	int tar_num = vtar.size(), i = 0;
	vector<vector<cell*> > vPath;
	vector<cell*> Path, Path2;
	vector<cell*>::iterator it, it1;
	vector<cell*> vtargetPassThrough;

	while(tar_num != 0)
	{
		for(it = vtar.begin(); it != vtar.end(); ++it)
		{
			if((*it)->is_tar)
			{
				bool delta = false;
				cell* tmpTarget = *it;
				delta = dd(c, c[in_x-1][in_y-1], tmpTarget, c[out_x-1][out_y-1], Q, Path, Path2);
				vector<cell*> tmpPath1 = Path, tmpPath2 = Path2;
				vector<cell*> vtartmp;
				
				if(!delta)
				{
					for(it1 = tmpPath1.begin(); it1 != tmpPath1.end(); ++it1)
					{
						(*it1)->is_path = false;
						if((*it1)->is_tar && (*it1) != tmpTarget)
						{
							vtartmp.push_back((*it1));
							(*it1)->is_tar = false;
						}
					}
					tmpPath1.clear();
					delta = dd(c, c[in_x-1][in_y-1], tmpTarget, c[out_x-1][out_y-1], Q, tmpPath1, Path2);
					if(!delta)
					{
						for(it1 = tmpPath1.begin(); it1 != tmpPath1.end(); ++it1) (*it1)->is_path = false;
						tmpPath1.clear();
						delta = dd(c, c[out_x-1][out_y-1], tmpTarget, c[in_x-1][in_y-1], Q, tmpPath1, Path2);
					}
					for(it1 = vtartmp.begin(); it1 != vtartmp.end(); ++it1)
					{
						(*it1)->is_tar = true;
					}
				}

				if(delta) Path = tmpPath1;
				else
				{
					tmpPath1 = Path;
					for(it1 = tmpPath1.begin(); it1 != tmpPath1.end(); ++it1)
					{
						if((*it1)->is_path) (*it1)->is_path = false;
					}
					delta = dd(c, c[out_x-1][out_y-1], tmpTarget, c[in_x-1][in_y-1], Q, tmpPath1, Path2);
				}
				
				if(delta) Path = tmpPath1;
				else
				{
					int i = 0;
					tmpPath1 = Path;
					for(it1 = tmpPath1.begin(); it1 != tmpPath1.end(); ++it1)
					{
						if((*it1)->is_path) (*it1)->is_path = false;
					}
					while(!delta && i < vtartmp.size() && vtartmp[i] != tmpTarget)
					{
						cout << "FUCK2\n";
						vtartmp[i]->is_tar = false;
						for(it1 = tmpPath1.begin(); it1 != tmpPath1.end(); ++it1) (*it1)->is_path = false;
						tmpPath1.clear();
						delta = dd(c, c[in_x-1][in_y-1], tmpTarget, c[out_x-1][out_y-1], Q, tmpPath1, Path2);
						if(!delta)
						{
							for(it1 = tmpPath1.begin(); it1 != tmpPath1.end(); ++it1) (*it1)->is_path = false;
							tmpPath1.clear();
							delta=dd(c, c[out_x-1][out_y-1], tmpTarget, c[in_x-1][in_y-1], Q, tmpPath1, Path2);
						}
						vtartmp[i]->is_tar = true;
						++i;						
					}
				}

				if(delta) Path = tmpPath1;
				else
				{
					Path = tmpPath1;
					cout << "OH FUCK\n";
				}

				if(Path2.size() >= 2)
					Path.insert(Path.end(), ++Path2.begin(), Path2.end());
				
				for(it1 = Path.begin(); it1 != Path.end(); ++it1)
				{
					(*it1)->is_path = true;
					if((*it1)->is_tar)
					{
						(*it1)->is_tar = false;
						--tar_num;
					}
				}

				vPath.push_back(Path);
			}
		}
	}
	
	// cout << endl << vtar.size() << endl;

	/*for(int i = 0; i < Path.size(); ++i)
	{
		cout <<  "(" << Path[i]->x << "," << Path[i]->y << ")";
	}
	cout << endl;*/

	/*for(int i = 0; i < vPath.size(); ++i)
	{
		for(int j = 0; j < vPath[i].size(); ++j)
		{
			cout << "(" << vPath[i][j]->x << "," << vPath[i][j]->y << ")";
		}
		cout << endl;
	}*/

	ofstream fout(argv[2]);
	fout << vPath.size() << endl;
	for(int i = 0; i < vPath.size(); ++i)
	{
		fout << vPath[i].size() << " ";
		for(int j = 0; j < vPath[i].size(); ++j)
		{
			fout << vPath[i][j]->x << " " << vPath[i][j]->y << " ";
		}
		fout << endl;
	}

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

void initializeSingleSource(vector<vector<cell*> > G, cell* s, bool reset)
{
	for(int i = 0; i < G.size(); ++i)
	{
		for(int j = 0; j < G[i].size(); ++j)
		{
			G[i][j]->d = 1e9;
			G[i][j]->parent.clear();
			G[i][j]->is_visit = false;
			
			if(reset == 1)
			G[i][j]->is_path = false;
		}
	}
	s->d = 0;
}

void relax(cell* u, cell* v, int w, vector<cell*>& Q)
{
	if(v->d >= u->d + w)
	{
		v->d = u->d + w;
		v->parent.clear();
		v->parent.push_back(u);
		decrease_key(Q, v, v->d);
	}
	else if(v->d == u->d + w && v->is_out == true) v->parent.push_back(u);
}

int weight(cell* u, cell* v)
{
	return (u->is_tar == true || v->is_tar == true)? 1 : 3;
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

bool nonNeighboringViolation(cell* child, cell* parent)
{
	vector<cell*>::iterator it1;
	for(it1 = child->neighbor.begin(); it1 != child->neighbor.end(); ++it1)
	{
		if((*it1)->is_path && (*it1) != parent && (*it1)->dfsParent != NULL)
		{
			// cout << "FUCK (" << (*it1)->x << "," << (*it1)->y << ")" <<' ';
			return true;
		}
	}
	return false;
}

void insertion(vector<cell*> &v, int p, int r)
{
	for (int i = p + 1; i <= r; ++i)
	{
		cell* key = new cell(v[i]->x, v[i]->y);
		key->d = v[i]->d;
		// ELEMENT key = ELEMENT(v[i].get_id(), v[i].get_value());
		int j = i - 1;
		while (j >= p && v[j]->d > key->d)
		{
			v[j + 1] = v[j];
			j -= 1;
		}
		v[j + 1] = key;
	}
}

vector<cell*> dfs_iterative(cell* v)
{
	int targetPassThrough = 0, count = 0;
	bool violation = false;
	vector<cell*> path;
	vector<cell*>::iterator it, it1;
	stack<cell*> S;
	cell* u;
	S.push(v);
	v->dfsParent = NULL;
	v->is_path = true;
	while(!S.empty())
	{
		v = S.top();
		S.pop();
		if(!v->parent.empty())
		{
			for(it = v->parent.begin(); it != v->parent.end(); ++it)
			{
				cell *tmpchild = *it;
				cell* tmpNeighbor = new cell(1, 1);
				tmpNeighbor->d = 1e9;
				if(!tmpchild->parent.empty() && nonNeighboringViolation(tmpchild, v))
				{
					for(it1 = v->neighbor.begin(); it1 != v->neighbor.end(); ++it1)
					{
						if(!(*it1)->is_path && (*it1) != tmpchild && (*it1)->d <= tmpNeighbor->d && !(*it1)->is_obs)
						{
							tmpNeighbor = *it1;
						}
					}
					tmpchild = tmpNeighbor;
					if(tmpNeighbor->d == 1e9)
					{
						return path;
					}
				}
				S.push(tmpchild);
				tmpchild->dfsParent = v;
				tmpchild->is_path = true;
			}
		}
		else
		{
			// cout << "v : (" << v->x << "," << v->y << ") ";
			int tmpTPT = 0;
			std::vector<cell*> tmpPath;
			tmpPath.push_back(v);
			if(v->is_tar) ++tmpTPT;
			// cout << "(" << v->x << "," << v->y << ")";
			cell* tmpc;
			tmpc = v;
			while(tmpc->dfsParent != NULL)
			{
				tmpc = tmpc->dfsParent;
				tmpPath.push_back(tmpc);
				if(tmpc->is_tar) ++tmpTPT;
				// cout << "(" << tmpc->x << "," << tmpc->y << ")";
			}
			if(tmpTPT >= targetPassThrough) path = tmpPath;
			// cout << endl;
		}
	}
	if(path.size() == 1)
	{
		path.clear();
	}
	return path;
}

bool dijkstra(vector<vector<cell*> > c, cell* source, cell* target, vector<cell*> Q, bool reset)
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
			if(!u->parent.empty()) return true;
			else return false;

		}
		for(it = u->neighbor.begin(); it != u->neighbor.end(); ++it)
		{
			if((*it)->is_obs || (*it)->is_visit || (*it)->is_path) continue;
			// cout << "(" << (*it)->x << "," << (*it)->y << ")";
			relax(u, (*it), weight(u, (*it)), Q);
		}
	}
	return false;
	// return true;
	/*cout << endl;
	for(int i = 0; i < c.size(); ++i)
	{
		for(int j = 0 ; j < c[i].size(); ++j)
		{
			cout << "C (" << c[i][j]->x << "," << c[i][j]->y << ") : ";
			cout << c[i][j]->d << endl;
		}
	}*/
}

bool dd(vector<vector<cell*> > c, cell* start, cell* temp, cell* end, vector<cell*> Q, vector<cell*>& path1, vector<cell*>& path2)
{
	if(!dijkstra(c, start, temp, Q, 1))
	{
		// cout << "No path from (" << start->x << "," << start->y << ") to ";
		// cout << "(" << temp->x << "," << temp->y << ") exists.\n";
	}
	path1 = dfs_iterative(temp);
	if(!dijkstra(c, temp, end, Q, 0))
	{
		// cout << "No path from (" << temp->x << "," << temp->y << ") to ";
		// cout << "(" << end->x << "," << end->y << ") exists.\n";		
	}
	path2 = dfs_iterative(end);
	if(path2.empty()) return false;
	return true;
}
