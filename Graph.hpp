#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstddef> // size_t
#include <vector>
#include <stack>
#include <queue>
#include <functional>

class Graph
{
public:
	Graph();
	Graph(size_t size);
	~Graph();

	void AddEdge(int v, int w);

	void BFS(int start, const std::function<void(int v)>& fn);
	void DFS(int start, const std::function<void(int v)>& fn);

private:
	std::vector<int>* mAdj;
	size_t mSize;
};

/////////////////////

Graph::Graph()
	: mAdj(nullptr), mSize(0)
{}

Graph::Graph(size_t size)
	: mSize(size)
{
	mAdj = new std::vector<int>[size];
}

Graph::~Graph()
{
	for (size_t i = 0; i < mAdj[i].size(); ++i)
		mAdj[i].clear();

	delete[] mAdj;
}

void Graph::AddEdge(int v, int w)
{
	mAdj[v].push_back(w);
}

void Graph::BFS(int start, const std::function<void(int v)>& fn)
{
	// Iterative
	std::vector<bool> visited(mSize, false);

	std::queue<int> Queue;

	Queue.push(start);
	visited[start] = true;

	while (Queue.empty() == false)
	{
		int v = Queue.front();
		Queue.pop();

		if (fn)
			fn(v);

		for (int& w : mAdj[v])
		{
			if (visited[w] == false)
			{
				Queue.push(w);
				visited[w] = true;
			}
		}
	}
}

void Graph::DFS(int start, const std::function<void(int v)>& fn)
{
	// Recursive
	std::vector<bool> visited(mSize, false);

	auto DFS_R = [&visited, &fn](int v) -> void
	{
		visited[v] = true;
		if (fn)
			fn(v);

		for (int& w : mAdj[v])
		{
			if (visited[w] == false)
				DFS_R(w);
		}
	}(start);
}

#endif /* GRAPH_HPP */