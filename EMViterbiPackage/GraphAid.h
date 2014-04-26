#ifndef GRAPH_AID_H_
#define GRAPH_AID_H_

#include "Node.h"
#include "Edge.h"

using namespace std;

struct Node;
struct Edge;

namespace GraphAid {
  void LinkNodeAndEdge(Node *src_node, Edge &edge, Node *dest_node);
}

#endif
