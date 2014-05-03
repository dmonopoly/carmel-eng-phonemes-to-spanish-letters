// Main method outputs results (quick test).

#include "TagGrammarFinder.h"

#define DO_MAIN false
#define EXTRA_PRINTING true

bool TagGrammarFinder::GetBigramTagGrammarFromOrganizedRows(
    const string &filename, map<Notation, double> *data,
    vector<string> *tag_list) {
  ifstream fin(filename.c_str());
  if (fin.fail()) {
    cerr << "Could not open file " << filename << endl;
    return false;
  } else {
    set<string> sounds;
    map<Notation, int> unigram_counts;
    map<Notation, int> bigram_counts;
    // Read bigram counts from file.
    int count;
    string sound1, sound2;
    // SUM_i C(s_i)
    vector<string> arb_sound; arb_sound.push_back(TagGrammarFinder::ARB_SOUND_PLACEHOLDER);
    Notation n_count_total(TagGrammarFinder::SIGMA + "C", arb_sound);
    while (true) {
      fin >> count;
      if (fin.eof())
        break;
      fin >> sound1 >> sound2;
      sounds.insert(sound1);
      sounds.insert(sound2);
      vector<string> vec1; vec1.push_back(sound1);
      vector<string> vec2; vec2.push_back(sound2);
      Notation n_count_seq("C", vec1, TagGrammarFinder::SEQ_DELIM,
                           vec2);
      Notation n_count_seq_total(TagGrammarFinder::SIGMA + "C", vec1,
                                 TagGrammarFinder::SEQ_DELIM, arb_sound);
      bigram_counts[n_count_seq] = count; // should only encounter once 
      bigram_counts[n_count_seq_total] += count;
      // Single probabilities. Treat C(s1) = SUM_i C(s1 s_i).
      Notation n_count("C", vec1);  // C(s1), followed by SUM_i C(s_i).
      unigram_counts[n_count] += count;
      unigram_counts[n_count_total] += count;
    }
    fin.close();
    // Determine tag grammar probabilities.
    // Unigrams.
    for (set<string>::iterator s1 = sounds.begin(); s1 != sounds.end(); ++s1) {
      vector<string> vec1; vec1.push_back(*s1);
      Notation nSingle("P", vec1);
      Notation n_count("C", vec1);
      try {
        (*data)[nSingle] = (double) unigram_counts.at(n_count) / 
                                  unigram_counts.at(n_count_total);
      } catch (out_of_range &e) {
        cerr << "Out of range error in unigram stuff: " << e.what() << endl;
      }
    }
    // Bigrams.
    double lambda = .9;
    for (set<string>::iterator s1 = sounds.begin(); s1 != sounds.end(); ++s1) {
      for (set<string>::iterator s2 = sounds.begin(); s2 != sounds.end(); ++s2) {
        vector<string> vec1; vec1.push_back(*s1);
        vector<string> vec2; vec2.push_back(*s2);
        Notation nGiven("P", vec2, TagGrammarFinder::GIVEN_DELIM, vec1);
        Notation n_count_seq("C", vec1, TagGrammarFinder::SEQ_DELIM, vec2);
        Notation n_count_seq_total(TagGrammarFinder::SIGMA + "C", vec1,
            TagGrammarFinder::SEQ_DELIM, arb_sound);
        // Fixed-lambda interpolation smoothing.
        Notation n_count_s2("C", vec2);
        double val = (double) (lambda * bigram_counts[n_count_seq] / // note the square brackets which default to 0 if not found
          bigram_counts.at(n_count_seq_total)) +
          .1 * unigram_counts.at(n_count_s2) / unigram_counts.at(n_count_total);
        // Old:
//         double val = (double) bigram_counts.at(n_count_seq) /
//           bigram_counts.at(n_count_seq_total);
        (*data)[nGiven] = val;
//         }
      }
    }
    // Pass sounds to tag_list.
    for (set<string>::iterator it = sounds.begin(); it != sounds.end(); ++it) {
      tag_list->push_back(*it);
    }
  }
  return true;
}

