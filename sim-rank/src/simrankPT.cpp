PNGraph graph;
double *rOld;
double *rNew;

map <int, int> reverseIds;
map<TInt, TInt*> preProcessedInNodes;
map <int, int> ids;

void *Runner(void *args) {
	int* args2 = (int *) args;
	int start = args2[0];
	int end = args2[1];

	TNGraph::TNodeI begin = graph->GetNI(reverseIds.find(start)->second);
	TNGraph::TNodeI finish = graph->GetNI(reverseIds.find(end)->second);

	while (begin < finish) {
		int noOfInNodes = begin.GetInDeg();
		int NId = begin.GetId();

		if (noOfInNodes < 1) {
			rNew[start] = 0;
		}
		else {
			TInt* inNodesList = preProcessedInNodes[NId];

			double sum = 0;
			for (int i = 0; i < noOfInNodes; i++) {
				TNGraph::TNodeI MI = graph->GetNI(inNodesList[i]);
				int outDegreeOfInNode = MI.GetOutDeg();

				sum += 0.8 * (rOld[ids.find(inNodesList[i])->second] / outDegreeOfInNode);
			}
			rNew[start] = sum;
		}
		start++;
		begin++;
	}

	pthread_exit(0);

}

bool isCorrect(double* rNew, int size)
{
	double sum = 0.0;

	for( int i = 0; i < size; i++ )
	{
		sum += rNew[i];
	}
//	cout << "Sum is: "<< sum << endl;
	if( sum >= 0.99999 && sum <= 1.0001)
	{		
		return true;
	}

	return false;
}

bool isConverged(double* rOld, double* rNew, int size) {
	double threshold = 0.00001;

	double avg_difference = 0.0;

	for (int i = 0; i < size; i++) {
		double diff = (rNew[i] - rOld[i]);
		avg_difference += diff > 0 ? diff : -diff;
	}

	avg_difference /= size;
	if (avg_difference <= threshold) {
		return true;
	}
	return false;

}

