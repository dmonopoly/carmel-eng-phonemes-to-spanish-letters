#include <stdexcept>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

#include "EMViterbiPackage/Notation.h"
#include "TagGrammarFinderSparse.h"
#include "CypherReader.h"

#define PRINT_PROBS true

using namespace std;

const string WFSA_FILE = "1_eng_phoneme_trigram.wfsa";
const string EMPTY = "*e*";
const string NODE_NAME_DELIM = "-";
const double PROB_TO_END = .00001;

void WriteLine(ofstream &fout, const string &node1, const string &node2,
    const string &first, const string &second) {
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
  fout << "(" << node1 << " (" << node2 << " " << input << " " << output << "))"
    << endl;
}

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
    cerr << "Usage: ./<exec> <trigram-counts-file>" << endl;
    return 0;
  }
  cout << "-EnglishPhonemeTrigramModel-" << endl;
  string counts_file = argv[1];

  // Get LM data for WFSA.
  map<Notation, double> data;  // Storage for log probabilities and counts.
  vector<string> tag_list;
  cout << "Getting tag grammar..." << endl;
  bool found = TagGrammarFinderSparse::GetTrigramTagGrammarFromOrganizedRows(
      counts_file, &data, &tag_list);
  if (!found) {
    cerr << "Error getting tag grammar." << endl;
    return 0;
  }

  if (PRINT_PROBS) {
    ofstream fout;
    fout.open("found_trigram_probs.txt");
    for (map<Notation, double>::iterator it = data.begin(); it != data.end(); ++it) {
      fout << it->first << ": " << it->second << endl;
    }
    fout.close();
  }

  // Begin writing out the WFSA, sparsely connected trigram version.
  cout << "Starting to write WFSA for English phonemes." << endl;
  ofstream fout;
  fout.open(WFSA_FILE.c_str());
  fout << "END" << endl;
  double lambda2 = .9;
  double prob_to_end = .0001;
  // Unigram probs: START->x
  for (int i = 0; i < tag_list.size(); ++i) {
    string s = tag_list[i];
    string node_name = s;
    string node_name_sharp = node_name + "#";
    vector<string> vec; vec.push_back(s);
    Notation n("P", vec);
    try {
      double prob = data.at(n);
      WriteLine(fout, "START", node_name, EMPTY, s, prob, "!");
      // x->START
      WriteLine(fout, node_name, "START", EMPTY, EMPTY, 1 - lambda2, "!");
      // x->x#
      WriteLine(fout, node_name, node_name_sharp, EMPTY, EMPTY, lambda2, "!");
      // x#->START
      WriteLine(fout, node_name_sharp, "START", EMPTY, EMPTY, prob_to_end, "!");
      // x->END
      WriteLine(fout, node_name, "END", EMPTY, EMPTY, prob_to_end, "!");
      // x#->END
      WriteLine(fout, node_name_sharp, "END", EMPTY, EMPTY, prob_to_end, "!");
    } catch (out_of_range &e) {
      cerr << "Out of range error for notation " << n << "; " << e.what() <<
        endl;
      exit(0);
    }
  }
  // Bigram probs.
  double lambda1 = .9;
  for (int i = 0; i < tag_list.size(); ++i) {
    string s1 = tag_list[i];
    for (int j = 0; j < tag_list.size(); ++j) {
      string s2 = tag_list[j];
      vector<string> vec2; vec2.push_back(s2);
      vector<string> vec; vec.push_back(s1);
      Notation n("P", vec2, TagGrammarFinderSparse::GIVEN_DELIM, vec);
      string node1_name = s1;
      string node1_name_sharp = s1 + "#";
      string node2_name = s1 + NODE_NAME_DELIM + s2;
      // Trigram prep things.
      string node1a_name = s1 + NODE_NAME_DELIM + s2;
      string node1a_name_sharp = s1 + NODE_NAME_DELIM + s2 + "#";
      try {
        double prob = data.at(n);
        // x#->xy
        WriteLine(fout, node1_name_sharp, node2_name, EMPTY, s2, prob, "!");

        // Trigram preparation.
        // xy->xy#
        WriteLine(fout, node1a_name, node1a_name_sharp, EMPTY, EMPTY, lambda1, "!");
        // xy->y
        WriteLine(fout, node1a_name, s2, EMPTY, EMPTY, 1 - lambda1, "!");
        // xy->END
        WriteLine(fout, node1a_name, "END", EMPTY, EMPTY, prob_to_end, "!");
        // xy#->END
        WriteLine(fout, node1a_name_sharp, "END", EMPTY, EMPTY, prob_to_end, "!");
        // xy->START
        WriteLine(fout, node1a_name, "START", EMPTY, EMPTY, prob_to_end, "!");
        // xy#->START
        WriteLine(fout, node1a_name_sharp, "START", EMPTY, EMPTY, prob_to_end, "!");
      } catch (out_of_range &e) {
        cerr << "Out of range error for notation " << n << "; " << e.what() <<
          endl;
        exit(0);
      }
    }
  }
  // Trigram probs.
  for (int i = 0; i < tag_list.size(); ++i) {
    string s1 = tag_list[i];
    for (int j = 0; j < tag_list.size(); ++j) {
      string s2 = tag_list[j];
      for (int k = 0; k < tag_list.size(); ++k) {
        string s3 = tag_list[k];
        vector<string> vec3; vec3.push_back(s3);
        vector<string> vec1and2; vec1and2.push_back(s1); vec1and2.push_back(s2);
        Notation n("P", vec3, TagGrammarFinderSparse::GIVEN_DELIM, vec1and2);
        string node1_name_sharp = s1 + NODE_NAME_DELIM + s2 + "#";
        string node2_name = s2 + NODE_NAME_DELIM + s3;
        try {
          double prob = data.at(n);
          // Trigram prob path, if prob is available.
          WriteLine(fout, node1_name_sharp, node2_name, EMPTY, s3, prob, "!");
        } catch (out_of_range &e) {
          // We expect some probabilities to be nonexistent. In these cases,
          // reduce the number of edges.
        }
      }
    }
  }
  fout.close();
  cout << "Written to " << WFSA_FILE << endl;
  return 0;
}
