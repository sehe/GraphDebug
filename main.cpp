#include "Graph.h"

static inline double Heuristic(const int& index1, const int& index2);
static inline double calc(int i1, int j1, int i2, int j2);

int main(void)
{
	Graph<int> g;

	int a = g.CreateVertex(0, "FirstVertex");
	int b = g.CreateVertex(1, "SecondVertex");

	g.AddUndirEdge(a,b);
	g.AddUndirEdgeAndVtxs(2, 3);

	g.AddDirEdgesAndVtxs(4, 5);

	int c = g.CreateVertex(6, "Hue");
	int d = g.CreateVertex(7, "K");

	g.AddDirEdges(c, d);

	int e = g.CreateVertex(8, "LoL");

	g.AddDirEdges(a, e);
	g.AddDirEdges(e, d);

	g.A_Star(a, b, Heuristic, false);

	return 0;
}

static inline double calc(int i1, int j1, int i2, int j2)
{
	i1 -= i2;
	j1 -= j2;

	if(i1 <0 ) i1 = -i1;
	if(j1 <0 ) j1 = -j1;

	return (double) i1 + j1;
}

static inline double Heuristic(const int& index1, const int& index2)
{
	int cols = 3;
	return calc(index1/cols, index1%cols, index2/cols, index2%cols);
}