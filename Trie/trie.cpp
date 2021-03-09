// Header file includes
#include <string>
#include <vector>

// CPP includes
#include <fstream>
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
  
  // TO-DO: Return suggestions
  void suggest(TrieNode*, string s) {};

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
  TrieNode* copy = this;

  // Traverse the string, s
  for (char ch : s) {
    const int idx = asIdx(ch);

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
  TrieNode* copy = this;

  // Traverse string
  for (int x = 0; x < s.length(); x++) {
    const int idx = asIdx(s.at(x));

    // If current char not in Trie
    if (copy->children[idx] == NULL) {
      // Give suggestions, return false
      suggest(copy, s.substr(0, x));
      return false;
    }

    // Otherwise go to next char of the word
    copy = copy->children[idx];
  }

  // If we found the word, return true
  if (copy->isEnd == true) return true;

  // Otherwise give suggestions return false
  suggest(copy, s); return false;
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

int main() {
  const string file = "X:/College/CS 32/Project 4/Trie/dictionary.txt";
  TrieNode* root = new TrieNode;

  // See if file was loaded correctly
  if (load(root, file)) cerr << "Successfully loaded file!" << endl;
  else cerr << "Could not load file!" << endl;

  // See if word zebra is in trie
  const string word = "zebra";
  const string words[5] = { "zebra", "apple", "yeet", "asdf", "a" };

  if (root->has(word)) cerr << word << " is present." << endl;
  else cerr << word << " is not there." << endl;
}