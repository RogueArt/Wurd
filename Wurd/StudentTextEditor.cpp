#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>

// My includes:
#include <fstream>
using namespace std;

TextEditor* createTextEditor(Undo* un) {
	return new StudentTextEditor(un);
}

// O(1) time to initialize all variables
StudentTextEditor::StudentTextEditor(Undo* undo)
	: TextEditor(undo) {
	m_row = 0;
	m_col = 0;
	m_lines.push_back("");
	m_cur_line = m_lines.begin();
}

// O(N), N = number of lines in file
// Should not free the Undo pointer
// TO-DO: Check if this is correct
StudentTextEditor::~StudentTextEditor() {
	m_lines.clear();
}

// O(M+N+U) time
// M = number of chars in the editor being edited
// N = number of chars being loaded in
// U = number of items in undo stack
// Contains full path & filename
bool StudentTextEditor::load(std::string file) {
	// Read the file
	ifstream infile(file);

	// If reading file failed, do nothing, return false
	if (!infile) return false;

	// Should reset file if user was already editing it
	// O(M) operation
	reset();

	// Read file line by line, strip /r if exists
	// TO-DO: Strip \r from line
	string s;
	while (getline(infile, s, '\n')) {
		m_lines.push_back(s); // Add to list of lines
	}

	// Successfully read file
	return true;
}

// O(M) time, M = number of chars edited
// TO-DO: See if it makes a new file if doesn't exist at path
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

// O(N+U), N = number of rows edited, U = items in undo stack
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
	getUndo()->clear();
}

// O(1) time
void StudentTextEditor::move(Dir dir) {
	switch (dir) {
	case (Dir::UP): {
		// Do nothing if at top row aka m_row == 0
		if (m_row > 0) {
			// If current column is farther than upper line's col
			// Move cursor to end of upper line  
			const int upperLineLen = (*prev(m_cur_line)).length();
			if (m_col > upperLineLen) m_col = upperLineLen;

			// Move editing pos up one line
			m_row--; m_cur_line--;
		}
		break;
	}

	case (Dir::DOWN): {
		// Move editing pos down one line
		// If already at bottom, do nothing
		if (m_row < (m_lines.size() - 1)) {
			// If current column is farther than lower line's col
			// Move cursor to end of upper line  
			const int lowerLineLen = (*next(m_cur_line)).length();
			if (m_col > lowerLineLen) m_col = lowerLineLen;

			// Move editing pos down one line
			m_row++; m_cur_line++;
		}
		break;
	}

	case (Dir::LEFT): {
		// Three possibilities for movement:
		// 1. ALready on first char of first line -- do nothing
		if (m_col == 0 && m_row == 0) {}

		// 2. Not on the first char, simply move left
		else if (m_col > 0) m_col--;

		// 3. First char of line that isn't first line
		// Move up one line
		else {
			// Go back to previous line
			m_cur_line--;
			m_row--;

			// Set cursor just past last char
			m_col = (*m_cur_line).length();
		}
		break;
	}
									

	case (Dir::RIGHT): {
		const int lineLen = (*m_cur_line).length();
		const int numLinesPage = m_lines.size();

		// Three possibilities for movement:
		// 1. Already after last char of last line -- do nothing
		if (m_row == numLinesPage - 1 && m_col == lineLen) {}

		// 2. Editing position is just after last char of current line
		else if (m_col == lineLen) {
			m_cur_line++; // Current line is next line
			m_row++; // Move down a row
			m_col = 0; // Go to first char on line
		}

		// 3. Otherwise just move cursor right
		else { m_col++; }
		break;
	}

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
	if (m_col < lastCol()) {
		const char deletedCh = (*m_cur_line)[m_col];
		(*m_cur_line).erase(m_col, 1);

		// Add to undo stack as DELETE after processing
		getUndo()->submit(Undo::Action::DELETE, m_row, m_col, deletedCh);
	}
	// 2. If just after last character on a line
	// Merge next line with this one
	else if (m_row < lastRow() - 1) {
		mergeNextLine(true);
	}
	// 3. Last character on last line, do nothing
	else {}
}

void StudentTextEditor::mergePrevLine(bool logAction) {
	list<string>::iterator prevLine = prev(m_cur_line); // Go to previous line
	string s = (*m_cur_line); // String at previous line
	int prevLen = (*prevLine).length(); // Length of previous line
	m_lines.erase(m_cur_line); // Delete current line
	m_cur_line = prevLine; // Set current line to previous line
	(*m_cur_line).append(s); // Add back original string
	m_row--; // Move up one row
	m_col = prevLen;

	// Add to undo stack as JOIN after processing
	getUndo()->submit(Undo::Action::JOIN, m_row, m_col, '\0');
}

void StudentTextEditor::mergeNextLine(bool logAction) {
	list<string>::iterator nextLine = next(m_cur_line); // Go to next line
	string s = (*nextLine); // String at current line
	m_lines.erase(nextLine); // Delete the next line
	(*m_cur_line).append(s); // Add back next line contents to current line

	// Add to undo stack as JOIN after processing
	if (logAction) getUndo()->submit(Undo::Action::JOIN, m_row, m_col, '\0');
}



void StudentTextEditor::splitLine(bool logAction) {
	// Add to undo stack as SPLIT before processing
	if (logAction) getUndo()->submit(Undo::Action::SPLIT, m_row, m_col, '\0');

	// Insert line break at current pos, move line one down
	// Split string into two
	string lhs = (*m_cur_line).substr(0, m_col);
	string rhs = (*m_cur_line).substr(m_col);

	// Split lines into two
	(*m_cur_line) = rhs;
	m_cur_line = m_lines.insert(m_cur_line, lhs);

	// Move down cursor to far left of new line
	m_col = 0; m_row++; m_cur_line++;
}

// 1. O(L) time, L = length of line of text
// 2. O(L1+L2), L1 = current line of text, L2 = next line of text
void StudentTextEditor::backspace() {
	// 1. If not at first char, remove char, move column down
	if (m_col > 0) {
		// Track deleted char
		const char deletedCh = (*m_cur_line)[m_col - 1];
		(*m_cur_line).erase(m_col - 1, 1);
		m_col--;

		// Add to undo stack as DELETE after processing
		getUndo()->submit(Undo::Action::DELETE, m_row, m_col, deletedCh);
	}
	// 2. If col == 0 and not at first line, merge with previous line
	else if (m_col == 0 && m_row > 0) {
		mergePrevLine(true);
	}
	// 3. If col == 0 and first line, do nothing
	else {}
}

// O(L) time, L = length of current line

void StudentTextEditor::insert(char ch) {
	// Inserting tab should result in 4 spaces, move pos by 4
	if (ch == '\t') {
		(*m_cur_line).insert(m_col, "    ");
		for (int x = 0; x < 4; x++) {
			m_col++;
			// Add character with column AFTER char was inserted
			getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ch);
		}
	}
	// Move characters to right of editing pos by one
	else {
		(*m_cur_line).insert(m_col, string(1, ch));
		m_col++;
		// Add character with column AFTER char was inserted
		getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ch);
	}
}

