#include "StudentSpellCheck.h"
#include <string>
#include <vector>


// My imports
#include <iostream>
#include <fstream>
using namespace std;

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
	// TODO
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
	const string file_name = "test.txt";
	ifstream infile(file_name);

	// If reading the file failed, return false
	if (!infile) {
		cerr << "Error: Cannot open" << file_name << endl;
		return false;
	}

	// Read the file line by line
	string s;
	while (getline(infile, s)) {
		cerr << s << endl;
	}

	// Successfully read file
	return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	return false; // TODO
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	// TODO
}
