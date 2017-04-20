#include <string>
#include <iostream>
#include <fstream>

#include "automaton.h"
#include "levenshtein.h"
#include "word_automaton.h"
#include "counting.h"

using namespace std;


void print_help () {
	cout << "The command line must be like: ./wordbourhood [options] [words...]";
	cout << "-alphabet-size, -a: alphabet size [default 4]" << endl;
	cout << "-dula, -d: path to dula file in fsm format" << endl;
	cout << "-help, -h: help" << endl;
	cout << "-k: number of allowed errors" << endl;
	cout << "-verbose, -v: verbose" << endl;
	cout << "-words, -w: words filename (if you want to submit a long wordlist)" << endl;
}


void read_words_file (vector<string> & words, string filename) {
	ifstream file;
	file.open(filename);

	if (file.fail()) {
		cerr << "Impossible to load words from the file " << filename << endl;
		exit(1);
	}

	string word;
	while (true) {
		file >> word;
		if (!file)
			break;

		words.push_back(word);
	}
}



int main (int argc, char *argv[]) {
	int k = 1;
	bool verbose = false;
	int alphabet_size = 4;
	string dula_filename = "";
	vector<string> words;

	// Too few arguments
	if (argc == 1) {
		print_help();
		return 0;
	}

	// Arguments parsing
	for (int i=1 ; i<argc ; i++) {
		string arg = string(argv[i]);

		if (arg[0] != '-')
			words.push_back(arg);
		else {
			if (arg == "-a" || arg == "-alphabet-size") {
				alphabet_size = stoi(argv[++i]);
			} else if (arg == "-d" || arg == "-dula") {
				dula_filename = argv[++i];
			} else if (arg == "-h" || arg == "-help") {
				print_help();
				return 0;
			} else if (arg == "-k") {
				k = stoi(argv[++i]);
			} else if (arg == "-v" || arg == "-verbose") {
				verbose = true;
			} else if (arg == "-w" || arg == "-words") {
				read_words_file(words, argv[++i]);
			}
		}
	}

	if (words.size() == 0) {
		cerr << "No words to analyse in the command line." << endl;
		print_help();
	}

	// --- Dula loading ---
	Automaton * dul = NULL;
	if (dula_filename.size() == 0) {
		if (verbose)
			cout << "Create dula automaton (you sould use the -d argument to load it from a file)" << endl;
		dul = dula(k);
	} else {
		if (verbose)
			cout << "Load the file " << dula_filename << endl;
		dul = loadDulaFromFsm (dula_filename, k);
	}

	// --- Words analysis ---
	for (string word : words) {
		// Encode
		Automaton * enc = encode(word, k);

		if (verbose)
			cout << "Couting the neighborhood of " << word << " with a distance of " << to_string(k) << endl;

		// Analysis
		long long count = count_neighbors (dul, enc, k, word.size(), alphabet_size);
		cout << word << "\t" << count << endl;
	}

	return 0;
}














