#include "CypherReader.h"

// Extra option: If true, accepts cyphers of the following format:
// "_" "p" "r" "i" "m" "e" "r" "a" "_" "p" "a" "r" "t" "e" "_" "d" "e" "l" 
// "_" "c" "a" "p" "I" "t" "u" "l" "o" "_" "p" "r" "i" "m" "e" "r" "o" "_" 
// Otherwise, same thing, but with no quotes.
#define FORMAT_HAS_QUOTES true

bool CypherReader::GetObservedData(const string &filename, vector<string> *observed_data,
                     set<string> *obs_symbols) {
  ifstream fin(filename.c_str());
  if (fin.fail()) {
    cerr << "Could not open file " << filename << endl;
    return false;
  } else {
    while (true) {
      string word;
      fin >> word;
      if (!word.empty()) {
        if (FORMAT_HAS_QUOTES)
          word = word.substr(1, word.size() - 2);  // Remove quotes.
        observed_data->push_back(word);
        obs_symbols->insert(word);
      }
      if (fin.eof())
        break;
    }
    return true;
  }
}

// int main(int argc, char *argv[]) {
//   if (argc < 2) {
//     cerr << "No filename given." << endl;
//     return 0;
//   }
//   string filename = argv[1];
//   vector<string> observed_data;
//   set<string> obs_symbols;
//   CypherReader::GetObservedData(filename, &observed_data, &obs_symbols);
//   for (auto it = observed_data.begin(); it != observed_data.end(); ++it) {
//     cout << *it << " ";
//   }
//   return 0;
// }
