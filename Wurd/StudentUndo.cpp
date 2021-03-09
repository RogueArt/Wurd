#include "StudentUndo.h"

// O(1)
Undo* createUndo()
{
	// Initialize current editing position to (0, 0) = (row, col)
	// Do any other initialization
	return new StudentUndo;
}

// TO-DO: Is concatenating a char to string O(1)/
void StudentUndo::submit(const Action action, int row, int col, char ch) {
	// Check if previous action can be batched
	Change* prevChange = m_changes.top();
	bool sameAction = prevChange->a == action;

	// If it has the same row, column, and action, can be batched
	if (sameAction) {
		bool sameRow = prevChange->row == row;
		switch (action) {
			case Action::DELETE: {
				// 1. Batch at the same column & row
				if (sameRow && prevChange->col == col) {
					prevChange->s += ch;
				}

				// TO-DO: This probably doesn't work
				// 2. Batch backspacing at same row, column one less
				else if (sameRow && prevChange->col == col - 1) {
					prevChange->row = row;
					prevChange->col = col;
					prevChange->s += ch;
				}
			}
			break;
			
			case Action::INSERT: {
				// Batch if insertion at adjacent spaces
				if (sameRow && prevChange->col == col + 1) {
					prevChange->row = row;
					prevChange->col = col;
					prevChange->s += ch;
				}
			}
			break;

			default:
				// Push it as a new/separate change
				m_changes.push(new Change(action, row, col, std::string(1, ch)));
			break;
		}
	}	
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
  // If stack empty, return error
	if (m_changes.empty()) return Action::ERROR;

	// Grab latest change from stack
	Change* latestChange = m_changes.top();
	m_changes.pop();

	// Set passed in values to values of latest change
	row = latestChange->row;
	col = latestChange->col;

	// Count is 1 for all actions other than DELETE
	count = latestChange->a == Action::DELETE
		? latestChange->s.size() : 1;

	// Text string empty for all other than INSERT
	text = latestChange->s;
}

// O(N), N = number of elements in undo stack
void StudentUndo::clear() {
	// Delete all items from stack
	while (!m_changes.empty()) {
		Change* c = m_changes.top();
		delete c;
		m_changes.pop();
	}
}

// TODO

// switch (action) {
// 	case Undo::Action::INSERT: 


// 		break;
// 	case Undo::Action::DELETE: break;
// 	case Undo::Action::JOIN: break;
// 	case Undo::Action::SPLIT: break;
// 	default: break;
// }


	// 


		// If not, pop off stack, and set cur_action as popped action

		// If SPLIT or JOIN, terminate immediately

		// If INSERT, BACKSPACE, or DELETE
		// Loop, peek stack and see if action is same
		// If they are, then 

			// TODO