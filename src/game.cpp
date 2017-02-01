#include <set>
#include <vector>

using namespace std;

////////////////////////////////////////////////////////////

bool isWithin(int mid, int left, int right) {
    return left <= mid && mid && right;
}

////////////////////////////////////////////////////////////

class Cell {
public:
    int rowIndex;
    int columnIndex;
    Cell(int m, int n) {
        rowIndex = m;
        columnIndex = n;
    }
    // int getRowIndex() {
    //     return rowIndex;
    // }
    // int getColumnIndex() {
    //     return columnIndex;
    // }
};

class CellCollection {
public:
    set<Cell> *cells;
    CellCollection (set<Cell> *c) {
        cells = c;
    }
    // set<Cell> *getCells() {
    //     return cells;
    // }
};

class CellState {
public:
    bool alive;
    CellState(bool a) {
        alive = a;
    }
    // bool isAlive() {
    //     return alive;
    // }
};

class Row {
public:
    vector<CellState> *cellStates;
    Row(vector<CellState> *c) {
        cellStates = c;
    }
    // vector<CellState> *getCellStates() {
    //     return cellStates;
    // }
};

class Grid {
public:
    vector<Row> *rows;
    Grid(vector<Row> *r) {
        rows = r;
    }
    // vector<Row> *getRows() {
    //     return rows;
    // }
};

class GridCollection {
public:
    set<Grid> *grids;
    GridCollection(set<Grid> *g) {
        grids = g;
    }
    // set<Grid> *getGrids() {
    //     return grids;
    // }
};

////////////////////////////////////////////////////////////

class Game {
public:
    int edgeSize, lowerSurvival, upperSurvival, lowerBirth, upperBirth;
    Game(int e) {
        edgeSize = e;
        lowerSurvival = 2;
        upperSurvival = 3;
        lowerBirth = 3;
        upperBirth = 3;
    }
    CellCollection *getNeighbors(Cell cell) {
        set<int> diffs = {-1, 0, 1};
        set<int> rowIndicies, columnIndicies;
        for (int diff : diffs) {
            rowIndicies.insert(cell.rowIndex + diff);
            columnIndicies.insert(cell.columnIndex + diff);
        }
        set<Cell> cells;
        for (int rowIndex : rowIndicies) {
            for (int columnIndex : columnIndicies) {
                Cell cell(rowIndex, columnIndex);
                cells.insert(cell);
            }
        }
        // cells.erase(cell);
        // CellCollection cellCollection;


        return nullptr;
    }
};

////////////////////////////////////////////////////////////

int main() {
    cout << "hi";
}
