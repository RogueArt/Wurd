#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>

// My includes
#include <iostream>
#include <fstream>
using namespace std;

TextEditor* createTextEditor(Undo* un) {
	return new StudentTextEditor(un);
}

// O(1) time
StudentTextEditor::StudentTextEditor(Undo* undo)
	: TextEditor(undo) {
	// TODO
}

// O(N) time, N = number of lines
StudentTextEditor::~StudentTextEditor() {
	// TODO
}

// O(M+N) times, M = number of lines edited, N = lines loaded
bool StudentTextEditor::load(std::string file) {
	// Load lines onto a linked list

	// If specified file not found, do nothing, return false

	return false;  // TODO
}

// O(M) time, M = number of lines in editor being edited
bool StudentTextEditor::save(std::string file) {
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

// O(N) time, number of rows being edited
void StudentTextEditor::reset() {

	// TODO
}

void StudentTextEditor::move(Dir dir) {
	// TODO
}

// O(L1+L2), L1 = cur line, L2 = next line
// Delete char @ current position 
void StudentTextEditor::del() {
	// TODO
}

void StudentTextEditor::backspace() {
	// TODO
}

// O(L), L = number of lines
// Insert char @ current position 
void StudentTextEditor::insert(char ch) {
	// TODO
}

// O(L), L = number of lines
// Add line break, show it graphically
void StudentTextEditor::enter() {
	// TODO
}


void StudentTextEditor::getPos(int& row, int& col) const {
	// TODO
}

void StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
	// TODO
}

void StudentTextEditor::undo() {
	// TODO
}
