#include <iostream>

#include "stdafx.h"
#include "Snap.h"
#include <map>
#include <pthread.h>
#include <unordered_map>
#include <ctime>
#include <algorithm>

using namespace std;

#include "simrankPT.cpp"
#include "simrankMP.cpp"
// #include "cocitation.cpp"
// #include "bibliographiccoupling.cpp"

int main( int argc, char* argv[] ) 
{
	// Timer Start
	// std::clock_t start;
 //    double duration;
 //    start = std::clock();

    // int emptyArray2[100];
    // int emptyArray3[100];

    cout << "----------------------Starting simRank on Pthreads----------------------" << endl;
    TInt* emptyArray1 = new TInt[100];
    calculateSimRank(argc, argv, emptyArray1);

    cout << endl;
    cout << endl;

    cout << "----------------------Starting simRank using openMP-----------------------" << endl;
    startSimRankMP(argc, argv);

  //  int* cocitation = calculateCoCitation(argc, argv, emptyArray2);
  //  int* biblio = calculateBibliographicCoupling(argc, argv, emptyArray3);

    // for(int i = 0; i < 100; i++)
    // {
    // 	cout << "Node:: " << cocitation[i] << endl;
    // }

 //    for(int i = 0; i < 100; i++)
 //    {
 //    	cout << "Node:: " << biblio[i] << endl;
 //    }


	// // Timer Stop
	// duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
 //    cout <<"Time Taken: "<< duration << " seconds" << endl;

	return 0;
}

