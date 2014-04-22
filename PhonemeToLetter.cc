#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "CypherReader.h"

using namespace std;

const string FST_FILE = "2_phoneme_to_letter.fst";
const string EMPTY = "*e*";

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
    cerr << "Usage: ./<exec> <cipher-file>" << endl;
    return 0;
  }
  vector<string> inputs;
  inputs.push_back("S");
  inputs.push_back("N");
  inputs.push_back("V");
  string filename_for_cypher = argv[1];
  // Get all English phonemes.
  set<string> eng_phonemes;
  {
    vector<string> observed_data;
    bool got_obs_data = CypherReader::GetObservedData(filename_for_cypher,
                                                      &observed_data,
                                                      &eng_phonemes);
    if (!got_obs_data) {
      cerr << "Error getting English phonemes." << endl;
      return 1;
    }
  }
  // Get all Spanish letters.
  set<string> letters;
  {
    vector<string> observed_data;
    bool got_obs_data = CypherReader::GetObservedData(filename_for_cypher,
                                                      &observed_data,
                                                      &letters);
    if (!got_obs_data) {
      cerr << "Error getting Spanish letters." << endl;
      return 1;
    }
  }
  // Begin writing the FST.
  ofstream fout;
  fout.open(FST_FILE.c_str());
  string only_node = "0";
  fout << only_node << endl;
  // Remove the _ that was read in!
  letters.erase("_");
  WriteLine(fout, only_node, only_node, "_", "_");
  set<string>::iterator it;
  for (it = eng_phonemes.begin(); it != eng_phonemes.end(); ++it) {
    string phon = eng_phonemes[i];
    set<string>::iterator it2;
    for (it2 = letters.begin(); it2 != letters.end(); ++it2) {
      string letter = *it2;
      WriteLine(fout, only_node, only_node, phon, letter);
    }
  }
  fout.close();
  cout << "Written to " << FST_FILE << endl;
  return 0;
}

