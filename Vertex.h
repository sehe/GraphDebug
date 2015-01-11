//
//  Vertex.h
//  Graph
//
//  Created by Jordan Rodrigues Rangel on 12/18/14.
//
//

#ifndef __Graph__Vertex__
#define __Graph__Vertex__

#include "Edge.h"
#include <vector>
#include <limits>
#include <string>

typedef enum
{
    // Hasn't been processed yet
    White,

    // Is being processed
    Gray,

    // Has been processed
    Black
} Color;

template <typename T>
class Vertex
{
private:
    // Unique index given by the Graph
    int m_nIndex;

    // Cost until this vertex
    double m_dCost;

    // Id given by the user, not necessarily unique
    std::string m_strId;

    // Current color
    Color m_currColor;

    // Data being held
    T m_tInfo;

    // Edges that leave from this vertex
    std::vector<Edge*> m_vEdges;

    // Amount of edges pointing to this vertex
    int m_nArrivedEdges;

	// The index of the vertex the lead to this one through an edge in a certain path
	int m_nPrevious;

	// Interator used to get the adjacent vertex's unique index
	int m_nIterator;

	Vertex(const Vertex& v);
	Vertex& operator=(const Vertex& v);

public:

	// Vertex constructor
    Vertex(int nIndex, const T& tInfo, std::string id = "") : m_nIndex(nIndex), m_strId(id), m_tInfo(tInfo), m_dCost(std::numeric_limits<double>::max()), m_currColor(White), m_nArrivedEdges(0), m_nPrevious(-1), m_nIterator(0) {}

	// Returns the unique index given by the graph
	int    GetIndex();

	// Returns the id given by the user
    std::string& GetId();

	// Returns the cost from it is from the source
    double  GetCost();

	// Returns the process status
    Color   GetColor();

	// Returns the data given by the user
    T&      GetInfo();

	// Return the next adjacent vertex's unique index. Returns -1 if there aren't any or if all were already given (then use the reset function)
	int		GetNextAdjacentVertex();

	// Reset the iterator
	Vertex& ResetIterator();

	// Sets a new cost
    Vertex& SetCost(double dCost);

	// Update the process status
    Vertex& SetColor(Color c_color);

	// Sets previous vertex
	Vertex& SetPrevious(int nIndex);

	// Creates a new edge that departs from this vertex
    Vertex& AddEdgeTo(int nIndexTo, double dCost = 1.0);

	// Returns the edge cost to a certain adjacent vertex
	double GetEdgeCost(int nIndex);

	~Vertex();
};


/* Implementation */

template <typename T>
inline std::string&	Vertex<T>::GetId() { return m_strId; }

template <typename T>
inline int	Vertex<T>::GetIndex() { return m_nIndex; }

template <typename T>
inline double  Vertex<T>::GetCost() { return m_dCost; }

template <typename T>
inline Color  Vertex<T>::GetColor() { return m_currColor; }

template <typename T>
inline T&     Vertex<T>::GetInfo() { return m_tInfo; }

template <typename T>
inline int		Vertex<T>::GetNextAdjacentVertex()
{
	int eSize = m_vEdges.size();

	if(m_nIterator >= eSize)
		return -1;

	return m_nIterator++;
}

template <typename T>
double Vertex<T>::GetEdgeCost(int nIndex)
{
	if(nIndex < 0 || nIndex >= m_vEdges.size())
		return std::numeric_limits<double>::infinity();

	return m_vEdges[nIndex]->GetCost();
}

template <typename T>
inline Vertex<T>& Vertex<T>::ResetIterator()
{
	m_nIterator = 0;

	return *this;
}

template <typename T>
inline Vertex<T>& Vertex<T>::SetCost(double dCost)
{
    m_dCost = dCost;

    return *this;
}

template <typename T>
inline Vertex<T>& Vertex<T>::SetColor(Color c_color)
{
    m_currColor = c_color;

    return *this;
}

template <typename T>
inline Vertex<T>& Vertex<T>::SetPrevious(int nIndex)
{
	m_nPrevious = nIndex;
	return *this;
}

template <typename T>
inline Vertex<T>& Vertex<T>::AddEdgeTo(int nIndexTo, double dCost)
{
	m_vEdges.push_back(new Edge(nIndexTo, dCost));

	return *this;
}

template <typename T>
Vertex<T>::~Vertex()
{
	int vSize = m_vEdges.size();

	for(int i = 0; i < vSize; i++)
		delete m_vEdges[i];

#ifdef DEBUG_
	std::cout << "vertex " << m_nIndex << " is being destroyed" << std::endl;
#endif
}

// FIXING ERRORS IN "GUESS" MODE
template <typename T>
bool operator==(std::pair<Vertex<T>*, double> const& p, Vertex<T> const* v) {
    return p.first == v;
}

#endif /* defined(__Graph__Vertex__) */
