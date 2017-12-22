#include "MinSpanningTree.h"
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <iterator>
#include <list>
#include <string>
#include <ctime>

using namespace std;



void MinSpanTree::MinSpanningTreeFinder::ErrorMessage(string _error)
{
	cout << "Error: " << _error << endl;
}

edge_data MinSpanTree::MinSpanningTreeFinder::FindVergeWithMinimalWeight(set<int> &used, vector<set<edge>> graph)
{
	edge_data res;
	res.to = -1;
	for (int i = 0; i < graph.size(); ++i)
		if (used.find(i) == used.end())
			for (auto j : graph[i])
			{
				if (used.find(j.to) != used.end() && ((res.to < 0 || j.weight < res.weight)))
					res = { j.to, i, j.weight };
			}

	for (auto i = used.begin(); i != used.end(); ++i)
		for (auto j : graph[*i])
			if (((res.to < 0) || (j.weight < res.weight)) && used.find(j.to) == used.end())
			{
				res = { *i, j.to, j.weight };
			}

	used.emplace(res.to);
	return res;
}

T_Graph MinSpanTree::MinSpanningTreeFinder::getLastSpanTree()
{
	return T_Graph();
}

///<summary>returns spanning tree of given graph</summary>
///<returns>spanning tree</returns>
///<param name="_graph">given graph</param>
MinSpanTree::SpanTree MinSpanTree::MinSpanningTreeFinder::PrimAlgo(T_Graph _graph)
{
	sTweight = 0;
	spanTree.clear();
	spanTree.resize(_graph.vertexCount());
	int start = _graph.getStart();
	int end = _graph.getEnd();
	vector<set<edge>> gr = _graph.to_vector();
	
	set<int> used;
	used.insert(start);
	edge_data found;
	while (used.size() != gr.size())
	{
		found = FindVergeWithMinimalWeight(used, gr);
		spanTree[found.from].insert({ found.to, found.weight });
		sTweight += found.weight;
	}
	return { T_Graph(spanTree, start, end), sTweight };
}

vector<MinSpanTree::SpanTree> MinSpanTree::MinSpanningTreeFinder::PrimAlgo(vector<T_Graph> _graphs)
{
	vector<SpanTree> spanTrees;
	spanTrees.resize(_graphs.size());
	for (int i = 0; i < _graphs.size(); ++i)
			spanTrees[i] = PrimAlgo(_graphs[i]);
	return spanTrees;
}

set<edge_data> MinSpanTree::MinSpanningTreeFinder::sortEdges(vector<set<edge>> _graph)
{
	set<edge_data> sorted;

	for (int i = 0; i < _graph.size(); i++)
		for (auto j : _graph[i])
			sorted.insert({ i, j.to, j.weight });
	return sorted;
}

void MinSpanTree::MinSpanningTreeFinder::initCC()
{
	for (int i = 0; i < cc.size(); i++)
		cc[i].insert(i);
}

bool MinSpanTree::MinSpanningTreeFinder::fromSameCC(int x, int y)
{
	bool found = false;
	int index1 = -1, index2 = -1;

	for (int i = 0; i < cc.size(); ++i)
	{
		for (auto p : cc[i])
		{
			if (p == x)
			{
				if (found)
					return true;
				index1 = i;
				found = true;
			}
			else if (p == y)
			{
				if (found)
					return true;
				index2 = i;
				found = true;
			}
				
		}
		found = false;
	}
	if (index1 >= 0 && index2 >= 0)
	{
		for (auto p : cc[index2])
			cc[index1].emplace(p);
		cc[index2].clear();
	}
	return false;
}

MinSpanTree::SpanTree MinSpanTree::MinSpanningTreeFinder::CruscalAlgo(T_Graph _graph)
{
	sTweight = 0;
	spanTree.clear();
	spanTree.resize(_graph.vertexCount());
	set<edge_data> sortedEdges = sortEdges(_graph.to_vector());
	cc.resize(_graph.vertexCount());
	initCC();

	for (auto i : sortedEdges)
		if (!fromSameCC(i.from, i.to))
		{
			spanTree[i.from].insert({ i.to, i.weight });
			sTweight += i.weight;
		}

	return { T_Graph(spanTree, 0, spanTree.size() - 1), sTweight };
}

vector<MinSpanTree::SpanTree> MinSpanTree::MinSpanningTreeFinder::CruscalAlgo(vector<T_Graph> _graphs)
{
	vector<SpanTree> spanTrees;
	spanTrees.resize(_graphs.size());
	for (int i = 0; i < _graphs.size(); ++i)
			spanTrees[i] = CruscalAlgo(_graphs[i]);
	return spanTrees;
}

void MinSpanTree::MinSpanningTreeFinder::CompareAlgos(vector<T_Graph> _graphs)
{
	for (int i = 0; i < _graphs.size(); ++i)
	{
		cout << "     << GRAPH #" << i << " >>" << endl << endl;
		time_t timer1 = clock();
		SpanTree Span1 = PrimAlgo(_graphs[i]);
		timer1 = clock() - timer1;
		Span1.print();
		cout << "Prim algo worked " << timer1 / CLOCKS_PER_SEC << " msec." << endl;
		cout << endl;

		time_t timer2 = clock();
		SpanTree Span2 = CruscalAlgo(_graphs[i]);
		timer2 = clock() - timer1;
		Span2.print();
		cout << endl;

		cout << "Cruscal algo worked " << timer2 / CLOCKS_PER_SEC << " msec." << endl;
		cout << endl << endl;
		GraphCompare(Span1.TreeData, Span2.TreeData);
	}
}

void MinSpanTree::MinSpanningTreeFinder::demo()
{
	string cd = getcd();
	cout << "Make sure graph files are stored in " << getcd() << " ) : " << endl;

	vector<T_Graph> graphs;
	T_Graph input;
	string filename;
	cout << "Enter filenames (/ - end of entering)" << endl;
	while (filename != "/")
	{
		cin >> filename;
		input = T_Graph(cd + "\\" + filename);
		if (!input.empty())
			graphs.push_back(input);
		cin.ignore(cin.rdbuf()->in_avail());
		cin.clear();
		fflush(stdin);
	}
	int choise;
	cout << "1 - Prim algo, 2 - Cruscal algo, 3 - comparison : " << endl;
	do {
		cin >> choise;
	} while (choise < 1 && choise > 3);

	if (choise == 1)
	{
		vector<SpanTree> SpanningTrees = PrimAlgo(graphs);
		for (auto it : SpanningTrees)
		{
			if (!it.TreeData.isConnected())
				cout << "WARNING! IS NOT CONNECTED!" << endl;
			it.print();
			cout << endl << endl << endl;
		}
	}
	else if (choise == 2)
	{
		vector<SpanTree> SpanningTrees = CruscalAlgo(graphs);
		for (auto it : SpanningTrees)
				it.print();
		cout << endl << endl << endl;
	}
	else {
		CompareAlgos(graphs);
	}
	

}

void MinSpanTree::SpanTree::print()
{
	if (!TreeData.empty())
		TreeData.print();
	cout << endl;
	cout << "  TOTAL WEIGHT IS  " << totalWeight << endl;;
}
