#include <algorithm>

struct node
{
	int value;
	int id;
};

bool acompare(node lhs, node rhs) 
{ 
	return lhs.value > rhs.value; 
}

int* calculateCoCitation( int argc, char* argv[], int output[] ) 
{
	Env = TEnv(argc, argv, TNotify::StdNotify);

	// Gets the ids of the papers from user whose similarity is to be calculated
	// also gets the path of the dataset from the user
	if(argc != 3)
	{
		printf("Usage: %s [node-id A] [dataset path]\n", argv[0]);
		exit(1);
	}
	TInt nodeIdA = atoi(argv[1]);
	TStr dataSetPath = argv[2];

	// Reads the graph from dataSetPath into PGraph using snap
	const TStr InFNm = Env.GetIfArgPrefixStr("-i:", dataSetPath, "Input directed graph file");
	PNGraph graph = TSnap::LoadEdgeList<PNGraph>(InFNm, 0, 1); 

	// updates N to the size of the graph
	int N = graph->GetNodes();

	TInt* inNodesListA = getInNodesList(nodeIdA, graph, N);

	TNGraph::TNodeI NIA = graph->GetNI(nodeIdA);
	int noOfInNodesA = NIA.GetInDeg();

	// iterate through the graph to calculate the co-citation value of all other nodes with respect to nodeIdA
	node citationValues[N];
	int c = 0;
	for( TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++ )		
	{	
		TInt* inNodesListTemp = getInNodesList(NI.GetId(), graph, N);
		int noOfInNodesTemp = NI.GetInDeg();

		int count = 0;
		for( int i = 0; i < noOfInNodesA; i++ )
		{
			for( int j = 0; j < noOfInNodesTemp; j++ )
			{
				if( inNodesListA[i] == inNodesListTemp[j] )
				{
					count++;
					j = noOfInNodesTemp;
				}
			}
		}

		// put the value
		citationValues[c].value = count;
		citationValues[c].id = NI.GetId();
		c++;
	}	

	// for(int i = 0; i < N; i++)
	// {
	// 	cout << "Id: " << citationValues[i].id << " | Value: " << citationValues[i].value << endl;
	// }

	// cout << "===============SORTING==========" << endl;

	std::sort(citationValues, citationValues+N, acompare);

	// for(int i = 0; i < N; i++)
	// {
	// 	cout << "Id: " << citationValues[i].id << " | Value: " << citationValues[i].value << endl;
	// }

	// getting the top 100 values from citationValues
    for( int i = 0; i < 100; i++ )
    {
      	output[i] = citationValues[i].id;
    }

    return output;
}