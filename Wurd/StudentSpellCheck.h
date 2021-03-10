#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

const int NUM_LETTERS = 27;
class TrieNode;

class StudentSpellCheck : public SpellCheck {
public:
	StudentSpellCheck();
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
	TrieNode* m_root;

	char asChar(int idx) { return idx == 26 ? '\'' : char(idx + 'a'); }
	bool isValidChar(char ch) {
		// Return if alphabetic or is an apostrophe
		return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch == '\'');
	}
};

class TrieNode {
public:
	TrieNode();
	// Insert a string into the trie
	void insert(std::string s);

	// Check if the trie has a string
	bool has(std::string s);

	// Convert characters to an array index
	int asIdx(char ch) { return ch == '\'' ? 26 : ch - 'a'; }

	void deleteAllNodes(TrieNode* node);

private:
	TrieNode* children[NUM_LETTERS];
	bool isEnd;
};

TrieNode::TrieNode() {
	// Initialize all children to nullptr
	for (int x = 0; x < NUM_LETTERS; x++) {
		children[x] = nullptr;
	}
	// All nodes are isEnd by default
	isEnd = false;
}

void TrieNode::insert(std::string s) {
	// Make a copy of the root node
	TrieNode* copy = this;

	// Traverse the string, s
	for (char ch : s) {
		const int idx = asIdx(tolower(ch));

		// If char doesn't exist in Trie, add it
		if (copy->children[idx] == nullptr) {
			copy->children[idx] = new TrieNode();
		}

		// Go one level into the Trie
		copy = copy->children[idx];
	}

	// Mark last char of string to true
	copy->isEnd = true;
}

bool TrieNode::has(std::string s) {
	// Make a copy of the root node
	TrieNode* copy = this;

	// Traverse string
	for (int x = 0; x < s.length(); x++) {
		const int idx = asIdx(tolower(s.at(x)));

		// If current char not in Trie, return false
		if (copy->children[idx] == nullptr) return false;

		// Otherwise go to next char of the word
		copy = copy->children[idx];
	}

	// Return true if we found the word
	return copy->isEnd;
}

void TrieNode::deleteAllNodes(TrieNode* node) {
	if (node == nullptr) return;

	for (int x = 0; x < NUM_LETTERS; x++) {
		if (node->children[x] != nullptr) {
			deleteAllNodes(node->children[x]);
		}
	}

	delete node;
}

#endif  // STUDENTSPELLCHECK_H_
