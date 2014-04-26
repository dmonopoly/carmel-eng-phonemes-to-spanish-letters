#include "GraphAid.h"

namespace GraphAid {
  void LinkNodeAndEdge(Node *src_node, Edge &edge, Node *dest_node) {
    // Sets the nodes to be connected to the edge.
    src_node->child_edges.push_back(&edge);
    dest_node->parent_edges.push_back(&edge);
  }
}


