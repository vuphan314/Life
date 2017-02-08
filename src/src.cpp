#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>

#include "../cplusplus/src/cartesian_product.cpp"

using namespace std;

////////////////////////////////////////////////////////////

using Order = unsigned short;
using Index = Order;
using Count = unsigned int;

using CellState = bool;
using Row = vector<CellState>;
using Grid = vector<Row>;
// using GridCollection = unordered_set<Grid>;
// using GridCollection = set<Grid>;
using GridCollection = vector<Grid>;

////////////////////////////////////////////////////////////

const Count LOWER_SURVIVAL = 2;
const Count UPPER_SURVIVAL = 3;
const Count LOWER_BIRTH = 3;
const Count UPPER_BIRTH = 3;

////////////////////////////////////////////////////////////

void setImage(Order order, GridCollection &image);

void setDomain(Order order, GridCollection &domain);

void setNextGrid(const Grid &grid, Grid &nextGrid);

CellState getNextCellState(const Grid &grid,
  Index rowIndex, Index columnIndex);

Count getAliveNeighborCount(const Grid &grid,
  Index rowIndex, Index columnIndex);

CellState isAlive(const Grid &grid,
  Index rowIndex, Index columnIndex);

////////////////////////////////////////////////////////////

void setImage(Order order, GridCollection &image) {
  GridCollection domainV{{{}}}, &domain = domainV;
  setDomain(order + 2, domain);
  Grid nextGridV{{}}, &nextGrid = nextGridV;
  for (Grid &grid : domain) {
    setNextGrid(grid, nextGrid);
    if (find(begin(image), end(image), nextGrid) != end(image)) {
      image.push_back(nextGrid);
    }
  }
}

void setDomain(Order order, GridCollection &domain) {
  vector<CellState> statesV{false, true}, &states = statesV;
  vector<Row> rowsV{{}}, &rows = rowsV;
  setCartesianPower(states, order, rows);
  setCartesianPower(rows, order, domain);
}

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
  for (char rowDelta : deltas) {
    Index ri = rowIndex + rowDelta;
    for (char columnDelta : deltas) {
      Index ci = columnIndex + columnDelta;
      if (!(ri == rowIndex && ci == columnIndex) &&
          isAlive(grid, ri, ci)) {
        count++;
      }
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
  // GridCollection imageV{{{}}}, &image = imageV;
  // setImage(1, image);
  GridCollection domainV{{{}}}, &domain = domainV;
  Order order = 2;
  setDomain(order, domain);
  printVectors(domain);
  // Grid gridV{{1, 0, 0, 1}, {1, 0, 1, 0}, {1, 0, 0, 0}}, &grid = gridV,
  //   nextGridV(order, Row(order, -1)), &nextGrid = nextGridV;
}
