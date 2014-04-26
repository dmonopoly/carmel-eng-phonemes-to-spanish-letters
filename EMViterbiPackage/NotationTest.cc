#include <assert.h>

// TODO: Could clean up and use constants.
// Includes various libraries as well and sets namespace to std.
#include "Notation.h"

const string X = "X";
const string Y = "Y";
const string A = "A";
const string B = "B";

void TestXGivenY() {
  Notation pXGivenY("P", {X}, Notation::GIVEN_DELIM, {Y});
  stringstream ss;  // We simulate 'cout <<' with this.
  ss << pXGivenY;
  cout << ss.str() << endl;
  assert(ss.str() == "P(" + X + Notation::GIVEN_DELIM + Y + ")");
  ss.clear();
}

void TestJustX() {
  Notation pX("P", {X});
  stringstream ss;  // We simulate 'cout <<' with this.
  ss << pX;
  cout << ss.str() << endl;
  assert(ss.str() == "P("+X+")" && "Nonexistent second vector didn't work.");
}

void TestChainX() {
  Notation pABA("P", {A,B,A}, Notation::SEQ_DELIM);
  stringstream ss;
  ss << pABA;
  cout << ss.str() << endl;
  assert(ss.str() == "P(ABA)");

  ss.clear();
  ss.str("");

  Notation pABA2("P", {A,B,A}, Notation::AND_DELIM);
  ss << pABA2;
  cout << ss.str() << endl;
  assert(ss.str() == "P(A,B,A)");

  ss.clear();
  ss.str("");

  Notation pAGivenX("P", {A}, Notation::GIVEN_DELIM, {X});
  ss << pAGivenX;
  cout << ss.str() << endl;
  assert(ss.str() == "P(A|X)");
  assert(pAGivenX.repr() == "P(A|X)");
}

void TestNotation() {
  cout << "Testing Notation..." << endl;
  TestXGivenY();
  TestJustX();
  TestChainX();
  cout << "Test(s) passed." << endl;
}

int main() {
  TestNotation();
  return 0;
}

