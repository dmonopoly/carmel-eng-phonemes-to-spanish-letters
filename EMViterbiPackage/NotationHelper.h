#ifndef NOTATION_HELPER_H_
#define NOTATION_HELPER_H_

#include <iostream>
#include <vector>

#include "Notation.h"

namespace NotationHelper {
  // Replaces all symbols in n that are equal to old_s with new_s.
  void ReplaceSymbol(const string &old_s, const string &new_s, Notation *n);
}

#endif
