// Generates a sparse WFSA based on the given bigram counts file and the FST
// using the given cypher and the counts file.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>

#include "EMViterbiPackage/Notation.h"
#include "TagGrammarFinder.h"
#include "CypherReader.h"

#define PRINT_PROBS true

using namespace std;

const string WFSA_FILE = "1_eng_phoneme_trigram.wfsa";
const string EMPTY = "*e*";

void WriteLine(ofstream &fout, const string &node1, const string &node2,
    const string &first, const string &second, double prob, const string &end) {
  string input = first; // x in x:y
  string output = second; // y in x:y
  if (first != EMPTY) {
    stringstream ss;
    ss << "\"" << first << "\"";
    input = ss.str();
  }
  if (second != EMPTY) {
    stringstream ss;
    ss << "\"" << second << "\"";
    output = ss.str();
  }
  fout << "(" << node1 << " (" << node2 << " " << input << " " << output << " " 
    << prob << end << "))" << endl;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: ./<exec> <bigram-counts-file>" << endl;
    return 0;
  }
  string filename_for_bigrams = argv[1];
  // Get LM data for WFSA.
  map<Notation, double> data;  // Storage for log probabilities and counts.
  vector<string> tag_list;
  bool found = TagGrammarFinder::GetBigramTagGrammarFromOrganizedRows(
      filename_for_bigrams, &data, &tag_list);
  if (!found) {
    cerr << "Error getting tag grammar." << endl;
    return 0;
  }

  if (PRINT_PROBS) {
    ofstream fout;
    fout.open("found_bigram_probs.txt");
    for (map<Notation, double>::iterator it = data.begin(); it != data.end(); ++it) {
      fout << it->first << ": " << it->second << endl;
    }
    fout.close();
  }

  // Begin writing out the WFSA.
  ofstream fout;
  fout.open(WFSA_FILE.c_str());
  fout << "END" << endl;
  double prob_to_end = .0001;
  double lambda = .9;
  // Unigram probs.
  for (int i = 0; i < tag_list.size(); ++i) {
    string s = tag_list[i];
    string node_name = s;
    string node_name_sharp = node_name + "#";
    vector<string> vec; vec.push_back(s);
    Notation n("P", vec);
    try {
      double prob = data.at(n);
      // Unigram prob.
      WriteLine(fout, "START", node_name, EMPTY, s, prob, "!");
      // Back to Start.
      WriteLine(fout, node_name, "START", EMPTY, EMPTY, 1 - lambda, "!");
      // Commit to bigram.
      WriteLine(fout, node_name, node_name_sharp, EMPTY, EMPTY, lambda, "!");
      // To End.
      WriteLine(fout, node_name, "END", EMPTY, EMPTY, prob_to_end, "!");
    } catch (out_of_range &e) {
      cerr << "Out of range error for notation " << n << "; " << e.what() <<
        endl;
      exit(0);
    }
  }
  // Bigram probs.
  for (int i = 0; i < tag_list.size(); ++i) {
    string s1 = tag_list[i];
    for (int j = 0; j < tag_list.size(); ++j) {
      string s2 = tag_list[j];
      vector<string> vec1; vec1.push_back(s1);
      vector<string> vec2; vec2.push_back(s2);
      Notation n("P", vec2, TagGrammarFinder::GIVEN_DELIM, vec1);
      string node1_name_sharp = s1 + "#";
      string node2_name = s2;
      try {
        double prob = data.at(n);
        // Bigram prob - node sharp to node.
        WriteLine(fout, node1_name_sharp, node2_name, EMPTY, s2, prob, "!");
      } catch (out_of_range &e) {
        cerr << "Out of range error for notation " << n << "; " << e.what() <<
          endl;
        exit(0);
      }
    }
  }
  fout.close();
  cout << "Written to " << WFSA_FILE << endl;
  return 0;
}
