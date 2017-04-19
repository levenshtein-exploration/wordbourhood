#include <string>
#include <iostream>

#include "automaton.h"
#include "levenshtein.h"
#include "word_automaton.h"
#include "counting.h"

using namespace std;




int main (int argc, char *argv[]) {
	int k = 1;
	bool verbose = false;

	// Too few arguments
	if (argc <= 2) {
		cerr << "Wrong arguments. The command line must be like:" << endl;
		cerr << "./wordbourhood <word> <distance> [options]" << endl;
		return -1;
	}

	// 
	for (int i=3 ; i<argc ; i++) {
		char * arg = argv[i];

		if (arg[0] != '-')
			continue;

		if (arg[1] == 'v' && arg[2] == '\0')
			verbose = true;
	}

	k = stoi(argv[2]);
	string word (argv[1]);

	if (verbose)
		cout << "Create automata" << endl;

	Automaton * dul = dula(k);
	Automaton * enc = encode(word, k);

	if (verbose)
		cout << "Couting the neighborhood of " << word << " with a distance of " << to_string(k) << endl;
	long long count = count_neighbors (dul, enc, k, word.size(), 3);

	cout << count << endl;

	return 0;
}














