#include <iostream>

#include "stdafx.h"

using namespace std;

int main( int argc, char* argv[] ) 
{
	cout << "Hello World!" << endl;

	// Constants
	double beta = 0.8;

	// Gets the ids of the papers from user whose similarity is to be calculated
	// also gets the path of the dataset from the user
	int nodeIdA;
	int nodeIdB;
	string dataSetPath;
	getUserInput( &nodeIdA, &nodeIdB, &dataSetPath );

	// Reads the graph from dataSetPath into PGraph using snap
	// updates N to the size of the graph
	int N;
	PNGraph *graph;
	graphInit(dataSetPath, graph, &N);

	// Initializes the default values of the row vector rOld
	double *rOld;
	rOldInit(rOld, N, beta);

	double *rNew = new double[N];

	// Calculates simRank i.e pageRank with restarts with respect to nodeIdA
	bool converged = false;
	while( converged == false )
	{
		for( int j = 0; j < N; j++ )
		{
			// gets the total number of inNodes for a particular node given its id
			int noOfInNodes = getNoOfInNodes(nodeIdA, graph);

			if( noOfInNodes < 1 )
			{
				rNew[j] = 0;
			}
			else
			{
				// gets the list of inNodes of a particular node given its id
				int* inNodesList = getInNodesList(nodeIdA, graph);

				double sum = 0;

				for( int i = 0; i < noOfInNodes; i++ )
				{
					// gets the out-degree of a particular in-node given its id
					int outDegreeOfInNode = getOutDegreeOfInNode(inNodesList[i]);

					sum += beta * ( rOld[i] / outDegreeOfInNode );
				}

				rNew[j] = sum;
			}
		}

		// Re-insert the leaked PageRank
		double S = 0;
		for( int i = 0; i < N; i++ )
		{
			S += rNew[i];
		}
		// Since, it's pageRank with restarts, teleport set contains only one node
		// Hence, only one update is required
		rNew[nodeIdA] += (1 - S) / N;	

		// given the old and new row vectors, rOld and rNew,
		// tells if convergence has been reached
		converged = isConverged(rOld, rNew);

		// rOld becomes rNew
		// delete rOld, create new rOld, init rOld to same values as rNew
		rOldBecomesRNew(rOld, rNew);
	}	

	// Should return true iff the sum of all values of the row vector rNew
	// is equal to 1
	bool finalCheck = isCorrect(rNew);

	if( finalCheck )
	{
		cout << "Success!" << endl;

		cout << "simRank of " << nodeIdB << " with respect to " << nodeIdA << " is: " << rNew[nodeIdB] << endl;
	}
	else
		cout << "Failure!" << endl;

	// Clean up
	delete[] rOld;
	delete[] rNew;

  	return 0;
}



