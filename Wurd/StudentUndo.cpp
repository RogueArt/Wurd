#include "StudentUndo.h"

using namespace std;

// O(1)
Undo* createUndo() {
	// Initialize current editing position to (0, 0) = (row, col)
	// Do any other initialization
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
	// Make sure stack isn't empty before checking previous action
	if (!m_changes.empty()) {
		// Check if previous action can be batched
		Change& prevChange = m_changes.top();
		bool sameAction = prevChange.a == action;
		bool sameRow = prevChange.row == row;

		// If both are backspace/delete
		if (sameAction && action == Action::DELETE) {
			// 1. If delete, batch them at same position
			if (sameRow && prevChange.col == col) {
				// Del() deletes left to right, add chars to end
				prevChange.s.push_back(ch);
				return;
			}
			// 2. If backspace, will be one column lower
			else if (sameRow && prevChange.col == col + 1) {
				// Backspace() deletes right to left, adds chars to front
				prevChange.s.insert(0, 1, ch);
				prevChange.col = col; // Update column
				return;
			}
		}

		// If both are insert
		if (sameAction && action == Action::INSERT) {
			// Batch if insertion at adjacent spaces
			// Initial insert is always a lower column
			if (sameRow && prevChange.col < col) {
				prevChange.s.push_back(ch);
				return;
			}
		}
	}

	// If stack is empty or can't be batched, simply push change and return
	return m_changes.push(Change(action, row, col, std::string(1, ch)));
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
	// If stack empty, return error
	if (m_changes.empty()) return Action::ERROR;

	// Grab latest change from stack
	Change& latestChange = m_changes.top();
	Action latestAction = latestChange.a;

	// Set correct row and column values as values of latest change
	row = latestChange.row; col = latestChange.col;
	if (latestChange.a) col -= 1;

	// Count is 1 for all actions other than DELETE
	count = latestAction == Action::DELETE ? latestChange.s.size() : 1;

	// Text string empty for all other than INSERT
	text = latestAction == Action::INSERT ? latestChange.s : "";

	// // Delete latestChange from stack
	// delete latestChange;
	m_changes.pop();

	// Return opposite of original action
	switch (latestAction) {
	case Action::INSERT: return Action::DELETE;
	case Action::DELETE: return Action::INSERT;

	case Action::SPLIT: return Action::JOIN;
	case Action::JOIN: return Action::SPLIT;

	default: return Action::ERROR;
	}
}

// O(N), N = number of elements in undo stack
void StudentUndo::clear() {
	// Delete all items from stack
	while (!m_changes.empty()) {
		m_changes.pop();
	}
}
