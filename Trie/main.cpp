#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
using namespace std;

const int len = 256;
struct TrieNode {
public:
  
  // Store address of character
  TrieNode* Trie[len];

  // Is last char of string?
  bool isEnd;

  // Constructor function
  TrieNode() {
    for (int x = 0; x < len; x++) {
      Trie[x] = NULL;
    }
    isEnd = false;
  }
};

// Insert a string into Trie
void insertStr(TrieNode* root, string s) {
  TrieNode* temp = root;

  // Traverse the string, s
  for (int x = 0; x < s.length(); x++) {
    // If char doesn't exist in Trie, add it
    if (temp->Trie[s[x]] == NULL) {
      // Create a new node
      temp->Trie[s[x]] = new TrieNode();
    }

    // Temp goes one level into the trie
    temp = temp->Trie[s[x]];
  }

  // Mark the last char of string to true
  temp->isEnd = true;
}

// Print suggestions for the string
void printSuggestions(TrieNode* root, string res) {
  // If current char is last char of string
  if (root->isEnd == true) {
    cout << res << " ";
  }

  // Iterate over all possible chars of string
  for (int x = 0; x < 256; x++) {
    // If current char in Trie
    if (root->Trie[x] != NULL) {
      // Insert current char into Trie
      res.push_back(x);
      printSuggestions(root->Trie[x], res);
      res.pop_back();
    }
  }
}

// Check if it's present in Trie or not
bool checkPresent(TrieNode* root, string key) {
  // Traverse string
  for (int x = 0; x < key.length(); x++) {
    // If current char not in Trie
    if (root->Trie[key[x]] == NULL) {
      printSuggestions(root, key.substr(0, x));
      return false;
    }
    root = root->Trie[key[x]];
  }
  if (root->isEnd == true) return true;

  printSuggestions(root, key);
  return false;
}

bool load(std::string dictionaryFile) {
  // Read the file
  ifstream infile(dictionaryFile);

  // If failed, do nothing, return false
  if (!infile) return false;

  string s;
  TrieNode* root = new TrieNode();
  while (getline(infile, s, '\n')) {
    // Add string to trie
    insertStr(root, s);
  }
}

// Driver Code 
int main() {
  const string file = "X:/College/CS 32/Project 4/Trie/dictionary.txt";
  TrieNode* root = new TrieNode();

  // Read the file
  ifstream infile(file);
  // If failed, do nothing, return false
  if (!infile) return false;

  string s;
  while (getline(infile, s, '\n')) {
    // Add string to the trie
    insertStr(root, s);
  }

  // TESTING THE TRIE HERE
  string key = "don't";
  if (checkPresent(root, key))
    cout << key << " is present in the Trie." << endl;

  return 0;
}