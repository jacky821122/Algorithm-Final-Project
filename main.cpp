#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

class cell
{
public:
	cell(int a, int b){ x = a; y = b; is_in = 0; is_out = 0; is_tar = 0; is_obs = 0; is_visit = 0;};
	~cell(){};
	int x, y;
	bool is_in, is_out, is_tar, is_obs, is_visit;
	vector<cell*> parent, child;
};

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

					/*----------Adding All Cells' child-----------*/
					if(i - 1 != 0)
					{
						tmpc->child.push_back(c[i - 2][j - 1]);
						c[i - 2][j - 1]->child.push_back(tmpc);
					}
					if(j - 1 != 0)
					{
						std::vector<cell*>::reverse_iterator rit = vcCol.rbegin();
						tmpc->child.push_back(*(++rit));
						(*rit)->child.push_back(tmpc);
					}
					/*----------Adding All Cells' child-----------*/
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

	/*for(int i = 0; i < m; ++i)
	{
		for(int j = 0 ; j < n; ++j)
		{
			cout << "C (" << c[i][j]->x << "," << c[i][j]->y << ") : ";
			cout << c[i][j]->child.size() << endl;
		}
	}*/		/*--------------All Cells' Coordinate and Their #(child)--------------------*/

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

