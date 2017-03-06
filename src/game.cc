#include "game.h"

////////////////////////////////////////////////////////////

Game::Game(Char order) {
  RULE_MATRIX = vector<vector<CellState>>(2,
    vector<CellState>(9, FALSE));
  for (Char j = LOWER_BIRTH; j <= UPPER_BIRTH; j++) {
    RULE_MATRIX[0][j] = TRUE;
  }
  for (Char j = LOWER_SURVIVAL; j <= UPPER_SURVIVAL; j++) {
    RULE_MATRIX[1][j] = TRUE;
  }

  ORDER = order;
  POST_ORDER = ORDER - 2;
  PRE_ORDER = ORDER + 2;

  SPACE_SIZE = getGridStateCount(ORDER);
  POST_SPACE_SIZE = getGridStateCount(POST_ORDER);

  image = Image(POST_SPACE_SIZE, FALSE);

  grid = Grid(ORDER, Row(ORDER, FALSE));
  postGrid = Grid(POST_ORDER, Row(POST_ORDER, FALSE));
}

void Game::setPreImage() {
  if (ORDER > 5) {
    cout << "Order-" << static_cast<Int>(ORDER) <<
      " pre-image is too big for std::vector.\n";
  }
  preImage = PreImage(SPACE_SIZE, Fiber());

  cout << "Will use " <<
    getGridStateCount(PRE_ORDER) / pow(2, 30) <<
    "GB of RAM.\n";

  cout << "Started setting pre-image.\n";
  Game preGame(PRE_ORDER);
  for (Long preGridStateIndex = 0;
      preGridStateIndex < preGame.SPACE_SIZE;
      preGridStateIndex++) {
    Long gridStateIndex =
      preGame.getPostGridStateIndex(preGridStateIndex);
    preImage[gridStateIndex].push_back(preGridStateIndex);
  }
  cout << "Ended setting pre-image.\n";
}

Float Game::getImageProportion() {
  Float imageProportion = 1.0 * getImageSize() / POST_SPACE_SIZE;
  cout << "Image proportion: " << imageProportion << ".\n";
  return imageProportion;
}

Long Game::getImageSize() {
  setImage();
  Long imageSize = 0;
  for (Long postGridStateIndex = 0;
      postGridStateIndex < POST_SPACE_SIZE;
      postGridStateIndex++) {
    if (image[postGridStateIndex]) {
      imageSize++;
    } else {
      cout << "Not in image: post grid state index " <<
        postGridStateIndex << ".\n";
    }
  }
  cout << "Post-space size: " << POST_SPACE_SIZE << ".\n";
  cout << "Image size: " << imageSize << ".\n";
  return imageSize;
}

void Game::setImage() {
  cout << "Started setting image.\n";
  auto startTime = chrono::system_clock::now();
  for (Long gridStateIndex = 0;
      gridStateIndex < SPACE_SIZE; gridStateIndex++) {
    if (!(gridStateIndex & COUT_PERIOD)) {
      Float percent = 100.0 * gridStateIndex / SPACE_SIZE;
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
    image[getPostGridStateIndex(gridStateIndex)] = TRUE;
  }
  auto endTime = chrono::system_clock::now();
  auto totalElapsedTime = chrono::duration_cast
    <chrono::seconds>(endTime - startTime).count();
  cout << "Ended setting image after "
    << totalElapsedTime << " seconds.\n";
}

Long Game::getPostGridStateIndex(Long gridStateIndex) {
  setGrid(gridStateIndex);
  setPostGrid();
  return getGridStateIndex(postGrid);
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

void Game::setPostGrid() {
  for (Char ri = 1; ri < ORDER - 1; ri++) {
    for (Char ci = 1; ci < ORDER - 1; ci++) {
      postGrid[ri - 1][ci - 1] = getPostCellState(ri, ci);
    }
  }
}

CellState Game::getPostCellState(Char rowIndex, Char columnIndex) {
  Char i = getCellState(rowIndex, columnIndex);
  Char j = getAliveNeighborCount(rowIndex, columnIndex);
  return RULE_MATRIX[i][j];
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
  return pow(2, pow(order, 2));
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
