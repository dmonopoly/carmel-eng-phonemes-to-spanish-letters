/*
 * Basic helper functions for strings, numbers, and printing.
 * Author: David Zhang
 */

#ifndef STANDARD_H_
#define STANDARD_H_

#include <iostream> // IO access
#include <iomanip>  // Format
#include <sstream>  // String helper
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

namespace Basic {

const int PRECISION = 4;
const int WIDTH = 10;

// Allows easy tabbing, so you can use Tab(1) and Tab(2).
string Tab(int n);

// Allows easy new lines, so you use Enter(1) and Enter(2).
void Enter(int n);

// Returns true if s1 contains s2; false otherwise.
bool Contains(string s1, string s2);

// Returns true if long num1 contains long l2.
bool Contains(long num1, long num2);

// For asking yes or no questions. Returns true if char is "y" or "Y"; false if
// "n" or "N"; otherwise, repeat question.
bool AskAndDecide(string question);

// Method that directly makes a string all lowercase.
void ToLowerCase(string &s);

// Replace all occurrences of string within string.
void ReplaceAll(string& str, const string& from, const string& to);

// Split a string about a delimiter and store in vector.
vector<string> &Split(const string &s, char delim, vector<string> &elems);
vector<string> Split(const string &s, char delim);

// Compute log(x + y) given logx and logy.
double AddLogs(double logx, double logy);

// Compute log(x - y) given logx and logy.
double SubtractLogs(double logx, double logy);

// Useful printing method for 2 columns.
template<typename A, typename B> void Print2(const A& a, const B& b) {
	cout << fixed << setprecision(2);
	cout << Tab(1) << left << setw(WIDTH) << a << right << b << endl;
}

// Useful printing method for 3 columns.
template<typename A, typename B, typename C> void Print3(const A& a, const B& b,
                                                         const C& c) {
	cout << fixed << setprecision(PRECISION);
  cout << Tab(1) << left << setw(WIDTH) << a << right << setw(WIDTH) << b <<
    setw(WIDTH) << c << endl;
}

// Useful writing method for 3 columns, like above.
template<typename A, typename B, typename C> void Write3(ofstream &fout,
                                                         const A& a, const B& b,
                                                         const C& c) {
	fout << fixed << setprecision(PRECISION);
  fout << Tab(1) << left << setw(WIDTH) << a << right << setw(WIDTH) << b <<
    setw(WIDTH) << c << endl;
}

// Useful printing method for n columns.
template<typename Container> void PrintRow(const Container &c) {
	cout << fixed << setprecision(PRECISION) << left << setw(WIDTH);
  for (typename Container::const_iterator it = c.begin(); it != c.end() - 1;
      ++it) {
      cout << *it << right << setw(WIDTH);
  }
  cout << c.back() << endl;
}

// Useful method to convert number to string.
template <typename T> string NumberToString ( T Number ) {
	stringstream ss;
	ss << Number;
	return ss.str();
}

// Useful method to convert string to number; returns specified fail variable if
// unsuccessful. Usage example: int i = StringToNumber<int>(response, -2), where
// response is a string.
template <typename T> T StringToNumber (const string &Text, T fail) {
	stringstream ss(Text);
	T result;
	return ss >> result ? result : fail;
}

// Prompts for a number with the specified question. Repeatedly asks until an
// acceptable answer is given. Usage: AskAndGetNumber<int>("What number?")
template <typename T> T AskAndGetNumber(string question) {
    string response;
    cout << question;
    getline(cin, response);
    T i = StringToNumber<T>(response, -2);
    while (i == -2) {
        cout << Tab(1) << "Improper response.\n";
        cout << question;
        getline(cin, response);
        i = StringToNumber<T>(response, -2);
    }
    return i;
}

}  // namespace Basic
#endif /* STANDARD_H_ */
