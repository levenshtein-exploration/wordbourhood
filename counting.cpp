#include <iostream>
#include <unordered_set>
#include <sstream>

#include "counting.h"


DoubleState::DoubleState (State * word, DulaState * dula, long long count, int word_automaton_size) {
	this->word = word;
	this->dula = dula;
	this->count = count;
	this->word_automaton_size = word_automaton_size;
}

string DoubleState::toString() const{
	stringstream ss;
	ss << "(" << this->dula->_idx << "," << this->word->_idx << "[" << this->count << "])";
	return ss.str();
}

bool DoubleState::operator==(const DoubleState& state) const{
	return (this->word->_idx == state.word->_idx) && (this->dula->_idx == state.dula->_idx);
}

void normalize (Automaton * automaton);


DoubleState get_next_state (const DoubleState & state, string transition) {
	State * next_word_state = state.word->getNext(transition);
	DulaState * next_dula_state = (DulaState *)state.dula->getNext(transition);

	return DoubleState(next_word_state, next_dula_state, state.count, state.word_automaton_size);
}

void insert_state (DoubleState & state, unordered_set<DoubleState> & set) {
	if (state.dula != NULL) {
		// Verify if the state is already in the set
		auto it = set.find(state);
		if (it != set.end()) {
			state.count += (*it).count;
			set.erase(it);
		}

		// Add the new state in the set
		set.insert(state);
	}
}


long long count_neighbors (Automaton * dula, Automaton * encode, int k, int w_size, int alphabet_size) {
	// Normalize the _idx of the automatons for the double state hash code
	normalize (dula); normalize (encode);

	// Init first double state
	DoubleState init (encode->states[0], (DulaState *)dula->states[0], 1, encode->states.size());
	unordered_set<DoubleState> previous_states;
	previous_states.insert(init);

	// Pre-define zero transition
	stringstream ss;
	for (int i=0 ; i<2*k+1 ; i++)
		ss << "0";
	string zero = ss.str();

	// Travel all along the word and errors
	for (int idx=0 ; idx<w_size+k ; idx++) {
		unordered_set<DoubleState> next_states;

		// From each previous dual states created
		for (auto & state : previous_states) {
			bool is$state = state.word->final;


			if (is$state) {
				string & transition = state.word->transitions[0];
				DoubleState next = get_next_state(state, transition);

				insert_state (next, next_states);

			} else {
				int nbZero = alphabet_size;

				// Basic transitions
				for (string & transition : state.word->transitions) {
					if (transition == zero)
						continue;

					DoubleState next = get_next_state (state, transition);
					insert_state (next, next_states);

					// Decrement the numbers of zero transitions if the transition was not made
					// by a $ consumption
					if (!next.word->final) {
						nbZero--;
					}
				}

				// Zero transitions
				DoubleState next = get_next_state (state, zero);
				next.count *= nbZero;
				insert_state (next, next_states);
			}
		}

		previous_states = next_states;
	}

	// Final count
	long long nbNeighbors = 0;
	for (const DoubleState & state : previous_states) {
		nbNeighbors += state.count;
	}

	return nbNeighbors;
}



void normalize (Automaton * automaton) {
	int idx = 0;
	for (auto & pair : automaton->states) {
		pair.second->_idx = idx++;
	}
}
