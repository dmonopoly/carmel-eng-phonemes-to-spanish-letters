#ifndef NODE_H_
#define NODE_H_

#include <iostream>
#include <vector>

#include "Edge.h"

using namespace std;

struct Edge;

struct Node {
  string name;  // Useful name accessible in repr(), used as key in maps.
  string word, tag; // The observed word and the corresponding tag.
  int index;  // Numeric that can be used as topological ordering index.
              // Does not have to be used if nodes are consistently pushed back
              // in vectors.
  vector<Edge *> parent_edges;
  vector<Edge *> child_edges;
  Node(string name, int index) {
    this->name = name;
    this->index = index;
  }
  Node(string name, int index, string tag, string word) {
    this->name = name;
    this->index = index;
    this->tag = tag;
    this->word = word;
  }
  string repr() {
    return this->name;
  }
};

#endif
