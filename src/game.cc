#include "game.h"

////////////////////////////////////////////////////////////

Game::Game(Char order) {
  NEXT_CELL_STATES = vector<vector<CellState>>(2,
    vector<CellState>(9, FALSE));
  for (Char j = LOWER_BIRTH; j <= UPPER_BIRTH; j++) {
    NEXT_CELL_STATES[0][j] = TRUE;
  }
  for (Char j = LOWER_SURVIVAL; j <= UPPER_SURVIVAL; j++) {
    NEXT_CELL_STATES[1][j] = TRUE;
  }

  ORDER = order;
  NEXT_ORDER = ORDER - 2;

  DOMAIN_SIZE = getGridStateCount(ORDER);
  CODOMAIN_SIZE = getGridStateCount(NEXT_ORDER);

  image = Image(CODOMAIN_SIZE, FALSE);

  grid = Grid(ORDER, Row(ORDER, FALSE));
  nextGrid = Grid(NEXT_ORDER, Row(NEXT_ORDER, FALSE));
}

Float Game::getImageProportion() {
  Float imageProportion = 1.0 * getImageSize() / CODOMAIN_SIZE;
  cout << "Image proportion: " << imageProportion << ".\n";
  return imageProportion;
}

Long Game::getImageSize() {
  setImage();
  Long imageSize = 0;
  for (Long nextGridStateIndex = 0;
      nextGridStateIndex < CODOMAIN_SIZE;
      nextGridStateIndex++) {
    if (image[nextGridStateIndex]) {
      imageSize++;
    } else {
      cout << "Not in image: next grid state index " <<
        nextGridStateIndex << ".\n";
    }
  }
  cout << "Codomain size: " << CODOMAIN_SIZE << ".\n";
  cout << "Image size: " << imageSize << ".\n";
  return imageSize;
}

void Game::setImage() {
  cout << "Started setting image.\n";
  auto startTime = chrono::system_clock::now();
  for (Long gridStateIndex = 0;
      gridStateIndex < DOMAIN_SIZE; gridStateIndex++) {
    if (!(gridStateIndex & COUT_PERIOD)) {
      Float percent = 100.0 * gridStateIndex / DOMAIN_SIZE;
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
    image[getNextGridStateIndex(gridStateIndex)] = TRUE;
  }
  auto endTime = chrono::system_clock::now();
  auto totalElapsedTime = chrono::duration_cast
    <chrono::seconds>(endTime - startTime).count();
  cout << "Ended setting image after "
    << totalElapsedTime << " seconds.\n";
}

Long Game::getNextGridStateIndex(Long gridStateIndex) {
  setGrid(gridStateIndex);
  setNextGrid();
  return getGridStateIndex(nextGrid);
}

void Game::setGrid(Long gridStateIndex) {
  for (Char ri = 0; ri < ORDER; ri++) {
    for (Char ci = 0; ci < ORDER; ci++) {
      Char leastBit = gridStateIndex & 1;
      grid[ri][ci] = leastBit;
      gridStateIndex >>= 1;
    }
  }
}

void Game::setNextGrid() {
  for (Char ri = 1; ri < ORDER - 1; ri++) {
    for (Char ci = 1; ci < ORDER - 1; ci++) {
      nextGrid[ri - 1][ci - 1] = getNextCellState(ri, ci);
    }
  }
}

CellState Game::getNextCellState(Char rowIndex, Char columnIndex) {
  Char i = getCellState(rowIndex, columnIndex);
  Char j = getAliveNeighborCount(rowIndex, columnIndex);
  return NEXT_CELL_STATES[i][j];
}

Char Game::getAliveNeighborCount(Char rowIndex, Char columnIndex) {
  Char count = 0;
  char deltas[] = {-1, 0, 1};
  for (char rowDelta : deltas) {
    Char ri = rowIndex + rowDelta;
    for (char columnDelta : deltas) {
      Char ci = columnIndex + columnDelta;
      count += getCellState(ri, ci);
    }
  }
  count -= getCellState(rowIndex, columnIndex);
  return count;
}

CellState Game::getCellState(Char rowIndex, Char columnIndex) {
  return grid[rowIndex][columnIndex];
}

////////////////////////////////////////////////////////////

Long getGridStateCount(Char order) {
  Long gridStateCount = pow(2, pow(order, 2));
  return gridStateCount;
}

Long getGridStateIndex(const Grid &grid) {
  Char order = grid.size();
  Long gridStateIndex = 0;
  Int cellStateIndex = 0;
  for (Char ri = 0; ri < order; ri++) {
    for (Char ci = 0; ci < order; ci++) {
      gridStateIndex += grid[ri][ci] << cellStateIndex;
      cellStateIndex++;
    }
  }
  return gridStateIndex;
}
