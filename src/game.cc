#include "game.h"

////////////////////////////////////////////////////////////

vector<vector<CellState>> RULE_MATRIX; // const after being set

void setRULE_MATRIX() {
  RULE_MATRIX = vector<vector<CellState>>(2,
    vector<CellState>(9, FALSE));
  for (Char j = LOWER_BIRTH; j <= UPPER_BIRTH; j++) {
    RULE_MATRIX[0][j] = TRUE;
  }
  for (Char j = LOWER_SURVIVAL; j <= UPPER_SURVIVAL; j++) {
    RULE_MATRIX[1][j] = TRUE;
  }
}

////////////////////////////////////////////////////////////

Space::Space(Char order) {
  ORDER = order;
  POST_ORDER = ORDER - 2;
  PRE_ORDER = ORDER + 2;

  SPACE_SIZE = getSpaceSize(ORDER);
  POST_SPACE_SIZE = getSpaceSize(POST_ORDER);
  PRE_SPACE_SIZE = getSpaceSize(PRE_ORDER);
  EDGE_PRE_SPACE_SIZE = getEdgeSpaceSize(PRE_ORDER);

  grid = Grid(ORDER, Row(ORDER, FALSE));
  postGrid = Grid(POST_ORDER, Row(POST_ORDER, FALSE));
  preGrid = Grid(PRE_ORDER, Row(PRE_ORDER, FALSE));
}

void Space::inspectSpace() {
  setPreImage();
  cout << "Pre-space size: " << PRE_SPACE_SIZE << ".\n";
  for (Long gridIndex = 0; gridIndex < SPACE_SIZE; gridIndex++) {
    Long fiberSize = preImage[gridIndex].size();
    Float percent = 100.0 * fiberSize / PRE_SPACE_SIZE;
    cout << "Grid index" << COUT_WIDTH << gridIndex <<
      COUT_WIDTH << COUT_PRECISION << fixed << percent << "%\n";
  }
}

Bool Space::isEachGrid3tupleJoinable() {
  setPreImage();
  Long totalCount = pow(SPACE_SIZE, 3), currentCount = 0;
  for (Long gridIndex = 0; gridIndex < SPACE_SIZE;
      gridIndex++, currentCount++) {
    for (Long rightGridIndex = 0; rightGridIndex < SPACE_SIZE;
        rightGridIndex++, currentCount++) {
      for (Long bottomGridIndex = 0; bottomGridIndex < SPACE_SIZE;
          bottomGridIndex++, currentCount++) {
        if (!(currentCount & COUT_PERIOD)) {
          Float percent = 100.0 * currentCount / totalCount;
          cout << "Grid 3-tuple: " << currentCount << "\t" << percent << "%.\n";
        }
        if (!(are3wayJoinable(gridIndex, rightGridIndex, bottomGridIndex))) {
          cout << "Unjoinable grid 3-tuple: " << gridIndex <<
            ", right " << rightGridIndex <<
            ", bottom " << bottomGridIndex << ".\n";
          return FALSE;
        }
      }
    }
  }
  cout << "Each grid 3-tuple is joinable.\n";
  return TRUE;
}

Bool Space::are3wayJoinable(Long gridIndex,
    Long rightGridIndex, Long bottomGridIndex) {
  auto startTime = chrono::system_clock::now();
  Long fiberSize = preImage[gridIndex].size(),
    rightFiberSize = preImage[rightGridIndex].size(),
    bottomFiberSize = preImage[bottomGridIndex].size(),
    totalCount = fiberSize * rightFiberSize * bottomFiberSize,
    currentCount = 0;
  // cout << "Fiber size: " << fiberSize << ".\n" <<
  //   "Right fiber size: " << rightFiberSize << ".\n" <<
  //   "Bottom fiber size: " << bottomFiberSize << ".\n" <<
  //   "Total count: " << totalCount << ".\n";
  for (Long preGridIndex : preImage[gridIndex]) {
    currentCount++;
    setGrid(preGrid, preGridIndex);
    Long right = getRightEdgeIndex(preGrid),
      bottom = getBottomEdgeIndex(preGrid);
    for (Long rightPreGridIndex : preImage[rightGridIndex]) {
      currentCount++;
      setGrid(preGrid, rightPreGridIndex);
      Long left = getLeftEdgeIndex(preGrid);
      if (right == left) {
        for (Long bottomPreGridIndex : preImage[bottomGridIndex]) {
          currentCount++;
          setGrid(preGrid, bottomPreGridIndex);
          Long top = getTopEdgeIndex(preGrid);
          if (bottom == top) {
            return TRUE;
          }
        }
      } else {
        currentCount += bottomFiberSize;
      }
      if (currentCount > COUNT_CUTOFF) {
        cout << "Skipped: " << gridIndex <<
          ", right " << rightGridIndex <<
          ", bottom " << bottomGridIndex <<
          " (total count: " << totalCount << ").\n";
          return TRUE;
      }
      // if (!(currentCount & COUT_PERIOD)) {
      //   Float percent = 100.0 * currentCount / totalCount;
      //   auto currentTime = chrono::system_clock::now();
      //   auto currentDuration = chrono::duration_cast
      //     <chrono::seconds>(currentTime - startTime).count();
      //   Float remainingDuration = currentDuration * (100.0 / percent - 1) / 3600;
      //   cout << "Current count" << COUT_WIDTH << currentCount <<
      //     COUT_WIDTH << COUT_PRECISION << fixed << percent << "%" <<
      //     COUT_WIDTH << remainingDuration << "h left.\n";
      // }
    }
  }
  return FALSE;
}

