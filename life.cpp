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

void setImage(Order order, GridCollection &image);

void setNextGrid(const Grid &grid, Grid &nextGrid);

CellState getNextCellState(const Grid &grid, Index rowIndex, Index columnIndex);

Count getAliveNeighborCount(const Grid &grid, Index rowIndex, Index columnIndex);

CellState isAlive(const Grid &grid, Index rowIndex, Index columnIndex);

////////////////////////////////////////////////////////////

Count getAliveNeighborCount(const Grid &grid, Index rowIndex, Index columnIndex) {
  Count count = 0;
  int deltas[] = {-1, 0, 1};
  for (int delta: deltas) {
    Index m = rowIndex + delta;
    Index n = columnIndex + delta;
    if (!(m == rowIndex && n == columnIndex)) {
      if (isAlive(grid, m, n)) {
        count++;
      }
    }
  }
  return count;
}
a
CellState isAlive(const Grid &grid, Index rowIndex, Index columnIndex) {
  return grid[rowIndex][columnIndex];
}

////////////////////////////////////////////////////////////

int main() {
  cout << "hi" << endl;
}
