#include "game.h"

////////////////////////////////////////////////////////////

Game::Game() {
  NEXT_CELL_STATES = vector<vector<CellState>>(2,
    vector<CellState>(9, FALSE));
  for (Char j = LOWER_BIRTH; j <= UPPER_BIRTH; j++) {
    NEXT_CELL_STATES[0][j] = TRUE;
  }
  for (Char j = LOWER_SURVIVAL; j <= UPPER_SURVIVAL; j++) {
    NEXT_CELL_STATES[1][j] = TRUE;
  }
}

Float Game::getImageProportion(Char order) {
  Int codomainSize = getGridStateCount(order - 2);
  Float imageProportion = 1.0 * getImageSize(order) / codomainSize;
  cout << "Image proportion: " << imageProportion << ".\n";
  return imageProportion;
}

Long Game::getImageSize(Char order) {
  Char imageOrder = order - 2;
  Long codomainSize = getGridStateCount(order - 2);
  Image image(codomainSize, FALSE);
  setImage(image, order);
  Long imageSize = 0;
  for (Long gridStateIndex = 0;
      gridStateIndex < codomainSize; gridStateIndex++) {
    if (image[gridStateIndex]) {
      imageSize++;
    } else {
      cout << "Not in image: grid state index " <<
        gridStateIndex << ".\n";
    }
  }
  cout << "Codomain size: " << codomainSize << ".\n";
  cout << "Image size: " << imageSize << ".\n";
  return imageSize;
}

void Game::setImage(Image &image, Char order) {
  cout << "Started setting image.\n";
  auto startTime = chrono::system_clock::now();
  Char imageOrder = order - 2;
  Grid grid(order, Row(order, FALSE)),
    nextGrid(imageOrder, Row(imageOrder, FALSE));
  Long gridStateCount = getGridStateCount(order);
  for (Long gridStateIndex = 0;
      gridStateIndex < gridStateCount; gridStateIndex++) {
    if (!(gridStateIndex & COUT_PERIOD)) {
      Float percent = 100.0 * gridStateIndex / gridStateCount;
      auto currentTime = chrono::system_clock::now();
      auto currentElapsedTime = chrono::duration_cast
        <chrono::seconds>(currentTime - startTime).count();
      Float remainingTime = currentElapsedTime *
        (100.0 / percent - 1) / 3600;
      cout << "Processed grid state index" <<
        COUT_WIDTH << gridStateIndex <<
        COUT_WIDTH << COUT_PRECISION << fixed << percent << "%" <<
        COUT_WIDTH << remainingTime << " hours left.\n";
    }
    setGrid(grid, gridStateIndex);
    setNextGrid(nextGrid, grid);
    Long nextGridStateIndex = getGridStateIndex(nextGrid);
    image[nextGridStateIndex] = TRUE;
  }
  auto endTime = chrono::system_clock::now();
  auto totalElapsedTime = chrono::duration_cast
    <chrono::seconds>(endTime - startTime).count();
  cout << "Ended setting image after "
    << totalElapsedTime << " seconds.\n";
}

Long Game::getGridStateCount(Char order) {
  Long gridStateCount = pow(2, pow(order, 2));
  return gridStateCount;
}

void Game::setGrid(Grid &grid, Long gridStateIndex) {
  Char order = grid.size();
  for (Char ri = 0; ri < order; ri++) {
    for (Char ci = 0; ci < order; ci++) {
      Char leastBit = gridStateIndex & 1;
      grid[ri][ci] = leastBit;
      gridStateIndex >>= 1;
    }
  }
}

Long Game::getGridStateIndex(const Grid &grid) {
  Long gridStateIndex = 0;
  Int cellStateIndex = 0;
  Char order = grid.size();
  for (Char ri = 0; ri < order; ri++) {
    for (Char ci = 0; ci < order; ci++) {
      gridStateIndex += grid[ri][ci] << cellStateIndex;
      cellStateIndex++;
    }
  }
  return gridStateIndex;
}

void Game::setNextGrid(Grid &nextGrid, const Grid &grid) {
  Char order = grid.size();
  for (Char ri = 1; ri < order - 1; ri++) {
    for (Char ci = 1; ci < order - 1; ci++) {
      CellState nextState = getNextCellState(grid, ri, ci);
      nextGrid[ri - 1][ci - 1] = nextState;
    }
  }
}

CellState Game::getNextCellState(const Grid &grid,
    Char rowIndex, Char columnIndex) {
  Char i = getCellState(grid, rowIndex, columnIndex);
  Char j = getAliveNeighborCount(grid,
    rowIndex, columnIndex);
  return NEXT_CELL_STATES[i][j];
}

Char Game::getAliveNeighborCount(const Grid &grid,
    Char rowIndex, Char columnIndex) {
  Char count = 0;
  char deltas[] = {-1, 0, 1};
  for (char rowDelta : deltas) {
    Char ri = rowIndex + rowDelta;
    for (char columnDelta : deltas) {
      Char ci = columnIndex + columnDelta;
      count += getCellState(grid, ri, ci);
    }
  }
  count -= getCellState(grid, rowIndex, columnIndex);
  return count;
}

CellState Game::getCellState(const Grid &grid,
    Char rowIndex, Char columnIndex) {
  return grid[rowIndex][columnIndex];
}
