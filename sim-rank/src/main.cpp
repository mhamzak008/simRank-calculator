#include <iostream>

#include "stdafx.h"
#include <cstdio>
#include <ctime>

struct node
{
	double value;	// similarity value
	int id; 		// node id
};

#include "simrank.cpp"
#include "bibliographiccoupling.cpp"
#include "cocitation.cpp"

using namespace std;

int main( int argc, char* argv[] ) 
{
	// Timer Start
	std::clock_t start;
    double duration;
    start = std::clock();

    node emptyArray1[100];
    node emptyArray2[100];
    node emptyArray3[100];

    for( int i = 0; i < 100; i++ )
    {
    	emptyArray1[i].value = 0;
    	emptyArray2[i].value = 0;
    	emptyArray3[i].value = 0;
    }

    node* simRank = calculateSimRank(argc, argv, emptyArray1);
    //double* cocitation = calculateCoCitation(argc, argv, emptyArray2);
    // double* biblio = calculateBibliographicCoupling(argc, argv, emptyArray3);

	// Timer Stop
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    cout <<"Time Taken: "<< duration << " seconds" << endl;

	return 0;
}