TInt* getInNodesList(TInt nodeId, PNGraph graph, int size) {
	TIntV allNodeIds = TIntV(size);
	graph->GetNIdV(allNodeIds);

	TNGraph::TNodeI NI = graph->GetNI(nodeId);
	int noOfInNodes = NI.GetInDeg();

	TInt* result = new TInt[noOfInNodes];
	int index = 0;

	for (int i = 0; i < size; i++) {
		TNGraph::TNodeI current = graph->GetNI(allNodeIds[i]);
		if (current.IsOutNId(nodeId)) {
			result[index] = allNodeIds[i];
			index++;
		}
	}

	// verify that k nodes are returned and k was the initial outdegree of starting node
	if (noOfInNodes != index) {
		cout << "Something went wrong in getInNodesList\n";
	}

	return result;
}
// node id a: 212168, node id b: 112041
void calculateSimRank( int argc, char** argv, TInt *output) 
{
	Env = TEnv(argc, argv, TNotify::StdNotify);

	// Constants
//	double beta = 0.8;

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
	TStr dataSetPath = argv[2];
//	dataSetPath2 = argv[3];

	// Reads the graph from dataSetPath into PGraph using snap
	// updates N to the size of the graph
	const TStr InFNm = Env.GetIfArgPrefixStr("-i:", dataSetPath, "Input directed graph file");
	graph = TSnap::LoadEdgeList<PNGraph>(InFNm, 0, 1); 

	int N = graph->GetNodes();
//	double Ndb = N;
	//Generates key-val pairs from graph
	int i = 0;
	
	for (TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)		
	{
		ids.insert(pair <int, int> (NI.GetId(), i));
		reverseIds.insert(pair <int, int> (i, NI.GetId()));
		i++;
	}

	// Initializes the default values of the row vector rOld --> rOldInit
	// Should we initialize all rOld values to 0 except for the starting node?
	// Or should we assign equal initial probabilities to all nodes?
	rOld = new double[N];

	// initializing all nodes with equal initial porbabilities
	// double val= 1 / Ndb;
	// fill(rOld, rOld+N, val);

	// initializing all rOld values to 0 except for the starting node
	fill(rOld, rOld+N, 0);
	rOld[ids.find(nodeIdA)->second] = 1;
	clock_t begin = clock();
	// calculate inNodes for all graphs
	int iter = 0;
	for(TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++) {
		if (iter % 10000 == 0)
			printf("pre-process %d\n", iter);
		preProcessedInNodes.insert(pair <TInt, TInt*> (NI.GetId(), getInNodesList(NI.GetId(), graph, N)));
		iter++;
	}	
	clock_t end = clock();
	printf("data preprocessing done in: %lf\n", (double) (end - begin));

	rNew = new double[N];

	// Calculates simRank i.e pageRank with restarts with respect to nodeIdA

	bool converged = false;
	int iteration = 0;
	
	for (int j = 0; j < 10; j++) {
	//	cout << "iter start: " << j << endl;
		struct timespec start, finish;
		double elapsed;
		clock_gettime(CLOCK_MONOTONIC, &start);
		while( !converged )
		{
			// 7-thread configureation
			pthread_t tids[7];
			int args0[] = {0, 4935};
			int args1[] = {4935, 9870};
			int args2[] = {9870, 14805};
			int args3[] = {14805, 19740};
			int args4[] = {19740, 19740+4935};
			int args5[] = {19740+4935, 19740+4935+4935};
			int args6[] = {19740+4935+4935, 34546};

			pthread_create(&tids[0], NULL, Runner, (void *) args0);
			pthread_create(&tids[1], NULL, Runner, (void *) args1);
			pthread_create(&tids[2], NULL, Runner, (void *) args2);
			pthread_create(&tids[3], NULL, Runner, (void *) args3);
			pthread_create(&tids[4], NULL, Runner, (void *) args4);
			pthread_create(&tids[5], NULL, Runner, (void *) args5);
			pthread_create(&tids[6], NULL, Runner, (void *) args6);

			pthread_join(tids[0], NULL);
			pthread_join(tids[1], NULL);
			pthread_join(tids[2], NULL);
			pthread_join(tids[3], NULL);
			pthread_join(tids[4], NULL);
			pthread_join(tids[5], NULL);
			pthread_join(tids[6], NULL);
			
			// 4-thread configuration
			/*
			pthread_t tids[4];
			int args0[] = {0, 8636};
			int args1[] = {8636, 17273};
			int args2[] = {17273, 17273+8636};
			int args3[] = {17273+8636, 34546};

			pthread_create(&tids[0], NULL, Runner, (void *) args0);
			pthread_create(&tids[1], NULL, Runner, (void *) args1);
			pthread_create(&tids[2], NULL, Runner, (void *) args2);
			pthread_create(&tids[3], NULL, Runner, (void *) args3);

			pthread_join(tids[0], NULL);
			pthread_join(tids[1], NULL);
			pthread_join(tids[2], NULL);
			pthread_join(tids[3], NULL);
			*/
			
			// 2-thread configuration
			/*
			pthread tids[2];
			int args0[] = {0, 17273};
			int args1[] = {17273, 34546};
			
			pthread_create(&tids[0], NULL, Runner, (void *) args0);
			pthread_create(&tids[1], NULL, Runner, (void *) args1);
			
			pthread_join(tids[0], NULL);
			pthread_join(tids[1], NULL);
			*/
			
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
		converged = false;
		iteration = 0;
		bool finalCheck = isCorrect(rNew, N);
		clock_gettime(CLOCK_MONOTONIC, &finish);

		elapsed = (finish.tv_sec - start.tv_sec);
		elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
		if( finalCheck )
		{
//			cout << "Success!" << endl;
			cout << "Calculation time:" << (double) elapsed << endl;
		}
		else
			cout << "Failure!" << endl;

		fill(rOld, rOld+N, 0);
		rOld[ids.find(nodeIdA)->second] = 1;
//		cout << "iter end: " << j << endl;
	}
	
	
	// Should return true iff the sum of all values of the row vector rNew is equal to 1
	

	// generate key value pairs for index and sim-rank value
	pair<double, TInt> rNewPaired[N];
	for (int i = 0; i < N; i++) {
		rNewPaired[i] = pair<double, TInt> (rNew[i], reverseIds.find(i)->second);
	}
	// sort the paired array
	sort(rNewPaired, rNewPaired+N);
	// fill the return array with the top 100 matches
	for (int i = 0; i < 100; i++) {
		output[i] = rNewPaired[N-i-1].second;
	}

	// Clean up
	delete[] rOld;
	delete[] rNew;

}
