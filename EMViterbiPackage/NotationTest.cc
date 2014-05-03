// clang++ -std=c++11 -stdlib=libc++ EMViterbiPackage/NotationTest.cc
// EMViterbiPackage/Notation.cc
#include <assert.h>

// TODO: Could clean up and use constants.
// Includes various libraries as well and set namespace to std.
#include "Notation.h"

const string X = "X";
const string Y = "Y";
const string A = "A";
const string B = "B";

void TestVecXGivenY() {
  vector<string> vecX; vecX.push_back(X);
  vector<string> vecY; vecY.push_back(Y);
  Notation pXGivenY("P", vecX, Notation::GIVEN_DELIM, vecY);
  stringstream ss;  // We simulate 'cout <<' with this.
  ss << pXGivenY;
  cout << ss.str() << endl;
  assert(ss.str() == "P(" + X + Notation::GIVEN_DELIM + Y + ")");
  ss.clear();
}

void TestXGivenY() {
  Notation pXGivenY("P", {X}, Notation::GIVEN_DELIM, {Y});
  stringstream ss;  // We simulate 'cout <<' with this.
  ss << pXGivenY;
  cout << ss.str() << endl;
  assert(ss.str() == "P(" + X + Notation::GIVEN_DELIM + Y + ")");
  ss.clear();
}

void TestVecJustX() {
  vector<string> vecX; vecX.push_back(X);
  Notation pX("P", vecX);
  stringstream ss;  // We simulate 'cout <<' with this.
  ss << pX;
  cout << ss.str() << endl;
  assert(ss.str() == "P("+X+")" && "Nonexistent second vector didn't work.");
}

void TestJustX() {
  Notation pX("P", {X});
  stringstream ss;  // We simulate 'cout <<' with this.
  ss << pX;
  cout << ss.str() << endl;
  assert(ss.str() == "P("+X+")" && "Nonexistent second vector didn't work.");
}

void TestVecChainX() {
  vector<string> vecABA;
  vecABA.push_back(A);
  vecABA.push_back(B);
  vecABA.push_back(A);
  Notation pABA("P", vecABA, Notation::SEQ_DELIM);
  stringstream ss;
  ss << pABA;
  cout << ss.str() << endl;
  assert(ss.str() == "P(A B A)");

  ss.clear();
  ss.str("");

  Notation pABA2("P", vecABA, Notation::AND_DELIM);
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

void TestChainX() {
  Notation pABA("P", {A,B,A}, Notation::SEQ_DELIM);
  stringstream ss;
  ss << pABA;
  cout << ss.str() << endl;
  assert(ss.str() == "P(A B A)");

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
  TestVecXGivenY();
  TestJustX();
  TestVecJustX();
  TestChainX();
  TestVecChainX();
  cout << "Test(s) passed." << endl;
}

int main() {
  TestNotation();
  return 0;
}

