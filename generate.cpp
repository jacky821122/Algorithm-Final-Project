#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <time.h>
#include <vector>
#include <algorithm>

using namespace std;
class coordinates{
    public:
    coordinates(int a,int b);
    int x;
    int y;
};

bool operator==(const coordinates &c1, const coordinates &c2){
    if(c1.x == c2.x && c1.y == c2.y) return 1 ;
    else return 0;
}

coordinates::coordinates(int a, int b){
    x = a;
    y = b;
}

int main(int argc, char* argv[])
{
    ofstream fout("infile.txt");
    srand(time(NULL));

    vector <coordinates> all, bound;
    vector<coordinates>::iterator it;
    int m, n;
    int i, j;

    cout << "m n" <<endl;
    cin >> m >> n;

    coordinates temp = coordinates(0, 0);
    for( i = 0 ; i < m ; ++i ){
        for( j = 0 ; j < n ; ++j){
            temp = coordinates(i+1,j+1);
            all.push_back(temp);
        }
    }

    for( i = 0 ;  i < m ; ++i){
        temp = coordinates(i+1, 1);
        bound.push_back(temp);
        temp = coordinates(i+1, n);
        bound.push_back(temp);
    }

    for( j = 1 ;  j < n-1 ; ++j){
        temp = coordinates(1, j+1);
        bound.push_back(temp);
        temp = coordinates(m, j+1);
        bound.push_back(temp);
    }

    random_shuffle(bound.begin(), bound.end());
    random_shuffle ( all.begin(), all.end() );
    fout << m << ' ' <<  n << ' ';

    coordinates start = *(bound.begin());
    coordinates end = *(++bound.begin());
    
/*    fout << '(' << start.x << ',' << start.y << ')' << ' ';
    fout << '(' << end.x << ',' << end.y << ')' << ' ' <<endl;  */  
    fout << start.x << ' ' << start.y  << ' ';
    fout << end.x << ' ' << end.y  <<endl;  

    int tnum, onum;
    cout << "target num" << endl;
    cin >> tnum;
    cout << "obstacle num" << endl;
    cin >> onum;

    fout << tnum << ' ';


    for(i = 0 ,it = all.begin() ; i < tnum  ; ++i,++it){
        
        if( start == (*it) || end == (*it) ){
            --i;
            continue;
        }
        //fout << '(' << (*it).x << ',' << (*it).y << ')' << ' ' ;
        fout << (*it).x << ' ' << (*it).y << ' ' ;
      
      }


    fout << endl << onum << ' ';

    
    for(i = 0 ,  ++it; i < onum ; ++i, ++it){
        
        if( start == (*it) || end == (*it) ) {
            --i;
            continue;
        }

        //fout << '(' << (*it).x << ',' << (*it).y << ')' << ' ' ;
        fout << (*it).x << ' ' << (*it).y << ' ' ;

    }
    return 0;
}
