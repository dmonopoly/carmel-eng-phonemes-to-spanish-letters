#ifndef CYPHER_READER_H_
#define CYPHER_READER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

namespace CypherReader {
  // Obtains observed data from a file of contents with the following format:
  // I N G C M P N Q S N W F _ C V _ F P N _ O W O K T V C V _ H U _ I H G Z S N
  // W F V _ R Q C F F N W _ C W
  // _ is a space.
  // Returns true if success. Also stores a set of unique observed cypher
  // symbols.
  bool GetObservedData(const string &filename, vector<string> *observed_data,
                       set<string> *obs_symbols);
}

#endif  // CYPHER_READER_H_
