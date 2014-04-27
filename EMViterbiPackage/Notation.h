// Models first|second, as in P(first|second) or C(first|second).
// Definition: a 'symbol' is a character or sound or etc. - represented by a
// string, and is an element in 'first' / 'second'.
#ifndef NOTATION_H_
#define NOTATION_H_

#include <initializer_list>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

class Notation {
 private:
  string repr_;  // Unused, but could be for small optimization.
  string GenerateRepr() const;
 public:
  static const string GIVEN_DELIM;
  static const string AND_DELIM;
  static const string SEQ_DELIM;
  static const string NULL_DELIM;
 
  // "P" or "C". Can prepend with SIGMA="SUM_i" if you want.
  // Example: 
  //     const string SIGMA = "SUM_i ";
  //     const string ARB_SOUND_PLACEHOLDER = "s_i";
  //   Then use Notation objects like
  //     Notation n_count_total(SIGMA + "C", {ARB_SOUND_PLACEHOLDER});
  string predicate;
  // Denotes the delimiter separating 'first' and 'second'.
  string delimiter;
  // Delimiters within each list. This specificity allows P(ABA|t1,t2,t3).
  string first_delimiter, second_delimiter;

  // Vectors of symbols (characters, sounds, etc.).
  vector<string> first;  // Each string can be used as a key to the map.
  vector<string> second;
  Notation();
  Notation(string predicate, initializer_list<string> first_args);
  Notation(string predicate, vector<string> first);
  Notation(string predicate, initializer_list<string> first_args, string first_delim);
  Notation(string predicate, vector<string> first, string first_delim);
  Notation(string predicate, initializer_list<string> first_args,
           string delimiter, initializer_list<string> second_args);
  Notation(string predicate, vector<string> first, string delimiter,
           vector<string> second);

  void set_first(const vector<string>& other) {
    this->first = other;
  }
  void set_second(const vector<string>& other) {
    this->second = other;
  }
  void set_predicate(const string new_pred) {
    this->predicate = new_pred;
  }
  string repr() const;

  bool is_probability() const {
    return this->predicate == "P";
  }
  bool is_count() const {
    return this->predicate == "C";
  }
  bool operator <(const Notation& rhs) const {
    // Ensure storage in map<Notation, double> data structure works. This makes
    // sure constructing Notation objects on the fly indeed replaces previous
    // Notation objects who had the same repr() value.
    return this->repr() < rhs.repr();
  }
};
ostream& operator<<(ostream& out, const Notation& n);

#endif
