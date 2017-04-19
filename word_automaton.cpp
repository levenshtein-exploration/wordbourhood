#include "word_automaton.h"

unordered_set<char> get_alphabet (const string & word) {
	unordered_set<char> alphabet;
	for (char c : word)
		alphabet.insert(c);

	return alphabet;
}

string get_subword (const string & word, int idx, int k) {
	stringstream ss;
	for (int i=idx-k ; i<=idx+k ; i++) {
		if (i<0 || i>=word.size())
			ss << '$';
		else
			ss << word[i];
	}

	return ss.str();
}

vector<bool> get_bit_vector (const string & word, const char c) {
	vector<bool> bits;

	for (char c_word : word)
		bits.push_back(c_word == c ? true : false);

	return bits;
}

Automaton * encode (string & word, int k) {
	int w_size = word.size();

	// Create basic states
	Automaton * aut = new Automaton();
	for (int idx=0 ; idx<=k+w_size ; idx++)
		aut->states[idx] = new State();

	// Pre-create the O^{2k+1}
	stringstream zero;
	for (int i=0 ; i<2*k+1 ; i++)
		zero << "0";

	// Fill basic transitions
	unordered_set<char> alphabet = get_alphabet(word);
	for (int idx=0 ; idx<k+w_size ; idx++) {
		string subword = get_subword(word, idx, k);
		State * cur_state = aut->states[idx];

		// Multiple deletions case O^{2k+1}
		cur_state->addTransition(aut->states[idx+1], zero.str());

		// Normal case
		for (char c : alphabet) {
			// Create the string transition
			stringstream ss;
			vector<bool> bits = get_bit_vector(subword, c);
			for (bool val : bits)
				ss << (val ? "1" : "0");

			// Add the transition if it's a new one
			vector<string> & transitions = cur_state->transitions;
			if (find(transitions.begin(), transitions.end(), ss.str()) == transitions.end())
				cur_state->addTransition(aut->states[idx+1], ss.str());
		}
	}

	// Create $ states (use the unused final property)
	int offset = 0;
	int nb_$states = 2*k;
	if (k > w_size) {
		nb_$states = k + w_size;
		offset = k - w_size;
	}

	for (int i=0 ; i<nb_$states ; i++) {
		State * st = new State();
		st->final = true;
		aut->states[aut->states.size()] = st;
	}

	// Create $ transitions
	int first_idx = k + w_size - nb_$states;
	for (int idx=0 ; idx<nb_$states ; idx++) {
		// Transition
		int nb_1 = idx + 1 + offset;
		stringstream ss;
		for (int i=0 ; i<2*k+1 ; i++)
			if (i < 2*k+1-nb_1)
				ss << "0";
			else
				ss << "1";

		// Diagonal transition
		State * current = aut->states[first_idx+idx];
		State * next = aut->states[k+w_size+idx+1];
		current->addTransition(next, ss.str());

		// Horizontal transition
		if (idx > 0) {
			current = aut->states[k+w_size+idx];
			current->addTransition(next, ss.str());
		}
	}

	// Prints
	// for (int idx=0 ; idx<aut->states.size() ; idx++) {
	// 	State * current = aut->states[idx];

	// 	for (string & trans : current->transitions) {
	// 		cout << current->_idx << " -> " << current->getNext(trans)->_idx << endl;
	// 		cout << trans << endl;
	// 	}
	// 	cout << endl << endl;
	// }

	return aut;
}

