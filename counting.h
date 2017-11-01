#include <string>

#include "automaton.h"

using namespace std;


class DoubleState: State {
public:
	State * word;
	DulaState * dula;
	long long count;

	int word_automaton_size;

	DoubleState (State * word, DulaState * dula, long long count, int word_automaton_size);
	string toString() const;

	bool operator==(const DoubleState& state)const;
};


namespace std {
    template<> struct hash<DoubleState> {
        size_t operator()(DoubleState const& state) const {
            return (((state.dula->_idx * state.word_automaton_size)) % (1<<31) + state.word->_idx) % (1<<31);
        }
    };
}



void print_automaton (bool value);
long long count_neighbors (Automaton * dula, Automaton * encode, int k, int w_size, int alphabet_size);

