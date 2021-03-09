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
	// TODO

	switch (action) {
		case Undo::Action::INSERT: 
			
		
			break;
		case Undo::Action::DELETE: break;
		case Undo::Action::JOIN: break;
		case Undo::Action::SPLIT: break;
		default: break;
	}


}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
    return Action::ERROR;  // TODO
}

// O(N), N = number of elements in undo stack
void StudentUndo::clear() {
	// Delete all items from stack
	while (!m_undos.empty()) {
		m_undos.pop();
	}
}
