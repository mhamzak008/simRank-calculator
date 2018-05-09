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
#include "cocitation.cpp"
#include "bibliographiccoupling.cpp"
#include "simrankSerial.cpp"

int main( int argc, char* argv[] ) 
{
	// Timer Start
	std::clock_t start;
    double duration;
    start = std::clock();

    cout << endl;
    cout << "--------------------------Starting simRank on Pthreads--------------------------" << endl;
    TInt* result = new TInt[100];
    calculateSimRank(argc, argv, result);
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    cout <<"Time Taken So Far: "<< duration << " seconds" << endl;


    cout << endl;
    cout << "-------------------------Starting simRank using openMP------------------------" << endl;
    startSimRankMP(argc, argv);
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    cout <<"Time Taken So Far: "<< duration << " seconds" << endl;


    cout << endl;
    cout << "--------------------------Starting simRank serial version--------------------" << endl;
    startSimRankSerial(argc, argv);
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    cout <<"Time Taken So Far: "<< duration << " seconds" << endl;


    cout << endl;
    cout << "-------------------------Top 10 most similar node ids-----------------------" << endl;
    for(int i = 0; i < 10; i++)
    {
        cout << result[i] << endl;
    }

    // std::sort (result,result+100);     //  5 10 15 20 25

    // cout << endl;
    // cout << "-------------------------Starting Co-Citation---------------------------------" << endl;
    // int emptyArray2[100];
    // int* cocitation = calculateCoCitation(argc, argv, emptyArray2);

    // // Co-citation intersection with simRank    
    // std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
    // std::vector<int>::iterator it;
    // std::sort (cocitation,cocitation+100);   // 10 20 30 40 50    
    // it=std::set_intersection (result, result+100, cocitation, cocitation+100, v.begin());
    // v.resize(it-v.begin());    
    // std::cout << "The Co-Citation Intersection with simRank has " << (v.size()) << " elements:\n";    
    // duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    // cout <<"Time Taken So Far: "<< duration << " seconds" << endl;


    // cout << endl;
    // cout << "-------------------------Starting Bibliographic Coupling-----------------------" << endl;
    // int emptyArray3[100];    
    // int* biblio = calculateBibliographicCoupling(argc, argv, emptyArray3);   

    // // Bibliographic intersection with simRank
    // std::vector<int> v2(10);                      // 0  0  0  0  0  0  0  0  0  0
    // std::vector<int>::iterator it2;
    // std::sort (biblio,biblio+100);   
    // it2=std::set_intersection (result, result+100, biblio, biblio+100, v2.begin());
    // v2.resize(it2-v2.begin());    
    // std::cout << "The Bibliographic Intersection with simRank has " << (v2.size()) << " elements:\n";


    // Timer Stop
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    cout <<"Time Taken Total: "<< duration << " seconds" << endl;

	return 0;
}

