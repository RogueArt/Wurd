#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"

// My includes
#include <stack>

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
	struct Change {
		Action a;
		int row;
		int col;
		std::string s = "";

		// Constructor function
		Change(Action a, int row, int col, std::string s) {
			this->a = a;
			this->row = row;
			this->col = col;
			this->s = s;
		}

	};

	std::stack<Change*> m_changes;
};

#endif // STUDENTUNDO_H_
