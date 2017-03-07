#include "game.h"

////////////////////////////////////////////////////////////

Space::Space(Char order) {
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

  SPACE_SIZE = getSpaceSize(ORDER);
  POST_SPACE_SIZE = getSpaceSize(POST_ORDER);
  PRE_SPACE_SIZE = getSpaceSize(PRE_ORDER);
  EDGE_PRE_SPACE_SIZE = getEdgeSpaceSize(PRE_ORDER);

  image = Image(POST_SPACE_SIZE, FALSE);

  grid = Grid(ORDER, Row(ORDER, FALSE));
  postGrid = Grid(POST_ORDER, Row(POST_ORDER, FALSE));
}

void Space::inspectSpace() {
  setEdgePreImages();
  cout << "Edge pre-space size: " << EDGE_PRE_SPACE_SIZE << ".\n";
  for (Long gridIndex = 0; gridIndex < SPACE_SIZE; gridIndex++) {
    Long rightEdgeFiberSize = rightEdgePreImage[gridIndex].size();
    Float percent = 100.0 * rightEdgeFiberSize / EDGE_PRE_SPACE_SIZE;
    cout << "Grid index" << COUT_WIDTH << gridIndex <<
      COUT_WIDTH << COUT_PRECISION << fixed << percent << "%\n";
  }
}

void Space::showHorizontallyUnjoinableGridPair() {
  setEdgePreImages();
  for (Long leftGridIndex = 0; leftGridIndex < SPACE_SIZE; leftGridIndex++) {
    for (Long rightGridIndex = 0; rightGridIndex < SPACE_SIZE; rightGridIndex++) {
      if (!(areHorizontallyJoinable(leftGridIndex, rightGridIndex))) {
        cout << "Horizontally unjoinable grid pair: left " << leftGridIndex
          << ", right " << rightGridIndex << ".\n";
        return;
      }
    }
  }
  cout << "Every grid pair is horizontally joinable.\n";
}

