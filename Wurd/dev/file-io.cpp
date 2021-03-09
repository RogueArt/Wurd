#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool read() {
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

bool write() {
  // Attempt to open outfile
  const string fileName = "tests.txt";
  ofstream outfile(fileName);

  // If it's not there, 
  if (!outfile) {
    cerr << "Error: Cannot create " << fileName << endl;
    return false;
  }

  // Write text to outfile  
  outfile << "This text written to file" << endl;
  return true;
}