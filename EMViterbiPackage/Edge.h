#ifndef EDGE_H_
#define EDGE_H_

#include <iostream>
#include <vector>

#include "Node.h"
#include "Notation.h"
#include "GraphAid.h"

using namespace std;

struct Node;

struct Edge {
  Notation notation;
  Node *src, *dest;
  Edge(Notation n, Node *src, Node *dest);
  Notation repr() {
    return notation;
  }
};

#endif  // EDGE_H_
