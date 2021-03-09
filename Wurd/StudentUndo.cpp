#include "StudentUndo.h"

// O(1)
Undo* createUndo()
{
	// Initialize current editing position to (0, 0) = (row, col)
	// Do any other initialization
	return new StudentUndo;
}

// 
void StudentUndo::submit(const Action action, int row, int col, char ch) {
	m_changes.push(Change(action, row, col, ch));
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
  // If stack empty, return error
	if (m_changes.empty()) return Action::ERROR;

	// Grab latest change from stack
	Change latestChange = m_changes.top();
	m_changes.pop();

	// Perform assignment related to action type
	switch (latestChange.a) {
		case Undo::Action::SPLIT:
		case Undo::Action::JOIN: {
			
		}
		break;

		default: break;
	}


		// If not, pop off stack, and set cur_action as popped action

		// If SPLIT or JOIN, terminate immediately

		// If INSERT, BACKSPACE, or DELETE
		// Loop, peek stack and see if action is same
		// If they are, then 
		
		  // TODO
}

// O(N), N = number of elements in undo stack
void StudentUndo::clear() {
	// Delete all items from stack
	while (!m_changes.empty()) {
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
