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
	int alphabet_size = 4;

	// Too few arguments
	if (argc < 4) {
		cerr << "Wrong arguments. The command line must be like:" << endl;
		cerr << "./wordbourhood <word> <distance> <alphabet size> [options]" << endl;
		cerr << "Options:" << endl;
		cerr << "-v: Verbose" << endl;
		cerr << "-d: path to dula file" << endl;
		return -1;
	}

	// 
	for (int i=4 ; i<argc ; i++) {
		char * arg = argv[i];

		if (arg[0] != '-')
			continue;

		if (arg[1] == 'v' && arg[2] == '\0')
			verbose = true;
	}

	k = stoi(argv[2]);
	string word (argv[1]);
	alphabet_size = stoi(argv[3]);

	if (verbose)
		cout << "Create automata" << endl;

	Automaton * dul = dula(k);
	Automaton * enc = encode(word, k);

	if (verbose)
		cout << "Couting the neighborhood of " << word << " with a distance of " << to_string(k) << endl;
	long long count = count_neighbors (dul, enc, k, word.size(), alphabet_size);

	cout << count << endl;

	return 0;
}














