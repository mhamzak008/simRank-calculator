#include <iostream>

#include "stdafx.h"
#include "Snap.h"
#include <map>
#include <ctime>
#include <omp.h>
struct timespec start, finish, start1, finish1;
double elapsed, elapsed1;

using namespace std;

int startSimRankMP( int argc, char* argv[] ) 
{
	Env = TEnv(argc, argv, TNotify::StdNotify);
	clock_gettime(CLOCK_MONOTONIC, &start);


	// Constants
	double beta = 0.8;

	//Variables
	// char *dataSetPath2 = (char*)calloc(512, sizeof(char*));
	// char line[200];

	// Gets the ids of the papers from user whose similarity is to be calculated
	// also gets the path of the dataset from the user
	if(argc != 3)
	{
		printf("Usage: %s [node-id A] [dataset path]\n", argv[0]);
		exit(1);
	}
	TInt nodeIdA = atoi(argv[1]);
//	TInt nodeIdB = atoi(argv[2]);
	TStr dataSetPath = argv[2];
//	dataSetPath2 = argv[3];

	// Reads the graph from dataSetPath into PGraph using snap
	// updates N to the size of the graph
	const TStr InFNm = Env.GetIfArgPrefixStr("-i:", dataSetPath, "Input directed graph file");
	PNGraph graph = TSnap::LoadEdgeList<PNGraph>(InFNm, 0, 1); 

	int N = graph->GetNodes();
//	double Ndb = N;
	//Generates key-val pairs from graph
	int i = 0;
	map <int, int> ids;
	map <int, int> reverseids;
	for (TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)		
	{
		ids.insert(pair <int, int> (NI.GetId(), i));
		reverseids.insert(pair <int, int> (i, NI.GetId()));
		i++;
	}

	// Initializes the default values of the row vector rOld --> rOldInit
	// Should we initialize all rOld values to 0 except for the starting node?
	// Or should we assign equal initial probabilities to all nodes?
	double *rOld= new double[N];

	// initializing all nodes with equal initial porbabilities
	// double val= 1 / Ndb;
	// fill(rOld, rOld+N, val);

	// initializing all rOld values to 0 except for the starting node
	fill(rOld, rOld+N, 0);
	rOld[ids.find(nodeIdA)->second] = 1;


	// calculate inNodes for all graphs
	map <TInt, TInt*> preProcessedInNodes;
//	int iter = 0;

	clock_gettime(CLOCK_MONOTONIC, &start1);
//	#pragma omp parallel for
	for(int i = 0; i < ids.size(); i++) 
	{
		TNGraph::TNodeI NI = graph->GetNI(reverseids.find(i)->second);
		//printf("pre-process %d\n", iter);
		preProcessedInNodes.insert(pair <TInt, TInt*> (NI.GetId(), getInNodesList(NI.GetId(), graph, N)));
		//iter++;
	}
	clock_gettime(CLOCK_MONOTONIC, &finish1);

	elapsed1 = (finish1.tv_sec - start1.tv_sec);
	elapsed1 += (finish1.tv_nsec - start1.tv_nsec) / 1000000000.0;
	cout << "Preprocess: " << elapsed1 << endl;


	/*for(TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++) {
		//printf("pre-process %d\n", iter);
		preProcessedInNodes.insert(pair <TInt, TInt*> (NI.GetId(), getInNodesList(NI.GetId(), graph, N)));
		//iter++;
	}*/	


	double *rNew = new double[N];

	// Calculates simRank i.e pageRank with restarts with respect to nodeIdA

	bool converged = false;
	int iteration = 0;
	while( !converged )
	{	
		printf("iteration: %d\n", iteration);
		for (TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)		
		{
			// gets the total number of inNodes for a particular node given its id
			//TNGraph::TNodeI NI = graph->GetNI(j);
			int noOfInNodes = NI.GetInDeg();
			int NId = NI.GetId();
			
			if( noOfInNodes < 1 )
			{
				rNew[ids.find(NId)->second] = 0;
			}
			else
			{
				// gets the list of inNodes of a particular node given its id
				double sum = 0;				
				TInt* inNodesList = preProcessedInNodes[NId];
			//	#pragma omp parallel for private(i)
				
				for( int i = 0; i < noOfInNodes; i++ )
				{
					// gets the out-degree of a particular in-node given its id
					TNGraph::TNodeI MI = graph->GetNI(inNodesList[i]);
					int outDegreeOfInNode = MI.GetOutDeg();
					
					sum += beta * ( rOld[ids.find(inNodesList[i])->second] / outDegreeOfInNode );
				}
				rNew[ids.find(NId)->second] = sum;
			}
		}

		// Re-insert the leaked PageRank
		// Should we re-insert it to all nodes?
		// or should we re-insert it to only the starting node?
		// Calculating what needs to be re-inserted
		double S = 0;
		for( int i = 0; i < N; i++ )
		{
			S += rNew[i];
		}
		// re-inserted
		rNew[ids.find(nodeIdA)->second] += (1.0 - S);
		// debug
		/*for( int i = 0; i < 4; i ++ )
		{
			rNew[i] += (1.0 - S) / Ndb;
		}*/

		// given the old and new row vectors, rOld and rNew,
		// tells if convergence has been reached
		iteration++;
		if(iteration == 5)
			converged = true;

		//converged = isConverged(rOld, rNew, N);

		// rOld becomes rNew
		// delete rOld, create new rOld, init rOld to same values as rNew done
		for(int i = 0 ; i < N; i++){
			rOld[i] = rNew[i];
		}
	}
	
	// Should return true iff the sum of all values of the row vector rNew is equal to 1
	bool finalCheck = isCorrect(rNew, N);
	if( finalCheck )
	{
		cout << "Success!" << endl;

	//	cout << "simRank of " << nodeIdB << " with respect to " << nodeIdA << " is: " << rNew[ids.find(nodeIdB)->second] << endl;
		
	}
	else
		cout << "Failure!" << endl;

	// Clean up
	delete[] rOld;
	delete[] rNew;

	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	cout << elapsed << endl;

	return 0;
}

