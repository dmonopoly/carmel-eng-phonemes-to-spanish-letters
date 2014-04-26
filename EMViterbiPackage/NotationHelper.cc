#include "NotationHelper.h"

#define EXTRA_PRINTING false

void NotationHelper::ReplaceSymbol(const string &old_s, const string &new_s,
                                   Notation *n) {
  vector<string> changed;
  for (int i = 0; i < n->first.size(); ++i) {
    if (n->first[i] == old_s) {
      if (EXTRA_PRINTING)
        cout << "REPLACING..." << n->first[i] << " w/ " << new_s << endl;
      changed.push_back(new_s);
    } else
      changed.push_back(n->first[i]);
  }
  if (EXTRA_PRINTING) {
    cout << "new vector for first: " << endl;
    for (auto it = changed.begin(); it != changed.end(); ++it) {
      cout << *it << " ";
    }
    cout << endl;
  }
  n->set_first(changed);
  changed.clear();
  for (int i = 0; i != n->second.size(); ++i) {
    if (n->second[i] == old_s) {
      if (EXTRA_PRINTING)
        cout << "REPLACING..." << n->second[i] << " w/ " << new_s << endl;
      changed.push_back(new_s);
    } else
      changed.push_back(n->second[i]);
  }
  n->set_second(changed);
}
