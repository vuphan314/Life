#include "game.h"

////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////

void setImage(Char order, GridCollection &image) {
  GridCollection domainV{{{}}}, &domain = domainV;
  setDomain(order + 2, domain);
  Grid nextGridV(order, Row(order, false)), &nextGrid = nextGridV;
  for (Grid &grid : domain) {
    Grid nextGridV(order, Row(order, false)), &nextGrid = nextGridV;
    setNextGrid(grid, nextGrid);
    if (find(begin(image), end(image), nextGrid) != end(image)) {
      image.push_back(nextGrid);
    }
  }
}

void setNextGrid(Grid &nextGrid, const Grid &grid) {
  Char order = grid.size();
  for (Char ri = 1; ri < order - 1; ri++) {
    for (Char ci = 1; ci < order - 1; ci++) {
      CellState nextState = getNextCellState(grid, ri, ci);
      nextGrid[ri - 1][ci - 1] = nextState;
    }
  }
}

CellState getNextCellState(const Grid &grid,
    const Char rowIndex, const Char columnIndex) {
  Char count = getAliveNeighborCount(grid,
    rowIndex, columnIndex);
  if (isAlive(grid, rowIndex, columnIndex)) {
    return LOWER_SURVIVAL <= count &&
      count <= UPPER_SURVIVAL;
  } else {
    return LOWER_BIRTH <= count && count <= UPPER_BIRTH;
  }
}

Char getAliveNeighborCount(const Grid &grid,
    const Char rowIndex, const Char columnIndex) {
  Char count = 0;
  char deltas[] = {-1, 0, 1};
  for (char rowDelta : deltas) {
    Char ri = rowIndex + rowDelta;
    for (char columnDelta : deltas) {
      Char ci = columnIndex + columnDelta;
      if (!(ri == rowIndex && ci == columnIndex) &&
          isAlive(grid, ri, ci)) {
        count++;
      }
    }
  }
  return count;
}

CellState isAlive(const Grid &grid,
    const Char rowIndex, const Char columnIndex) {
  return grid[rowIndex][columnIndex];
}

////////////////////////////////////////////////////////////
