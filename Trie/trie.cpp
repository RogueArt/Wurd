// Header file includes
#include <string>
#include <vector>

// CPP includes
#include <fstream>
#include <cctype>
const int NUM_LETTERS = 27;
using namespace std;

// REMOVE
#include <iostream>

class TrieNode {
public:
  TrieNode();
  // Insert a string into the trie
  void insert(string s);

  // Check if the trie has a string
  bool has(string s);

  // Convert characters to an array index
  int asIdx(char ch) { return ch == '\'' ? 26 : ch - 'a'; }

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

void TrieNode::insert(string s) {
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

bool TrieNode::has(string s) {
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

bool load(TrieNode* root, string dictionaryFile) {
  // Read the dictionary
  ifstream infile(dictionaryFile);

  // If reading file failed, do nothing, return false
  if (!infile) return false;

  
  string s;
  // Read file line by line
  // TO-DO: Why doesn't \r work?
  while (getline(infile, s, '\n')) {
    root->insert(s); // Add to list of lines
  }

  // Successfully read file
  return true;
}

char asChar(int idx) {
  return idx == 26 ? '\'' : char(idx + 'a');
}

bool spellCheck(TrieNode* root, string word, int maxSuggestions, vector<string>& suggestions) {
  // If word found, return true
  if (root->has(word)) return true;

  // Otherwise, clear suggestions
  suggestions.clear();

  // Loop through every possible version of string
  // TO-DO: O(N * NUM_CHARS) right now
  int suggestionsFound = 0;
  for (int modified = 0; modified < word.length(); modified++) {
    const string left = word.substr(0, modified);
    const string right = word.substr(modified+1);

    for (int y = 0; y < NUM_LETTERS; y++) {
      const char mid = asChar(y);
      const string possibleWord = left + mid + right;

      if (root->has(possibleWord)) {
        suggestionsFound++;
        suggestions.push_back(possibleWord);
      }
      if (suggestionsFound == maxSuggestions) break;
    }
  }

  return false;
}

struct Position {
  int start;
  int end; // inclusive
};

bool isValidChar(char ch) {
  // Return if alphabetic or is an apostrophe
  return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch == '\'');
}

void spellCheckLine(TrieNode* root, const string& line, vector<Position>& problems) {
  string acc = "";
  Position p;

  // Go through entire line
  for (int x = 0; x < line.length(); x++) {
    // Lowercase each letter for comparison
    const char ch = line.at(x);
    if (isValidChar(ch)) {
      // Set start index
      if (acc == "") p.start = x;
      acc += ch; // Add char to end of string
    }
    // If it's an invalid char
    else {
      // If word was being formed
      if (acc != "") {
        p.end = x-1; // Set end index
         // Push if not in Trie
        if (!root->has(acc)) problems.push_back(p);
      }
      acc = ""; // Restore string to empty
    }
  }
}

int main() {
  const string file = "X:/College/CS 32/Project 4/Trie/dictionary.txt";
  TrieNode* root = new TrieNode;

  // See if file was loaded correctly
  if (load(root, file)) cerr << "Successfully loaded file!" << endl;
  else cerr << "Could not load file!" << endl;

  // See if word zebra is in trie
  // const string word = "zebra";
  const string words[5] = { "zebra", "apple", "yeet", "asdf", "a" };

  for (string word : words) {
    if (root->has(word)) cerr << word << " is present." << endl;
    else cerr << word << " is not there." << endl;
  }

  // vector<string> suggestions;
  // spellCheck(root, "ar", 10, suggestions); 
  // for (int x = 0; x < suggestions.size(); x++)
  //   cout << suggestions[x] << endl;

  const string line = "Diz\"iz-a tezt.";
  vector<Position> problems;
  spellCheckLine(root, line, problems);
  for (int x = 0; x < problems.size(); x++)
    cout << "(" << problems[x].start << " , " << problems[x].end << ")" << endl;
}