#include <iostream>

#include "stdafx.h"
#include <cstdio>
#include <ctime>
#include <algorithm>

using namespace std;

//#include "simrank.cpp"
#include "cocitation.cpp"
#include "bibliographiccoupling.cpp"

int main( int argc, char* argv[] ) 
{
	// Timer Start
	std::clock_t start;
    double duration;
    start = std::clock();

    int emptyArray1[100];
    int emptyArray2[100];
    int emptyArray3[100];

    for( int i = 0; i < 100; i++ )
    {
    	emptyArray1[i] = -111;
    	emptyArray2[i] = -222;
    	emptyArray3[i] = -333;
    }

 //   node* simRank = calculateSimRank(argc, argv, emptyArray1);
  //  int* cocitation = calculateCoCitation(argc, argv, emptyArray2);
    int* biblio = calculateBibliographicCoupling(argc, argv, emptyArray3);

    // for(int i = 0; i < 100; i++)
    // {
    // 	cout << "Node:: " << cocitation[i] << endl;
    // }

    for(int i = 0; i < 100; i++)
    {
    	cout << "Node:: " << biblio[i] << endl;
    }


	// Timer Stop
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    cout <<"Time Taken: "<< duration << " seconds" << endl;

	return 0;
}

