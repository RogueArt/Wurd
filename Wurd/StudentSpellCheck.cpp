#include "StudentSpellCheck.h"
#include <string>
#include <vector>

// TO-DO: Remove below
#include <fstream>
using namespace std;
// TO-DO: Remove above

SpellCheck* createSpellCheck() {
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
	// TODO


}


bool StudentSpellCheck::load(std::string dictionaryFile) {
	// Read the dictionary
	ifstream infile(dictionaryFile);

	// If reading file failed, do nothing, return false
	if (!infile) return false;

	TrieNode* root = new TrieNode();
	string s;

	// Read file line by line
	// TO-DO: Why doesn't \r work?
	while (getline(infile, s, '\n')) {
		// insert(root, s); // Add to list of lines
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

