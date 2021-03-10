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

StudentSpellCheck::StudentSpellCheck() {
	m_root = new TrieNode();
}

StudentSpellCheck::~StudentSpellCheck() {
	m_root->deleteAllNodes(m_root);
}


bool StudentSpellCheck::load(std::string dictionaryFile) {
	// Read the dictionary
	ifstream infile(dictionaryFile);

	// If reading file failed, do nothing, return false
	if (!infile) return false;

	string s;
	// Read file line by line
	// TO-DO: Why doesn't \r work?
	while (getline(infile, s, '\n')) {
		m_root->insert(s);
	}

	// Successfully read file
	return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	// If word found, return true
	if (m_root->has(word)) return true;

	// Otherwise, clear suggestions
	suggestions.clear();

	// Loop through every possible version of string
	// TO-DO: O(N * NUM_CHARS) right now
	int suggestionsFound = 0;
	for (int modified = 0; modified < word.length(); modified++) {
		const string left = word.substr(0, modified);
		const string right = word.substr(modified + 1);

		for (int y = 0; y < NUM_LETTERS; y++) {
			const char mid = asChar(y);
			const string possibleWord = left + mid + right;

			if (m_root->has(possibleWord)) {
				suggestionsFound++;
				suggestions.push_back(possibleWord);
			}
			if (suggestionsFound == max_suggestions) break;
		}
	}

	return false;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	string acc = "";
	Position p;

	// Go through entire line
	for (int x = 0; x < line.length(); x++) {
		// Lowercase each letter for comparison
		const char ch = line.at(x);
		if (isValidChar(ch)) {
			// Set start index
			if (acc == "") p.start = x;
			acc.push_back(ch); // Add char to end of string
		}
		// If it's an invalid char
		else {
			// If word was being formed
			if (acc != "") {
				p.end = x - 1; // Set end index
				 
				// Push if not in Trie
				if (!m_root->has(acc)) {
					problems.push_back(p);
				}
			}
			acc = ""; // Restore string to empty
		}
	}

	if (acc != "") {
		p.end = line.length() - 1;
		if (!m_root->has(acc)) problems.push_back(p);
	}
}

