#include "StudentSpellCheck.h"
#include <string>
#include <vector>

#include <fstream>
using namespace std;

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

	// Reset dictionary if loading a different one
	reset();

	// Read file character by character, strip \r if exists
	string acc; char ch;
	while (infile.get(ch)) {
		switch (ch) {
			// Detect line break, insert, reset accumulator
		case '\n': m_root->insert(acc); acc = ""; break;
			// Detect tab, convert to 4 spaces
		case '\t': acc.append("    ");
			// Do nothing if see \r
		case '\r': break;
			// Default is to add to accumulator
		default: acc.push_back(ch); break;
		}
	}

	// Successfully read file
	return true;
}

void StudentSpellCheck::reset() {
	// Delete all nodes connected to root node
	m_root->deleteAllNodes(m_root);

	// Restore root node
	m_root = new TrieNode();
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	// If word found, return true
	if (m_root->has(word)) return true;

	// Otherwise, clear suggestions in O(oldS) time
	suggestions.clear();

	// Loop through every possible version of string, check in Trie
	// O(L^2) time since O(L) iterations * O(L) lookup time
	const string copy = word;
	for (int modifiedIdx = 0; modifiedIdx < word.length(); modifiedIdx++) {
		for (int y = 0; y < NUM_LETTERS; y++) {
			// Replace char in string with modified char
			const char changedChar = m_root->asChar(y);
			word[modifiedIdx] = changedChar;

			// If it found a word, add to suggestions
			// O(maxSuggestions)
			if (m_root->has(word)) {
				max_suggestions--;
				suggestions.push_back(word);
			}
			// If remaining suggestions are 0, stop looping
			if (max_suggestions == 0) break;
		}

		// Reset the word
		word = copy;
	}

	// Could not find word
	return false;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	string acc = "";
	Position p;

	// Go through entire line
	for (int x = 0; x < line.length(); x++) {
		// Lowercase each letter for comparison
		const char ch = line.at(x);
		if (m_root->isValidChar(ch)) {
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

	// Check string that was being formed at the end of line
	if (acc != "") {
		p.end = line.length() - 1;
		if (!m_root->has(acc)) problems.push_back(p);
	}
}

