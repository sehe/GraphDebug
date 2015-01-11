//
//  Edge.cpp
//  Graph
//
//  Created by Jordan Rodrigues Rangel on 12/18/14.
//
//

#include "Edge.h"

double	Edge::GetCost() { return m_dCost; }

int		Edge::GetIndexTo() { return m_nIndexTo; }

Edge::~Edge()
{
#ifdef DEBUG_
	std::cout << "edge destroyed" << std::endl;
#endif
}
