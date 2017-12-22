#pragma once

#include <set>
#include <vector>

using namespace std;

namespace Graph
{
	struct edge
	{
		int to;
		int weight;
	};

	struct edge_data
	{
		int from;
		int to;
		int weight;
	};

	bool operator<(edge x, edge y);
	bool operator<(const edge_data &x, const edge_data &y);

	class T_Graph
	{

	private:

		int						start, end;

		vector<set<edge>>			graph;

		void					ErrorMessage(string _error);

		void					checkConnectivity(vector<bool>& vizited, int vertex);

	public:

		int						getStart();

		void					setStart(unsigned int _start);

		int						getEnd();

		void					setEnd(unsigned int _end);

		void					fexport(string _filename);

		void					fimport(string _filename);



		int vertexCount();

		vector<set<edge>>		to_vector();

		void					print();

		T_Graph(vector<set<edge>> gr, int _start, int _end)
			: graph(gr), start(_start), end(_end) {};

		T_Graph()
		{};

		T_Graph(string filename);

		bool isUndirected();

		bool empty();

		bool isConnected();

		vector<set<edge>>	Graph::T_Graph::transpose();

	};

	string getcd();

	string getExtension(string _path);

	void GraphCompare(T_Graph _gr1, T_Graph _gr2);

	vector<T_Graph> readAllFilesFromCD();
}
