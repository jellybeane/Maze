# include "maze.hh"
# include <cassert>

using namespace std;

// Initialize a new maze of size rows x cols
Maze::Maze(int rows, int cols) {
	// Cell coordinates
	numRows = rows;
	numCols = cols;
	// Expanded coordinates
	cells = new MazeCell[(2*numRows+1) * (2*numCols+1)];
}

// Make a copy of an existing maze object
Maze::Maze(const Maze &m) {
	numRows = m.getNumRows();
	numCols = m.getNumCols();
	start = m.getStart();
	end = m.getEnd();

	cells = new MazeCell[(2*numRows+1) * (2*numCols+1)];

	for (int i = 0; i < (2*numRows+1) * (2*numCols+1); i++) {
		cells[i] = m.getExpanded(i);
	}
}

// Maze destructor
Maze::~Maze() {
	delete[] cells;
}

// Maze assignment operator
Maze & Maze::operator=(const Maze &m) {
	if (this != &m) {
		numRows = m.getNumRows();
		numCols = m.getNumCols();
		start = m.getStart();
		end = m.getEnd();

		delete[] cells;
		cells = new MazeCell[(2*numRows+1) * (2*numCols+1)];

		for (int i = 0; i < (2*numRows+1) * (2*numCols+1); i++) {
			cells[i] = m.getExpanded(i);
		}
	}
	return *this;
}


// Returns the number of rows in the maze
int Maze::getNumRows() const {
	return numRows;
}

// Returns the number of columns in the maze
int Maze::getNumCols() const {
	return numCols;
}


// Returns the starting point in the maze
Location Maze::getStart() const {
	return start;
}

// Sets the starting point in the maze    
void Maze::setStart(int row, int col) {
	assert(row >= 0 && row < numRows);
	assert(col >= 0 && col < numCols);
	start = Location(row, col);
}


// Returns the ending point in the maze    
Location Maze::getEnd() const {
	return end;
}

// Sets the ending point in the maze
void Maze::setEnd(int row, int col) {
	assert(row >= 0 && row < numRows);
	assert(col >= 0 && col < numCols);
	end = Location(row, col);
}


// Sets all cells and walls to be empty, so that the maze is
// completely cleared
void Maze::clear() {
	for (int i =0; i < (2*numRows+1) * (2*numCols+1); i++) {
		cells[i] = MazeCell::EMPTY;
	}
}

// Places a wall at every location that can be a wall in the maze
void Maze::setAllWalls() {
	for (int i = 0; i < (2*numRows+1) * (2*numCols+1); i++) {
		// Every location that can be a wall should have odd expanded index
		if (i % 2 == 1) {
			cells[i] = MazeCell::WALL;
		}
	}
}

// Returns the value of the specified
MazeCell Maze::getCell(int cellRow, int cellCol) const {
	assert(cellRow >= 0 && cellRow < numRows);
	assert(cellCol >= 0 && cellCol < numCols);
	Location arraycoord = getCellArrayCoord(cellRow, cellCol);
	int index = getArrayIndex(arraycoord);
	return cells[index];
}

void Maze::setCell(int cellRow, int cellCol, MazeCell val) {
	assert(cellRow >= 0 && cellRow < numRows);
	assert(cellCol >= 0 && cellCol < numCols);
	Location arraycoord = getCellArrayCoord(cellRow, cellCol);
	int index = getArrayIndex(arraycoord);
	cells[index] = val;
}

// Like getCell, but also includes walls
MazeCell Maze::getExpanded(int i) const {
	assert(i >= 0 && i< (2*numRows+1) * (2*numCols+1));
	return cells[i];
}

// Returns the cell-coordinates of the neighboring cell in the specified
// direction.  Trips an assertion if the given cell has no neighbor in the
// specified direction (e.g. the NORTH neighbor of cell (0,5)).
Location Maze::getNeighborCell(int cellRow, int cellCol,
                         Direction direction) const {
	int row = cellRow;
	int col = cellCol;
	if (direction == Direction::NORTH) {
		assert(cellRow > 0);
		row--;
	}
	else if (direction == Direction::SOUTH) {
		assert(cellRow < numRows - 1);
		row++;
	}
	else if (direction == Direction::WEST) {
		assert(cellCol > 0);
		col--;
	}
	else {
		assert(cellCol < numCols - 1);
		col++;
	}
	Location neighbor(row, col);
	return neighbor;
}


// Returns true if there is a wall in the specified direction from the
// given cell, false otherwise
bool Maze::hasWall(int cellRow, int cellCol, Direction direction) const {
	assert(cellRow >= 0 && cellRow < numRows);
	assert(cellCol >= 0 && cellCol < numCols);
	Location arraycoord = getWallArrayCoord(cellRow, cellCol, direction);
	int index = getArrayIndex(arraycoord);
	assert(cells[index] == MazeCell::WALL || cells[index] == MazeCell::EMPTY);
	return (cells[index] == MazeCell::WALL);
}

