//
//  Graph.h
//  Graph
//
//  Created by Jordan Rodrigues Rangel on 12/18/14.
//
//

/*
	The Graph doesn't keep track if you gave the same id (string)
	to more than one vertex or not.
*/

#ifndef __Graph__Graph__
#define __Graph__Graph__

#include <vector>
#include <algorithm>
#include "Vertex.h"
#include <string>

template <typename T>
class Graph
{
private:
	std::vector< Vertex<T>* > m_vVertexes;
	int m_nIndexGenerator;

	Graph(const Graph& g);
	Graph& operator=(const Graph& g);
	void Init();
	static bool Compare(std::pair< Vertex<T>*, double > pFrom, std::pair< Vertex<T>*, double > pTo);
public:
	Graph(int nVertexesAmount = 10);

	// Return an unique index
	int 	CreateVertex(const T& tInfo, std::string id = "");

	// Creates an undirected edge and 2 vertexes with the ids (strings) given by the user

	Graph&	AddUndirEdgeAndVtxs(const T& tFrom, const T& tTo, std::string sFromVertex = "",
						 std::string sToVertex = "", double dCost = 1.0);

	// Creates an undirected edge from the 2 unique id's from 2 already created vertexes
	Graph&	AddUndirEdge(int nFromVertex, int nToVertex, double dCost = 1.0);

	// Creates 2 edges and 2 vertexes with the ids (strings) given by the user
	Graph& 	AddDirEdgesAndVtxs(const T& t1, const T& t2, std::string sVertex1 = "",
						 std::string sVertex2 = "", double dCost1to2 = 1.0, double dCost2to1 = 1.0);

	// Creates 2 edges from the 2 unique id's from 2 already created vertexes
	Graph&	AddDirEdges(int nVertex1, int nVertex2, double dCost1to2 = 1.0, double dCost2to1 = 1.0);

	bool A_Star(int source, int destiny, double (* Heuristic)(const T& tFrom, const T& tTo), const bool testIfItsOnOpenSet);
	~Graph();
};


/* Implementation */

template <typename T>
Graph<T>::Graph(int nVertexesAmount) : m_nIndexGenerator(0)
{
	if(nVertexesAmount > 0)
		m_vVertexes.reserve(nVertexesAmount);
}

template <typename T>
int	Graph<T>::CreateVertex(const T& tInfo, std::string id)
{
	int index = m_nIndexGenerator++;

	m_vVertexes.push_back(new Vertex<T>(index, tInfo, id));

#ifdef DEBUG_
	std::cout << m_vVertexes.size() << std::endl;
#endif

	return index;
}

template <typename T>
Graph<T>&	Graph<T>::AddUndirEdge(int nFromVertex, int nToVertex, double dCost)
{
	m_vVertexes[nFromVertex]->AddEdgeTo(nToVertex, dCost);
	return *this;
}

template <typename T>
Graph<T>&	Graph<T>::AddUndirEdgeAndVtxs(const T& tFrom, const T& tTo, std::string sFromVertex,
						 	  std::string sToVertex, double dCost)
{
	int indexFrom = CreateVertex(tFrom, sFromVertex);
	int indexTo = CreateVertex(tTo, sToVertex);

	return AddUndirEdge(indexFrom, indexTo, dCost);
}

template <typename T>
Graph<T>&	Graph<T>::AddDirEdges(int nVertex1, int nVertex2, double dCost1to2, double dCost2to1)
{
	AddUndirEdge(nVertex1, nVertex2, dCost1to2);
	return AddUndirEdge(nVertex2, nVertex1, dCost2to1);
}

template <typename T>
Graph<T>& 	Graph<T>::AddDirEdgesAndVtxs(const T& t1, const T& t2, std::string sVertex1,
						 	 std::string sVertex2, double dCost1to2, double dCost2to1)
{
	int index1 = CreateVertex(t1, sVertex1);
	int index2 = CreateVertex(t2, sVertex2);

	return AddDirEdges(index1, index2, dCost1to2, dCost2to1);
}

