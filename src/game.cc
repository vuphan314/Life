#include "game.h"

////////////////////////////////////////////////////////////

Long getImageSize(Char order) {
  Char imageOrder = order - 2;
  Image image(getGridStateCount(imageOrder), false);
  setImage(image, order);
  Long imageSize = 0;
  for (bool gridState : image) {
    if (gridState) {
      imageSize++;
    }
  }
  return imageSize;
}

void setImage(Image &image, Char order) {
  Char imageOrder = order - 2;
  Grid grid(order, Row(order, false)),
    nextGrid(imageOrder, Row(imageOrder, false));
  Long gridStateCount = getGridStateCount(order);
  for (Long gridStateIndex = 0;
      gridStateIndex < gridStateCount; gridStateIndex++) {
    setGrid(grid, gridStateIndex);
    setNextGrid(nextGrid, grid);
    Long nextGridStateIndex = getGridStateIndex(nextGrid);
    image[nextGridStateIndex] = true;
  }
}

Long getGridStateCount(Char order) {
  return pow(2, pow(order, 2));
}

void setGrid(Grid &grid, Long gridStateIndex) {
  Char order = grid.size();
  for (Char ri = 0; ri < order; ri++) {
    for (Char ci = 0; ci < order; ci++) {
      grid[ri][ci] = gridStateIndex & 1;
      gridStateIndex >> 1;
    }
  }
}

Long getGridStateIndex(const Grid &grid) {
  Long gridStateIndex = 0;
  Int cellStateIndex = 0;
  Char order = grid.size();
  for (Char ri = 0; ri < order; ri++) {
    for (Char ci = 0; ci < order; ci++) {
      gridStateIndex += grid[ri][ci] << cellStateIndex;
      cellStateIndex++;
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
    Char rowIndex, Char columnIndex) {
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
    Char rowIndex, Char columnIndex) {
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
    Char rowIndex, Char columnIndex) {
  return grid[rowIndex][columnIndex];
}

////////////////////////////////////////////////////////////