// Puts a wall on the specified side of the given cell
void Maze::setWall(int cellRow, int cellCol, Direction direction) {
	assert(cellRow >= 0 && cellRow < numRows);
	assert(cellCol >= 0 && cellCol < numCols);
	Location arraycoord = getWallArrayCoord(cellRow, cellCol, direction);
	int index = getArrayIndex(arraycoord);
	assert(cells[index] == MazeCell::WALL || cells[index] == MazeCell::EMPTY);
	cells[index] = MazeCell::WALL;
}

// Removes a wall on the specified side of the given cell
void Maze::clearWall(int cellRow, int cellCol, Direction direction) {
	assert(cellRow >= 0 && cellRow < numRows);
	assert(cellCol >= 0 && cellCol < numCols);
	Location arraycoord = getWallArrayCoord(cellRow, cellCol, direction);
	int index = getArrayIndex(arraycoord);
	assert(cells[index] == MazeCell::WALL || cells[index] == MazeCell::EMPTY);
	cells[index] = MazeCell::EMPTY;
}


// Returns true if the specified maze cell has been visited.
bool Maze::isVisited(int cellRow, int cellCol) const {
	assert(cellRow >= 0 && cellRow < numRows);
	assert(cellCol >= 0 && cellCol < numCols);
	Location arraycoord = getCellArrayCoord(cellRow, cellCol);
	int index = getArrayIndex(arraycoord);
	assert(cells[index] == MazeCell::VISITED || cells[index] == MazeCell::EMPTY);
	return (cells[index] == MazeCell::VISITED);
}

// Changes the cell's value to VISITED
void Maze::setVisited(int cellRow, int cellCol) {
	assert(cellRow >= 0 && cellRow < numRows);
	assert(cellCol >= 0 && cellCol < numCols);
	Location arraycoord = getCellArrayCoord(cellRow, cellCol);
	int index = getArrayIndex(arraycoord);
	assert(cells[index] == MazeCell::VISITED || cells[index] == MazeCell::EMPTY);
	cells[index] = MazeCell::VISITED;
}


// Outputs the maze using simple ASCII-art to the specified output stream.
// The output format is as follows, using the example maze from the
// assignment write-up.  (The text to the right of the maze is purely
// explanatory, and you don't need to output it.)
//
// 3 4                 (# of rows and then # of columns)
// +---+---+---+---+   (each cell is 3 spaces wide, with a + at each corner)
// | S     |       |   (walls indicated by --- or |)
// +---+   +   +   +   (start indicated by S, end indicated by E)
// |   |   |   |   |
// +   +   +   +   +
// |           | E |
// +---+---+---+---+
void Maze::print(ostream &os) const {
	os << numRows << " " << numCols << endl;

	// Top walls
	os << "+";
	for (int j = 0; j < numCols; j++){
		os << "---+";
	}
	os << endl;

	for (int i = 0; i < numRows; i++) {
		// Maze cells
		os << "|";
		for (int j = 0; j < numCols; j++) {
			Location index(i, j);
			// Print cell
			if (index == start) {
				os << " S ";
			}
			else if (index == end) {
				os << " E ";
			}
			else {
				os << "   ";
			}

			// Print wall to the right of cells
			if (hasWall(i, j, Direction::EAST)) {
				os << "|";
			}
			else {
				os << " ";
			}
		}
		os << endl;

		// Print walls below cells
		os << "+";
		for (int j = 0; j < numCols; j++) {
			if (hasWall(i, j, Direction::SOUTH)) {
				os << "---+";
			}
			else {
				os << "   +";
			}
		}
		os << endl;
	}
}

// Private helper functions
// Take 2D expanded coordinates and compute the corresponding 1D array index
int Maze::getArrayIndex(const Location &loc) const {
	int row = loc.row;
	int col = loc.col;
	return row * (2 * numCols + 1) + col;
}

// Returns the expanded coordinates of the specified cell coordinates
Location Maze::getCellArrayCoord(int cellRow, int cellCol) const {
	int row = 2 * cellRow + 1;
	int col = 2 * cellCol + 1;
	return Location(row, col);
}
  
// Returns the expanded coordinates of the wall on a specific side of
// a cell given in cell coordinates
Location Maze::getWallArrayCoord(int cellRow, int cellCol,
                             Direction direction) const {
	int row = 2 * cellRow + 1;
	int col = 2 * cellCol + 1;

	if (direction == Direction::NORTH) {
		row--;
	}
	else if (direction == Direction::SOUTH) {
		row++;
	}
	else if (direction == Direction::WEST) {
		col--;
	}
	else {
		col++;
	}
	return Location(row, col);
}
