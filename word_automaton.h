#include <string>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <sstream>
#include <algorithm>

#include "automaton.h"



string get_subword (const string & word, int idx, int k);
vector<bool> get_bit_vector (const string & word, const char c);
Automaton * encode (string & word, int k);

