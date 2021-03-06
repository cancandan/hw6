//============================================================================
// Name        : graph_class.cpp
// Author      : Can Candan
// Version     :
// Copyright   :
// Description : HW6
//============================================================================

#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>

using namespace std;

void PrintList(vector< list<int> >& lst) {
	  for (int i=0; i<lst.size(); i++) {
		  cout << i << ": ";
		  for (list<int>::iterator ip=lst[i].begin(); ip!=lst[i].end(); ip++) {
			  cout << *ip << " ";
		  }
		  cout << endl;
	  }
}

class Graph {
public:
	int numNodes;
	vector< list<int> > outlist;
	vector< list<int> >  inlist;
	Graph(int N) {
		numNodes = N;
		outlist.resize(N);
		inlist.resize(N);
	}

	Graph(string s) {
//		ReadFromFile(fn);
		ReadFromString(s);
	}

	void Print() {
		cout << "Outlist:" << endl;
		PrintList(outlist);
		cout << "Inlist:" << endl;
		PrintList(inlist);
	}

	int InDegree(int vertex) {
		return inlist[vertex].size();
	}

	int OutDegree(int vertex) {
		return outlist[vertex].size();
	}

	bool isNeighborDownStream(int neigh, int source) {
		for (list<int>::iterator ip=outlist[source].begin(); ip!=outlist[source].end(); ip++) {
			if (*ip==neigh) return true;
		}
		return false;
	}

	bool isNeighborUpStream(int neigh, int source) {
		for (list<int>::iterator ip=inlist[source].begin(); ip!=inlist[source].end(); ip++) {
			if (*ip==neigh) return true;
		}
		return false;
	}

	bool isReachable(int target, int source) {
		bool *visited = new bool[numNodes];
		return dfs(target,source,visited);
//		return bfs(target,source,visited);
	}

	bool bfs(int target, int source, bool visited[]) {
		list<int> queue;
		queue.push_back(source);
		while(!queue.empty())
		{
			int s = queue.front();
			queue.pop_front();
			for(list<int>::iterator it = outlist[s].begin(); it != outlist[s].end(); ++it)
			{
				if (*it==target) return true;
				if(!visited[*it])
				{
					visited[*it] = true;
					queue.push_back(*it);
				}
			}
		}
		return false;
	}

	bool dfs(int target, int source, bool visited[]) {
		visited[source] = true;
		for (list<int>::iterator ip=outlist[source].begin(); ip!=outlist[source].end(); ip++) {
			if (*ip==target) return true;
			else if (!visited[*ip]) {
				if (dfs(target, *ip, visited)) return true;
			}
		}
		return false;
	}

	void addEdge(int s, int t) {
		outlist[s].push_back(t);
		inlist[t].push_back(s);
	}

	void ReadFromStream(istream& st) {
		int s, t;
		st >> numNodes;
		inlist.resize(numNodes);
		outlist.resize(numNodes);

		st >> s >> t;
		while (st.good()) {
			outlist[s].push_back(t);
			inlist[t].push_back(s);
			st >> s >> t;
		}
	}

	void ReadFromString(string s) {
		istringstream iss (s);
		ReadFromStream(iss);
        
	}
    
	void ReadFromFile(string fn) {
		ifstream ifs;
		ifs.open(fn, std::ifstream::in);
		ReadFromStream(ifs);
		ifs.close();
	}
    
	void createDotFile() {
		string s=prepareDotFile();
		writeDotFile(s);
	}

    string prepareDotFile() {
    	string s= "digraph {\n";
		for (int i=0; i<numNodes; i++) {
			for (list<int>::iterator ip=outlist[i].begin(); ip!=outlist[i].end(); ip++) {
				s=s+to_string(i)+" -> "+to_string(*ip)+"\n";
			}
		}
        s=s+ "}";
        return s;
    }

    void writeDotFile(string s) {
    	ofstream myfile;
    	myfile.open ("graph.dot");
    	myfile << s;
    	myfile.close();
    }


};

void createPng() {
	system("/usr/local/bin/dot -Tpng -ograph.png graph.dot");
}

int main () {
//	Graph g("/Users/ody/Documents/swe501ws/test.txt");

	// Read from string instead of file for quick testing
//	string s=
//	"4\n"
//	"0 1\n"
//	"0 3\n"
//	"1 2\n"
//	"2 0\n";


	Graph g(4);
	g.addEdge(0,1); g.addEdge(1,2); g.addEdge(2,0); g.addEdge(0,3);
	g.Print();



//	Graph g(s);
//	g.Print();

	// create the "dot" file (the graphviz representation) and png files inside working directory
	// install graphviz from http://www.graphviz.org/
	g.createDotFile();
	createPng();

	// test isReachable
//	cout << g.isReachable(3,0);

	for (int i=0; i<g.numNodes; i++) {
		for (int j=0; j<g.numNodes; j++) {
			cout << "g.isReachable(target=" << i << ",source=" << j << ")=" << g.isReachable(i, j) << endl;
		}
	}

	return 0;
}
