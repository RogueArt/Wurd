#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

const int NUM_LETTERS = 27;

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck() { }
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
	struct TrieNode {
		struct TrieNode* children[NUM_LETTERS];
		bool isEnd;

		// Constructor function
		TrieNode() {
			for (int x = 0; x < NUM_LETTERS; x++) {
				children[x] = NULL;
			}
			isEnd = false;
		}
	};
};

#endif  // STUDENTSPELLCHECK_H_
