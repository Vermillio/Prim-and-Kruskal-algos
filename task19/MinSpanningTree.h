#pragma once
#include "T_Graph.h"
#include <vector>
#include <set>
using namespace std;
using namespace Graph;

namespace MinSpanTree
{
	struct SpanTree
	{
		T_Graph TreeData;
		int totalWeight;
		void print();
	};


	class MinSpanningTreeFinder
	{

		int sTweight;

		vector<set<edge>> spanTree;

		set<edge_data>	 sortEdges(vector<set<edge>> _graph);

		edge_data FindVergeWithMinimalWeight(set<int> &used, vector<set<edge>> graph);

		void ErrorMessage(string _error);

		vector<set<int>> cc;

		void initCC();

		bool fromSameCC(int x, int y);

	public:
		//void printSpanTree();
		T_Graph getLastSpanTree();

		SpanTree PrimAlgo(T_Graph _graph);
		vector<SpanTree> PrimAlgo(vector<T_Graph> _graphs);

		SpanTree CruscalAlgo(T_Graph _graph);
		vector<SpanTree> CruscalAlgo(vector<T_Graph> _graphs);

		void CompareAlgos(vector<T_Graph> _graphs);
		void demo();
	};
}