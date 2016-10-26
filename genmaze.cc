# include "maze.cc"
# include <vector>
# include <cstdlib>   // needed for srand() and rand() functions
# include <ctime> 
# include <string>

using namespace std;

// Helper function to add viable direction to path
void addDirectionOption(const Maze &maze, const Location &current,
                        Direction dir, vector<Direction> &v) {
    Location neighbor = maze.getNeighborCell(current.row, current.col,
                                                dir);
    if (! (maze.isVisited(neighbor.row, neighbor.col))){
        v.push_back(dir);
    }
}

// Usage function
void usage() {
    cerr << "usage: ./genmaze numRows numCols" << endl;
    cerr << "       numRows is the number of rows in the maze" << endl;
    cerr << "       numCols is the number of columns in the maze" << endl;
}

int main(int argc, char **argv) {

    srand(time(NULL));

    // If wrong number of arguments, print usage statement
    if (argc != 3) {
        usage();
        exit(1);
    }

    // From command line
    int numRows = (int) atoi(argv[1]);
    int numCols = (int) atoi(argv[2]);

    // Variables
    Maze maze(numRows, numCols);
    vector<Location> path;
    path.reserve(numRows * numCols);

    maze.clear();
    maze.setAllWalls();
    maze.setStart(0, 0);
    maze.setEnd(numRows-1, numCols-1);

    maze.setVisited(0, 0);
    path.push_back(maze.getStart());

    while (! path.empty()) {
        Location current = path.back(); // last element in path

        // If the cell we are looking at is the end of the maze, skip the
        // cell so that the current path will not go through the end-point.
        if (current == maze.getEnd()) {
            path.pop_back(); // remove last element
            continue;
        }

            
        // Generate vector of potential directions that can be taken 
        vector<Direction> options;
        options.reserve(4);

        // if not top row
        if (current.row > 0) {
            addDirectionOption(maze, current, Direction::NORTH, options);
        }
        // if not bottom row
        if (current.row < numRows - 1) {
            addDirectionOption(maze, current, Direction::SOUTH, options);
        }
        // if not left row
        if (current.col > 0) {
            addDirectionOption(maze, current, Direction::WEST, options);
        }
        // if not right row
        if (current.col < numCols - 1) {
            addDirectionOption(maze, current, Direction::EAST, options);
        }
        // if no viable path
        if (options.empty()){
            path.pop_back();
            continue;
        }
        
        // choose random direction
        int index = rand() % options.size();
        Direction dir = options[index];
        // clear wall in that direction
        maze.clearWall(current.row, current.col, dir);

        // move to next cell
        Location next = maze.getNeighborCell(current.row,current.col, dir);
        maze.setVisited(next.row, next.col);
        path.push_back(next);
    }
    // Print finished maze
    maze.print(cout);
}