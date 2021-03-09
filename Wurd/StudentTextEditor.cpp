#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>

// My includes:
#include <iostream>
#include <fstream>
using namespace std;

// REMOVE THIS
ofstream output;
// REMOVE THIS

TextEditor* createTextEditor(Undo* un) {
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
	: TextEditor(undo) {
	m_row = 0;
	m_col = 0;
	m_lines.push_back("");
	m_cur_line = m_lines.begin();
}

// O(N), N = number of lines in file
StudentTextEditor::~StudentTextEditor() {
	// TODO
}

// O(M+N) time, M = number of lines, N = number of lines
// Contains full path & filename
bool StudentTextEditor::load(std::string file) {
	// Read the file
	ifstream infile(file);

	// If reading file failed, do nothing, return false
	if (!infile) return false;

	// TO-DO: If loading same file, reset the contents
	// Should reset file if user was already editing it
	// O(M) operation
	reset();

	// Read file line by line, strip /r
	// TO-DO: Why doesn't \r work?
	string s;
	while (getline(infile, s, '\n')) {
		m_lines.push_back(s); // Add to list of lines
	}

	// Successfully read file
	return true;
}

// O(M) time, M = number of lines edited
bool StudentTextEditor::save(std::string file) {
	// Open file, write lines in text editor to file
	ofstream outfile(file);
	// If it can't be opened, return false
	if (!outfile) return false;

	// Overwrite any data w/ new contents
	// TO-DO: Check if adds \n correctly
	for (auto it = m_lines.begin(); it != m_lines.end(); it++) {
		// Append \n after each line written
		outfile << *it << endl;
	}

	// Successfully saved file
	return true;
}

// O(N), N = number of rows edited
void StudentTextEditor::reset() {
	// Clear contents
	m_lines.clear();

	// Update the iterator
	m_lines.push_back("");
	m_cur_line = m_lines.begin();

	// Reset pos to (0, 0)
	m_row = 0;
	m_col = 0;

	// Clear undo state

	// TODO
}

// More detail on page 10 of spec
// O(1) time

// TO-DO: Known bug going all the way to the end
void StudentTextEditor::move(Dir dir) {
	output.open("output.txt");
	output << "Dir: " << dir << "Row: " << m_row << "Col: " << m_col << endl;

	switch (dir) {
	case (Dir::UP):
		// Move editing pos up one line
		// If already at top, do nothing
		if (m_row > 0) {
			m_row--;
			m_cur_line--;
		}
		break;

	case (Dir::DOWN):
		// Move editing pos down one line
		// If already at bottom, do nothing
		if (m_row < (m_lines.size() - 1)) {
			m_row++;
			m_cur_line++;
		}
		break;

	case (Dir::LEFT):
		// Do nothing if at first char on top line
		if (m_col == 0 && m_row == 0) break;

		// Move cursor left
		if (m_col > 0) m_col--;

		// If reached end of line
		else {
			// Go back to previous line
			m_cur_line--;
			m_row--;

			// Set cursor just past last char
			m_col = (*m_cur_line).length();
		}
		break;

	case (Dir::RIGHT):
		// Do nothing if at last char on last line
		if (m_col == m_lines.size() - 1 && m_row == (*m_cur_line).length() - 1) {
			break;
		}

		// Move cursor right
		if (m_col < (*m_cur_line).length() - 1) {
			m_col++;
		}

		// If reached end of line
		else {
			// Go to next line
			m_cur_line++;
			m_row++;

			// Set cursor to first char
			m_col = 0;
		}
		break;

	case (Dir::HOME): m_col = 0; break;
	case (Dir::END): m_col = (*m_cur_line).length(); break;
	default: break;
	}
}

// TO-DO: Must not depend on number of lines edited

// 1. O(L) time, L = length of line of text
// 2. O(L1+L2), L1 = current line of text, L2 = next line of text
void StudentTextEditor::del() {
	// 1. If editing pos on valid character
	// Delete character, move all chars in line to left
	if (m_col < (*m_cur_line).length() - 1) {
		(*m_cur_line).erase(m_col, 1);
		// auto copy = m_cur_line;
		// auto next = next(m_cur_line);
	}
	// 2. If end of line
	// Merge next line with this one
	else {

	}

	// Track with Undo

	// TODO
}

// TO-DO: Must not depend on number of lines edited

// 1. O(L) time, L = length of line of text
// 2. O(L1+L2), L1 = current line of text, L2 = next line of text
void StudentTextEditor::backspace() {
	// 1. If column > 0, delete character before editing pos
	if (m_col > 0) {
		(*m_cur_line).erase(m_col, 1);
		// TO-DO: Remove this?
		m_col--;
	}
	// 2. If col == 0, merge with previous line
	else {

	}


	// TODO
}

// O(L) time, L = length of current line
void StudentTextEditor::insert(char ch) {
	// Inserting tab should result in 4 spaces, move pos by 4
	if (ch == '\t') {
		(*m_cur_line).insert(m_col, "    ");
		m_col += 4;
	}
	// Move characters to right of editing pos by one
	else {
		(*m_cur_line).insert(m_col, string(1, ch));
		m_col += 1;
	}

	// Tell Undo to track insertion so can be undone later

	// TODO
}

// O(L) time, L = length of line of text
void StudentTextEditor::enter() {
	// Insert line break at current pos, move line one down
	insert('\n');
	m_cur_line++;

	// Move one line down, to first column
	m_row++; m_col = 0;

	// If last line, then just simply move it to end
	// TODO
}

// O(1) time
void StudentTextEditor::getPos(int& row, int& col) const {
	// Set row and column to editor's current editing position
	row = m_row; col = m_col;
}

// O(numRows * L), where L = average line length
// TO-DO: Check if size() is O(N) or O(1)
// TO-DO: Figure out what numbers should get returned 
int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
	// Start row higher than available rows, return 0
	if (startRow < 0 || numRows < 0 || m_lines.size() < startRow) return -1;

	// Clear all current lines
	lines.clear();

	// Start iterator at startRow
	auto it = m_lines.begin();
	advance(it, startRow);

	// Add new lines
	int count = 0;
	for (int x = 0; x < numRows && it != m_lines.end(); x++) {
		lines.push_back(*it);
		count++;
		it++;
	}
	return count;
}

// Page 12 for more details
void StudentTextEditor::undo() {
	// TODO
}