bool TagGrammarFinder::GetTrigramTagGrammarFromOrganizedRows(
    const string &filename, map<Notation, double> *data,
    vector<string> *tag_list) {
  ifstream fin(filename.c_str());
  if (fin.fail()) {
    cerr << "Could not open file " << filename << endl;
    return false;
  } else {
    set<string> sounds;
    map<Notation, int> unigram_counts;
    map<Notation, int> bigram_counts;
    map<Notation, int> trigram_counts;
    set<Notation> set_of_tri_counts; // For checking purposes.
    // Read trigram counts from file.
    int count;
    string sound1, sound2, sound3;
    // SUM_i C(s_i)
    vector<string> arb_sound; arb_sound.push_back(TagGrammarFinder::ARB_SOUND_PLACEHOLDER);
    Notation n_count_total(TagGrammarFinder::SIGMA + "C", arb_sound);
    while (true) {
      fin >> count;
      if (fin.eof())
        break;
      fin >> sound1 >> sound2 >> sound3;
      sounds.insert(sound1);
      sounds.insert(sound2);

      vector<string> vec1; vec1.push_back(sound1);
      vector<string> vec2; vec2.push_back(sound2);
      vector<string> vec3; vec3.push_back(sound3);
      // Bigram. Do twice per line in counts file.
      // 1
      Notation n_bigram_count_seq("C", vec1, TagGrammarFinder::SEQ_DELIM,
                                  vec2);
      Notation n_bigram_count_seq_total(TagGrammarFinder::SIGMA + "C", vec1,
                                        TagGrammarFinder::SEQ_DELIM,
                                        arb_sound);
      bigram_counts[n_bigram_count_seq] += count;
      bigram_counts[n_bigram_count_seq_total] += count;
      // 2
      Notation n_bigram_count_seq2("C", vec2, TagGrammarFinder::SEQ_DELIM,
                                  vec3);
      Notation n_bigram_count_seq_total2(TagGrammarFinder::SIGMA + "C", vec2,
                                        TagGrammarFinder::SEQ_DELIM,
                                        arb_sound);
      bigram_counts[n_bigram_count_seq2] += count;
      bigram_counts[n_bigram_count_seq_total2] += count;

      // Trigram
      vector<string> vec123; vec123.push_back(sound1); vec123.push_back(sound2); vec123.push_back(sound3);
      vector<string> vec12arb; vec12arb.push_back(sound1); vec12arb.push_back(sound2); vec12arb.push_back(TagGrammarFinder::ARB_SOUND_PLACEHOLDER);
      Notation n_trigram_count_seq("C", vec123,
                                   TagGrammarFinder::SEQ_DELIM);
      Notation n_trigram_count_seq_total(TagGrammarFinder::SIGMA + "C", vec12arb, TagGrammarFinder::SEQ_DELIM);

      set<Notation>::iterator iter = set_of_tri_counts.find(n_trigram_count_seq);
      if (iter != set_of_tri_counts.end()) {
        cerr << "Should not have seen this trigram count already." << endl;
        cerr << "Notation: " << n_trigram_count_seq << endl;
        return 1;
      }
      set_of_tri_counts.insert(n_trigram_count_seq);
      trigram_counts[n_trigram_count_seq] = count; // should only encounter once 
      trigram_counts[n_trigram_count_seq_total] += count;

      // Single probabilities. Treat C(s1) = SUM_i C(s1 s_i).
      Notation n_count("C", vec1);  // C(s1), followed by SUM_i C(s_i).
      unigram_counts[n_count] += count;
      unigram_counts[n_count_total] += count;
    }
    fin.close();
    // Determine tag grammar probabilities.
    // Unigrams.
    for (set<string>::iterator s1 = sounds.begin(); s1 != sounds.end(); ++s1) {
      vector<string> vec1; vec1.push_back(*s1);
      Notation nSingle("P", vec1);
      Notation n_count("C", vec1);
      try {
        (*data)[nSingle] = (double) unigram_counts.at(n_count) / 
                                  unigram_counts.at(n_count_total);
      } catch (out_of_range &e) {
        cerr << "Out of range error in unigram stuff: " << e.what() << endl;
      }
    }
    // Bigrams.
    double lambda2 = .9;
    for (set<string>::iterator s1 = sounds.begin(); s1 != sounds.end(); ++s1) {
      for (set<string>::iterator s2 = sounds.begin(); s2 != sounds.end(); ++s2) {
        vector<string> vec1; vec1.push_back(*s1);
        vector<string> vec2; vec2.push_back(*s2);
        Notation nGiven("P", vec2, TagGrammarFinder::GIVEN_DELIM, vec1);
        Notation n_count_seq("C", vec1, TagGrammarFinder::SEQ_DELIM, vec2);
        Notation n_count_seq_total(TagGrammarFinder::SIGMA + "C", vec1,
            TagGrammarFinder::SEQ_DELIM,
            arb_sound);
        // Fixed-lambda interpolation smoothing.
        Notation n_count_s2("C", vec2);
        double val = (double) (lambda2 * bigram_counts[n_count_seq] / // note the square brackets which default to 0 if not found
                     bigram_counts.at(n_count_seq_total)) +
                     (1 - lambda2) * unigram_counts.at(n_count_s2) /
                     unigram_counts.at(n_count_total);
        (*data)[nGiven] = val;
      }
    }
    // Trigrams.
    double lambda1 = .9;
    for (set<string>::iterator s1 = sounds.begin(); s1 != sounds.end(); ++s1) {
      for (set<string>::iterator s2 = sounds.begin(); s2 != sounds.end(); ++s2) {
        for (set<string>::iterator s3 = sounds.begin(); s3 != sounds.end(); ++s3) {
          vector<string> vec2; vec2.push_back(*s2);
          vector<string> vec3; vec3.push_back(*s3);
          vector<string> vec12; vec12.push_back(*s1); vec12.push_back(*s2);
          vector<string> vec123; vec123.push_back(*s1); vec123.push_back(*s2); vec123.push_back(*s3);
          vector<string> vec12arb; vec12arb.push_back(*s1); vec12arb.push_back(*s2); vec12arb.push_back(TagGrammarFinder::ARB_SOUND_PLACEHOLDER);
          vector<string> vec23arb; vec23arb.push_back(*s2); vec23arb.push_back(*s3); vec23arb.push_back(TagGrammarFinder::ARB_SOUND_PLACEHOLDER);
          // P(w3|w1 w2)
          Notation nGiven("P", vec3, TagGrammarFinder::GIVEN_DELIM, vec12);
          // C(w1 w2 w3)
          Notation cW1W2W3("C", vec123, TagGrammarFinder::SEQ_DELIM);
          // C(w1 w2)
          Notation cW1W2(TagGrammarFinder::SIGMA + "C", vec12arb,
              TagGrammarFinder::SEQ_DELIM);
          Notation cW2W3(TagGrammarFinder::SIGMA + "C", vec23arb,
              TagGrammarFinder::SEQ_DELIM);
          // C(w2), C(w3)
          Notation cW2("C", vec2);
          Notation cW3("C", vec3);

          double val = 0;
          // Fixed-lambda interpolation smoothing.
          if (bigram_counts.find(cW1W2) == bigram_counts.end() || bigram_counts.at(cW1W2) == 0) {
            // Prevent divide by zero when this key does not exist.
            val = (double) (1 - lambda1) * (lambda2 * bigram_counts[cW2W3] / 
                  unigram_counts.at(cW2) + (1 - lambda2) *
                  unigram_counts.at(cW3) /
                  unigram_counts.at(n_count_total));
          } else {
            double part1 = (double) (lambda1 * trigram_counts[cW1W2W3] / 
                bigram_counts.at(cW1W2));
            double part2 = (double) (lambda2 * bigram_counts[cW2W3] / 
                                  unigram_counts.at(cW2) + (1 - lambda2) *
                                  unigram_counts.at(cW3) /
                                  unigram_counts.at(n_count_total));
            val = part1 + (1 - lambda1) * part2;
            // Above is same as this:
//             val = (double) (lambda1 * trigram_counts[cW1W2W3] / 
//                 bigram_counts.at(cW1W2)) +
//                 (1 - lambda1) * (lambda2 * bigram_counts[cW2W3] / 
//                   unigram_counts.at(cW2) + (1 - lambda2) *
//                   unigram_counts.at(cW3) /
//                   unigram_counts.at(n_count_total));
          }
          // Check if value is not Nan or inf.
          if (isfinite(val))
            (*data)[nGiven] = val;
//           else
//             (*data)[nGiven] = .0001; // .000000000001;
        }
      }
    }

    // Pass sounds to tag_list.
    for (set<string>::iterator it = sounds.begin(); it != sounds.end(); ++it) {
      tag_list->push_back(*it);
    }
  }
  return true;
}

// Quick test for bigram version.
#if DO_MAIN
int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "No filename given." << endl;
    return 0;
  } 
  string filename = argv[1];
  map<Notation, double> data;
  vector<string> tag_list;
  TagGrammarFinder::GetTagGrammarFromOrganizedRows(filename, &data, &tag_list);
  cout << "Data:\n";
  for (map<Notation, double>::iterator it = data.begin(); it != data.end(); ++it) {
    cout << it->first << " " << it->second << endl;
  }
  cout << "Tag list:\n";
  for (vector<string>::iterator it = tag_list.begin(); it != tag_list.end(); ++it) {
    cout << *it << ",";
  }
  cout << endl;
  return 0;
}
#endif
