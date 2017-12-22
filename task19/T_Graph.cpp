#include "T_Graph.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
//#include <Shlwapi.h>
#include <list>
#include <set>
#include <windows.h>
#include <iterator>

using namespace std;
using namespace Graph;

void				T_Graph::ErrorMessage(string _error)
{
	cout << "Error : " << _error << ". " << endl;
}

int T_Graph::getStart()
{
	return start;
}

void T_Graph::setStart(unsigned int _start)
{
	if (_start < graph.size())
		start = _start;
	else ErrorMessage("_start is out of range");
}

int T_Graph::getEnd()
{
	return end;
}

void T_Graph::setEnd(unsigned int _end)
{
	if (_end < graph.size())
		end = _end;
	else ErrorMessage("_end is out of range");
}

void				T_Graph::fimport(string _filename)
{
	ifstream fin(_filename, ios::in);

	if (!fin.is_open())
	{
		ErrorMessage("Couldn't open file.");
		return;
	}

	/*int sz;
	fin.read((char*)sz, sizeof(int));

	vector<set<int>> gr;

	fin.read((char*)gr, sz);*/

	char buff[20];
	int i = 0, to;
	start = i;
	while (!fin.eof())
	{
		fin >> buff;
		if (isdigit(buff[0]))
		{
			to = atoi(buff);
			if (graph.size() <= to)
				graph.resize(to + 1);

			graph[i].insert({ to });
		}
		else {
			++i;
			if (graph.size() < i)
				graph.resize(i);
		}
	}
	end = i - 1;
	fin.close();
}

int T_Graph::vertexCount()
{
	return graph.size();
}

vector<set<edge>> T_Graph::to_vector()
{
	return graph;
}

void				T_Graph::print()
{
	for (int i = 0; i < vertexCount(); ++i)
	{
		cout << "Vertex " << i << ": ";
		for (auto j : graph[i])
			cout << "{ " << i << " -> " << j.to << " w = " << j.weight << " } ";

		cout << endl;
	}
	cout << endl;
}

					T_Graph::T_Graph(string filename)
{
	fimport(filename);
}

void				T_Graph::fexport(string _filename)
{
	ofstream fout(_filename, ios::binary);

	if (!fout.is_open())
		ErrorMessage("Could't open file");

	//ostream_iterator<edge> iter(fout, "\n");




	for (int i = 0; i<graph.size(); ++i)
	{
		for (auto j = graph[i].begin(); j != graph[i].end(); ++j)
		{
			fout << j->to;
			fout << " ";
			fout << j->weight;
			fout << " ";
		}
		fout << "|";
	}
	fout.close();
}

vector<T_Graph> Graph::readAllFilesFromCD()
{
	vector<T_Graph> graphs;
	string cd = getcd();
	WIN32_FIND_DATA findfiledata;
	HANDLE hf;

	int i = 0;
	hf = FindFirstFile((getcd() + "\\*.txt*").c_str(), &findfiledata);
	graphs[i] = T_Graph(findfiledata.cFileName);
	return graphs;
}

bool Graph::operator<(edge x, edge y)
{
	if (x.weight != y.weight)
		return x.weight < y.weight;
	else return x.to < y.to;
}

bool Graph::operator<(const edge_data & x, const edge_data & y)
{
	if (x.weight != y.weight)
		return x.weight < y.weight;
	else if (x.to != y.to)
		return x.to < y.to;
	else return x.from < y.from;
}

string Graph::getcd()
{
	wchar_t *w = nullptr;
	wstring ws = _wgetcwd(w, _MAX_PATH);
	return string(ws.begin(), ws.end());
}

string Graph::getExtension(string _path)
{
	string ext;
	string::iterator cur = _path.end();
	while (*cur != '.')
	{
		ext = *cur + ext;
		--cur;
	}
	return ext;
}

bool Graph::T_Graph::isUndirected()
{
	for (int i = 0; i < vertexCount(); ++i)
		for (auto j : graph[i])
			if (graph[j.to].find({ j.to, j.weight }) == graph[j.to].end())
				return false;
	return true;
}

bool Graph::T_Graph::empty()
{
	return graph.empty();
}

vector<set<edge>>	Graph::T_Graph::transpose()
{
	int size = graph.size();
	vector<set<edge>> transp(size);

	for (int i = 0; i < size; ++i)
	{
		for (auto j = graph[i].begin(); j != graph[i].end(); ++j)
			transp[j->to].insert({ i, j->weight });
	}
	return transp;
};

bool Graph::T_Graph::isConnected()
{
	vector<bool> vizited;

	for (int i = 0; i < graph.size(); ++i)
		vizited.push_back(0);

	checkConnectivity(vizited, start);


	//vector<set<edge>> transp = transpose();

	//checkConnectivity()

	for (auto j : vizited)
		if (!vizited[j])
			return false;
	return true;
}

void Graph::T_Graph::checkConnectivity(vector<bool> &vizited, int vertex)
{
	vizited[vertex] = true;
	for (auto it : graph[vertex])
		if (!vizited[it.to])
			checkConnectivity(vizited, it.to);
}

void Graph::GraphCompare(T_Graph _gr1, T_Graph _gr2)
{
	vector<set<edge>> A = _gr1.to_vector();
	vector<set<edge>> B = _gr2.to_vector();
	for (int i = 0; i < A.size(); ++i)
	{
		for (auto j : A[i])
		{
			if (B[i].find(j) == B[i].end() && B[j.to].find({ i, j.weight }) == B[j.to].end())
				cout << "A : { " << i << " - " << j.to << " w = " << j.weight << "} " << endl;
		}
	}

	for (int i = 0; i < B.size(); ++i)
	{
		for (auto j : B[i])
		{
			if (A[i].find(j) == A[i].end() && A[j.to].find({ i, j.weight }) == A[j.to].end())
				cout << "B : { " << i << " - " << j.to << " w = " << j.weight << "} " << endl;
		}
	}

}