Bool Space::areHorizontallyJoinable(Long leftGridIndex, Long rightGridIndex) {
    for (Long rightPreEdgeIndex : rightEdgePreImage[leftGridIndex]) {
      for (Long leftPreEdgeIndex : leftEdgePreImage[rightGridIndex]) {
        if (rightPreEdgeIndex == leftPreEdgeIndex) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

void Space::setEdgePreImages() {
  setPreImage();
  rightEdgePreImage = EdgePreImage(SPACE_SIZE, EdgeFiber());
  leftEdgePreImage = EdgePreImage(SPACE_SIZE, EdgeFiber());
  Space preSpace(PRE_ORDER);
  for (Long gridIndex = 0; gridIndex < SPACE_SIZE; gridIndex++) {
    for (Long preGridIndex : preImage[gridIndex]) {
      preSpace.setGrid(preGridIndex);
      Long rightPreEdgeIndex = getRightEdgeIndex(preSpace.grid),
        leftPreEdgeIndex = getLeftEdgeIndex(preSpace.grid);
      rightEdgePreImage[gridIndex].insert(rightPreEdgeIndex);
      leftEdgePreImage[gridIndex].insert(leftPreEdgeIndex);
    }
  }
}

void Space::setPreImage() {
  if (PRE_ORDER > 7) {
     cout << "Too big for std::vector.\n";
  }
  preImage = PreImage(SPACE_SIZE, Fiber());

  cout << "RAM needed: " << getSpaceSize(PRE_ORDER) / pow(2, 30) << "GB.\n";
  if (PRE_ORDER > 5) {
    throw exception();
  }

  cout << "Started setting pre-image.\n";
  Space preSpace(PRE_ORDER);
  for (Long preGridIndex = 0;
      preGridIndex < PRE_SPACE_SIZE;
      preGridIndex++) {
    Long gridIndex =
      preSpace.getPostGridIndex(preGridIndex);
    preImage[gridIndex].push_back(preGridIndex);
  }
  cout << "Ended setting pre-image.\n";
}

Float Space::getImageProportion() {
  Float imageProportion = 1.0 * getImageSize() / POST_SPACE_SIZE;
  cout << "Image proportion: " << imageProportion << ".\n";
  return imageProportion;
}

Long Space::getImageSize() {
  setImage();
  Long imageSize = 0;
  for (Long postGridIndex = 0;
      postGridIndex < POST_SPACE_SIZE;
      postGridIndex++) {
    if (image[postGridIndex]) {
      imageSize++;
    } else {
      cout << "Not in image: post grid state index " <<
        postGridIndex << ".\n";
    }
  }
  cout << "Post-space size: " << POST_SPACE_SIZE << ".\n";
  cout << "Image size: " << imageSize << ".\n";
  return imageSize;
}

void Space::setImage() {
  cout << "Started setting image.\n";
  auto startTime = chrono::system_clock::now();
  for (Long gridIndex = 0;
      gridIndex < SPACE_SIZE; gridIndex++) {
    if (!(gridIndex & COUT_PERIOD)) {
      Float percent = 100.0 * gridIndex / SPACE_SIZE;
      auto currentTime = chrono::system_clock::now();
      auto currentElapsedTime = chrono::duration_cast
        <chrono::seconds>(currentTime - startTime).count();
      Float remainingTime = currentElapsedTime *
        (100.0 / percent - 1) / 3600;
      cout << "Processed grid state index" <<
        COUT_WIDTH << gridIndex <<
        COUT_WIDTH << COUT_PRECISION << fixed << percent << "%" <<
        COUT_WIDTH << remainingTime << " hours left.\n";
    }
    image[getPostGridIndex(gridIndex)] = TRUE;
  }
  auto endTime = chrono::system_clock::now();
  auto totalElapsedTime = chrono::duration_cast
    <chrono::seconds>(endTime - startTime).count();
  cout << "Ended setting image after "
    << totalElapsedTime << " seconds.\n";
}

Long Space::getPostGridIndex(Long gridIndex) {
  setGrid(gridIndex);
  setPostGrid();
  return getGridIndex(postGrid);
}

void Space::setGrid(Long gridIndex) {
  for (Char ri = 0; ri < ORDER; ri++) {
    for (Char ci = 0; ci < ORDER; ci++) {
      Char leastBit = gridIndex & 1;
      grid[ri][ci] = leastBit;
      gridIndex >>= 1;
    }
  }
}

void Space::setPostGrid() {
  for (Char ri = 1; ri < ORDER - 1; ri++) {
    for (Char ci = 1; ci < ORDER - 1; ci++) {
      postGrid[ri - 1][ci - 1] = getPostCellState(ri, ci);
    }
  }
}

CellState Space::getPostCellState(Char rowIndex, Char columnIndex) {
  Char i = getCellState(rowIndex, columnIndex);
  Char j = getAliveNeighborCount(rowIndex, columnIndex);
  return RULE_MATRIX[i][j];
}

Char Space::getAliveNeighborCount(Char rowIndex, Char columnIndex) {
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

CellState Space::getCellState(Char rowIndex, Char columnIndex) {
  return grid[rowIndex][columnIndex];
}

////////////////////////////////////////////////////////////

Long getEdgeSpaceSize(Char order) {
  return pow(2, 2 * order);
}

Long getSpaceSize(Char order) {
  return pow(2, pow(order, 2));
}

Long getRightEdgeIndex(const Grid &grid) {
  Char order = grid.size();
  return getSubGridIndex(grid, 0, order, order - 2, order);
}

Long getLeftEdgeIndex(const Grid &grid) {
  Char order = grid.size();
  return getSubGridIndex(grid, 0, order, 0, 2);
}

Long getGridIndex(const Grid &grid) {
  Char order = grid.size();
  return getSubGridIndex(grid, 0, order, 0, order);
}

Long getSubGridIndex(const Grid &grid,
    Char startRow, Char endRow,
    Char startColumn, Char endColumn) {
  Long subGridIndex = 0;
  Int cellIndex = 0;
  for (Char ri = startRow; ri < endRow; ri++) {
    for (Char ci = startColumn; ci < endColumn; ci++) {
      subGridIndex += grid[ri][ci] << cellIndex;
      cellIndex++;
    }
  }
  return subGridIndex;
}
