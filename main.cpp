#include <iostream>
#include <fstream>

using namespace std;

int main (int argc, char* argv[])
{
	ifstream fin(argv[1]);
	if (!fin.is_open()) cout << "No Imput Files.\n";
}