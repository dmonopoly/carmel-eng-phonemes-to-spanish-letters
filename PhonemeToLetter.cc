#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

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
  cout << "-PhonemeToLetter-" << endl;
  vector<string> inputs;
  inputs.push_back("S");
  inputs.push_back("N");
  inputs.push_back("V");
  string filename_for_cypher = argv[1];
  // Get all English phonemes.
  cout << "Getting English phonemes..." << endl;
  set<string> eng_phonemes;
  eng_phonemes.insert("A");
  eng_phonemes.insert("AE");
  eng_phonemes.insert("AH");
  eng_phonemes.insert("AO");
  eng_phonemes.insert("AW");
  eng_phonemes.insert("AY");
  eng_phonemes.insert("B");
  eng_phonemes.insert("CH");
  eng_phonemes.insert("D");
  eng_phonemes.insert("DH");
  eng_phonemes.insert("EH");
  eng_phonemes.insert("ER");
  eng_phonemes.insert("EY");
  eng_phonemes.insert("F");
  eng_phonemes.insert("G");
  eng_phonemes.insert("HH");
  eng_phonemes.insert("IH");
  eng_phonemes.insert("IY");
  eng_phonemes.insert("JH");
  eng_phonemes.insert("K");
  eng_phonemes.insert("L");
  eng_phonemes.insert("M");
  eng_phonemes.insert("N");
  eng_phonemes.insert("NG");
  eng_phonemes.insert("OW");
  eng_phonemes.insert("OY");
  eng_phonemes.insert("P");
  eng_phonemes.insert("R");
  eng_phonemes.insert("S");
  eng_phonemes.insert("SH");
  eng_phonemes.insert("T");
  eng_phonemes.insert("TH");
  eng_phonemes.insert("UH");
  eng_phonemes.insert("UW");
  eng_phonemes.insert("V");
  eng_phonemes.insert("W");
  eng_phonemes.insert("Y");
  eng_phonemes.insert("Z");
  eng_phonemes.insert("ZH");
  // Get all Spanish letters.
  cout << "Getting Spanish letters..." << endl;
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
  cout << "Starting to write Phoneme to Letter FST." << endl;
  ofstream fout;
  fout.open(FST_FILE.c_str());
  string only_node = "0";
  fout << only_node << endl;
  // Remove the _ that was read in!
  letters.erase("_");
  eng_phonemes.erase("_");
  WriteLine(fout, only_node, only_node, "_", "_");
  set<string>::iterator it;
  for (it = eng_phonemes.begin(); it != eng_phonemes.end(); ++it) {
    string phon = *it;
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