Bool Space::isEachGrid2tupleJoinable() {
  setEdgePreImages();
  for (Long gridIndex = 0; gridIndex < SPACE_SIZE; gridIndex++) {
    for (Long rightGridIndex = 0;
        rightGridIndex < SPACE_SIZE; rightGridIndex++) {
      if (!(are2wayJoinable(gridIndex, rightGridIndex))) {
        cout << "Unjoinable grid 2-tuple: " << gridIndex <<
          ", right " << rightGridIndex << ".\n";
        return FALSE;
      }
    }
  }
  cout << "Each grid 2-tuple is joinable.\n";
  return TRUE;
}

Bool Space::are2wayJoinable(Long gridIndex, Long rightGridIndex) {
  for (Long right : rightEdgePreImage[gridIndex]) {
    for (Long left : leftEdgePreImage[rightGridIndex]) {
      if (right == left) {
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
  for (Long gridIndex = 0; gridIndex < SPACE_SIZE; gridIndex++) {
    for (Long preGridIndex : preImage[gridIndex]) {
      setGrid(preGrid, preGridIndex);
      Long rightPreEdgeIndex = getRightEdgeIndex(preGrid),
        leftPreEdgeIndex = getLeftEdgeIndex(preGrid);
      rightEdgePreImage[gridIndex].insert(rightPreEdgeIndex);
      leftEdgePreImage[gridIndex].insert(leftPreEdgeIndex);
    }
  }
}

void Space::setPreImage() {
  cout << "Started setting pre-image.\n";
  if (PRE_ORDER > 7) {
     cout << "Too big for std::vector.\n";
  }
  preImage = PreImage(SPACE_SIZE, Fiber());
  cout << "RAM needed: " << getSpaceSize(PRE_ORDER) / pow(2, 30) << "GB.\n";
  if (PRE_ORDER > 5) {
    throw exception();
  }
  for (Long preGridIndex = 0;
      preGridIndex < PRE_SPACE_SIZE;
      preGridIndex++) {
    Long gridIndex =
      getPostGridIndex(preGridIndex, preGrid, grid);
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
      cout << "Not in image: post-grid state index " <<
        postGridIndex << ".\n";
    }
  }
  cout << "Post-space size: " << POST_SPACE_SIZE << ".\n";
  cout << "Image size: " << imageSize << ".\n";
  return imageSize;
}

void Space::setImage() {
  image = Image(POST_SPACE_SIZE, FALSE);
  cout << "Started setting image.\n";
  auto startTime = chrono::system_clock::now();
  for (Long gridIndex = 0; gridIndex < SPACE_SIZE; gridIndex++) {
    if (!(gridIndex & COUT_PERIOD)) {
      Float percent = 100.0 * gridIndex / SPACE_SIZE;
      auto currentTime = chrono::system_clock::now();
      auto currentDuration = chrono::duration_cast
        <chrono::seconds>(currentTime - startTime).count();
      Float remainingDuration = currentDuration * (100.0 / percent - 1) / 3600;
      cout << "Grid state index" << COUT_WIDTH << gridIndex <<
        COUT_WIDTH << COUT_PRECISION << fixed << percent << "%" <<
        COUT_WIDTH << remainingDuration << "h left.\n";
    }
    image[getPostGridIndex(gridIndex, grid, postGrid)] = TRUE;
  }
  auto endTime = chrono::system_clock::now();
  auto totalDuration = chrono::duration_cast
    <chrono::seconds>(endTime - startTime).count();
  cout << "Ended setting image after "
    << totalDuration << " seconds.\n";
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
Long getBottomEdgeIndex(const Grid &grid) {
  Char order = grid.size();
  return getSubGridIndex(grid, order - 2, order, 0, order);
}
Long getTopEdgeIndex(const Grid &grid) {
  Char order = grid.size();
  return getSubGridIndex(grid, 0, 2, 0, order);
}

Long getPostGridIndex(Long gridIndex, Grid &grid, Grid &postGrid) {
  setGrid(grid, gridIndex);
  setPostGrid(postGrid, grid);
  return getGridIndex(postGrid);
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

void setGrid(Grid &grid, Long gridIndex) {
  Char order = grid.size();
  for (Char ri = 0; ri < order; ri++) {
    for (Char ci = 0; ci < order; ci++) {
      Char leastBit = gridIndex & 1;
      grid[ri][ci] = leastBit;
      gridIndex >>= 1;
    }
  }
}

void setPostGrid(Grid &postGrid, const Grid &grid) {
  Char order = grid.size();
  for (Char ri = 1; ri < order - 1; ri++) {
    for (Char ci = 1; ci < order - 1; ci++) {
      postGrid[ri - 1][ci - 1] = getPostCellState(grid, ri, ci);
    }
  }
}

CellState getPostCellState(const Grid &grid,
    Char rowIndex, Char columnIndex) {
  Char i = getCellState(grid, rowIndex, columnIndex);
  Char j = getAliveNeighborCount(grid, rowIndex, columnIndex);
  return RULE_MATRIX[i][j];
}

Char getAliveNeighborCount(const Grid &grid,
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

CellState getCellState(const Grid &grid,
    Char rowIndex, Char columnIndex) {
  return grid[rowIndex][columnIndex];
}

////////////////////////////////////////////////////////////
