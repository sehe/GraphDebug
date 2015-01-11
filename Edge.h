//
//  Edge.h
//  Graph
//
//  Created by Jordan Rodrigues Rangel on 12/18/14.
//
//

#ifndef __Graph__Edge__
#define __Graph__Edge__
#define DEBUG_

#include <iostream>

class Edge
{
private:
    int     m_nIndexTo;
    double  m_dCost;

	Edge(const Edge& e);
	Edge& operator=(const Edge& e);

public:
    Edge(int nIndexTo, double dCost = 1.0) : m_nIndexTo(nIndexTo), m_dCost(dCost) {}

    double  GetCost();
    int    GetIndexTo();
	~Edge();
};

#endif /* defined(__Graph__Edge__) */