template <typename T>
void Graph<T>::Init()
{
	int vSize = m_vVertexes.size();

	for(int i = 0; i < vSize; i++)
	{
		m_vVertexes[i]->SetCost(std::numeric_limits<double>::max()).SetPrevious(-1).SetColor(White);
	}
}

template <typename T>
/*static*/ inline bool Graph<T>::Compare(std::pair< Vertex<T>*, double > pFrom, std::pair< Vertex<T>*, double > pTo)
{
	return pFrom.second >= pTo.second;
}

template <typename T>
bool Graph<T>::A_Star(int source, int destiny, double (* Heuristic)(const T& tFrom, const T& tTo), const bool testIfItsOnOpenSet)
{
	int vSize = m_vVertexes.size();
	bool found = false;

	// Checking if it is not out of the bounds
	if(source < 0 || destiny < 0 || source >= vSize || destiny >= vSize)
		return false;

	Vertex<T>* vDestiny = m_vVertexes[destiny];
	T& tDestinyInfo = vDestiny->GetInfo();

	// Initializing the vectors to proper values
	Init();
	m_vVertexes[source]->SetCost(0.0);
	std::pair< Vertex<T>*, double > pTempFrom;

	pTempFrom.second = Heuristic(m_vVertexes[source]->GetInfo(), tDestinyInfo);
	pTempFrom.first = m_vVertexes[source];

	// Initializing the minHeap
	std::vector< std::pair< Vertex<T>*, double > > minHeap;
	minHeap.reserve(vSize);

	std::make_heap(minHeap.begin(), minHeap.end(), Compare);
	minHeap.push_back(pTempFrom);
	std::push_heap(minHeap.begin(), minHeap.end(), Compare);

	while(!minHeap.empty())
	{
		std::pop_heap(minHeap.begin(), minHeap.end(), Compare);
		pTempFrom = minHeap.back();
		minHeap.pop_back();

		Vertex<T>* vTempFrom = pTempFrom.first;
		vTempFrom->SetColor(Black);

		double dCostFrom = vTempFrom->GetCost();

		if(vTempFrom == vDestiny)
		{
			found = true;
			break;
		}

		int fromIndex = vTempFrom->GetIndex();

		for(int i = vTempFrom->GetNextAdjacentVertex(); i > -1; i = vTempFrom->GetNextAdjacentVertex())
		{
			Vertex<T>* vTempTo = m_vVertexes[i];

			double dCost = dCostFrom + vTempFrom->GetEdgeCost(vTempTo->GetIndex);

			if(testIfItsOnOpenSet && vTempTo->GetColor() == Gray && dCost < vTempTo->GetCost())
			{
				vTempTo->SetColor(White);
				std::vector< Vertex<T>*, double >::iterator it = std::find(minHeap.begin(), minHeap.end(), vTempTo);
				minHeap.erase(it);
				std::make_heap(minHeap.begin(), minHeap.end(), Compare);
			}

			if(vTempTo->GetColor() == White)
			{
				vTempTo->SetCost(dCost);
				vTempTo->SetPrevious(fromIndex);
				std::pair< Vertex<T>*, double > pTempTo;

				pTempTo.first = vTempTo;
				pTempTo.second = dCost + Heuristic(vTempTo->GetInfo(), tDestinyInfo);
				minHeap.push_back(pTempTo);
				std::push_heap(minHeap.begin(), minHeap.end(), Compare);
			}
		}
	}

	minHeap.clear();

	return found;
}

template <typename T>
Graph<T>::~Graph()
{
	int vSize = m_vVertexes.size();

	for(int i = 0; i < vSize; i++)
		delete m_vVertexes[i];

#ifdef DEBUG_
	std::cout << "graph destroyed" << std::endl;
#endif
}

#endif /* defined(__Graph__Graph__) */
