#include <unordered_set>
#include <vector>

using namespace std;

////////////////////////////////////////////////////////////

using Order = unsigned short;
using Index = Order;
using Count = unsigned int;

using CellState = bool;
using Grid = vector<vector<CellState>>;
using GridCollection = unordered_set<Grid>;

////////////////////////////////////////////////////////////

const Count LOWER_SURVIVAL = 2;
const Count UPPER_SURVIVAL = 3;
const Count LOWER_BIRTH = 3;
const Count UPPER_BIRTH = 3;

////////////////////////////////////////////////////////////

void setImage(Order order, GridCollection &image);

void setNextGrid(const Grid &grid, Grid &nextGrid);

CellState getNextCellState(const Grid &grid,
  Index rowIndex, Index columnIndex);

Count getAliveNeighborCount(const Grid &grid,
  Index rowIndex, Index columnIndex);

CellState isAlive(const Grid &grid,
  Index rowIndex, Index columnIndex);

////////////////////////////////////////////////////////////

void setNextGrid(const Grid &grid, Grid &nextGrid) {
  Order order = grid.size();
  for (Index ri = 1; ri < order - 1; ri++) {
    for (Index ci = 1; ci < order - 1; ci++) {
      CellState nextState = getNextCellState(grid, ri, ci);
      nextGrid[ri - 1][ci - 1] = nextState;
    }
  }
}

CellState getNextCellState(const Grid &grid,
    Index rowIndex, Index columnIndex) {
  Count count = getAliveNeighborCount(grid,
    rowIndex, columnIndex);
  if (isAlive(grid, rowIndex, columnIndex)) {
    return LOWER_SURVIVAL <= count &&
      count <= UPPER_SURVIVAL;
  } else {
    return LOWER_BIRTH <= count && count <= UPPER_BIRTH;
  }
}

Count getAliveNeighborCount(const Grid &grid,
    Index rowIndex, Index columnIndex) {
  Count count = 0;
  char deltas[] = {-1, 0, 1};
  for (char delta : deltas) {
    Index ri = rowIndex + delta;
    Index ci = columnIndex + delta;
    if (!(ri == rowIndex && ci == columnIndex) &&
        isAlive(grid, ri, ci)) {
      count++;
    }
  }
  return count;
}

CellState isAlive(const Grid &grid,
    Index rowIndex, Index columnIndex) {
  return grid[rowIndex][columnIndex];
}

////////////////////////////////////////////////////////////

int main() {
  cout << "hi" << endl;
}