// O(L) time, L = length of line of text
// TO-DO: Do we literally need to insert a line break?
void StudentTextEditor::enter() {
	splitLine(true);
}

// O(1) time
void StudentTextEditor::getPos(int& row, int& col) const {
	// Set row and column to editor's current editing position
	row = m_row; col = m_col;
}

// O(oldR + abs(row num - start row) + numRows * L), where L = average line length
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

// Runtime depends on nature of undo operation
void StudentTextEditor::undo() {
	string text; int row, col, count;

	// Pass vars by reference, get Action type back
	Undo::Action a = getUndo()->get(row, col, count, text);
	if (a == Undo::Action::ERROR) return; // Return immediately if error

	// Synchronize line and m_row with undo row
	// If undo is further up the page
	while (row < m_row) {
		m_cur_line--;
		m_row--;
	}
	// If undo is further down the page
	while (row > m_row) {
		m_cur_line++;
		m_row++;
	}

	// Check action type
	switch (a) {
	// Reinsert deleted chars
	case (Undo::Action::INSERT): {
		(*m_cur_line).insert(col, text);
		m_col = col;
		break;
	}

	// Undo typed/inserted chars
	case Undo::Action::DELETE: {
		// col is location of cursor after insertion
		// col-1 is location of initial char
		(*m_cur_line).erase(col - 1, count);
		m_col = col - 1;
		break;
	}

	// Undo a line merge
	case Undo::Action::SPLIT: {
		m_col = col; // Go to row and column of merge
		splitLine(false); // Split the line there
		break;
	}

	// Undo enter()
	case Undo::Action::JOIN: {
		m_col = col; // Go to row and column of split
		mergeNextLine(false); // Merge the next line into current
		break;
	}

	default: break;
	}
}
