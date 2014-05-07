// Reads count data and determines P(x), P(x|y), P(y|x), etc. for all tags x,y.
// This version does NOT have smoothing, which is important for sparse WFSAs so
// that 0 probabilities are left out.

#ifndef TAG_GRAMMAR_FINDER_SPARSE_H_
#define TAG_GRAMMAR_FINDER_SPARSE_H_

#include <stdexcept>
#include <cmath>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>

#include "EMViterbiPackage/Notation.h"

using namespace std;

namespace TagGrammarFinderSparse {
  const string GIVEN_DELIM = "|";
  const string AND_DELIM = ",";
  const string SEQ_DELIM = " ";
  const string SIGMA = "SUM_i ";
  const string ARB_SOUND_PLACEHOLDER = "s_i";

  // Returns true if success.
  // Accepts file of following format:
  // 47344 a _
  // 39946 s _
  // 39429 e _
  // 36767 o _
  // 27626 _ e
  bool GetBigramTagGrammarFromOrganizedRows(const string &filename,
                                            map<Notation, double> *data,
                                            vector<string> *tag_list);
  bool GetTrigramTagGrammarFromOrganizedRows(const string &filename,
                                             map<Notation, double> *data,
                                             vector<string> *tag_list);
}
#endif // End TAG_GRAMMAR_FINDER_SPARSE_H_
