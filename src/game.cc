#include "game.h"

////////////////////////////////////////////////////////////

Matrix RULES;

void setRULES() {
  RULES = Matrix(2, Row(9, FALSE));
  for (Char j = LOWER_BIRTH; j <= UPPER_BIRTH; j++) {
    RULES[0][j] = TRUE;
  }
  for (Char j = LOWER_SURVIVAL; j <= UPPER_SURVIVAL; j++) {
    RULES[1][j] = TRUE;
  }
}

////////////////////////////////////////////////////////////
// class Space:

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

  verticalPreEdge = Grid(PRE_ORDER, Row(2, FALSE));
  horizontalPreEdge = Grid(2, Row(2, PRE_ORDER));
}

void Space::inspectPreImage() {
  setPreImage();
  cout << "Pre-space size: " << PRE_SPACE_SIZE << ".\n";
  for (Long gridIndex = 0; gridIndex < SPACE_SIZE;
      gridIndex++) {
    Long fiberSize = preImage[gridIndex].size();
    Float currentPercentage = 100.0 *
      fiberSize / PRE_SPACE_SIZE;
    cout << "Grid index" << COUT_WIDTH << gridIndex <<
      COUT_WIDTH << COUT_PRECISION << fixed <<
      currentPercentage << "%\n";
  }
}

Bool Space::isEachGrid4tupleJoinable() {
  cout << "isEachGrid4tupleJoinable\n";
  Time startTime = getTime();
  setEdgeMapPreImages();
  Long totalCount = pow(SPACE_SIZE, 4),
    cc = 0; // currentCount
  // for (Long gi0 = 0; gi0 < SPACE_SIZE; gi0++, cc++) {
  //   for (Long gi1 = 0; gi1 < SPACE_SIZE; gi1++, cc++) {
  //     for (Long gi2 = 0; gi2 < SPACE_SIZE; gi2++, cc++) {
  //       for (Long gi3 = 0; gi3 < SPACE_SIZE; gi3++, cc++) {
  for (long long gi0 = SPACE_SIZE - 1; gi0 >= 0; gi0--, cc++) {
    for (long long gi1 = SPACE_SIZE - 1; gi1 >= 0; gi1--, cc++) {
      for (long long gi2 = SPACE_SIZE - 1; gi2 >= 0; gi2--, cc++) {
        for (long long gi3 = SPACE_SIZE - 1; gi3 >= 0; gi3--, cc++) {
          if (cc < START_COUNT) {
            continue;
          }
          if (!(cc & COUT_PERIOD)) {
            Float currentPercentage = 100.0 *
              cc / totalCount;
            Duration remainingDuration =
              getRemainingDuration(startTime,
                currentPercentage);
            cout << "Grid 4-tuple"
              << COUT_WIDTH << cc <<
              COUT_WIDTH << COUT_PRECISION << fixed <<
              currentPercentage << "%" <<
              COUT_WIDTH << remainingDuration << "h left.\n";
          }
          if (!(are4wayJoinable(gi0, gi1, gi2, gi3))) {
            Duration totalDuration = getDuration(startTime);
            cout << "Unjoinable grid 4-tuple: " << gi0 <<
              ", " << gi1 << ", " << gi2 << ", " << gi3 <<
              ".\nEnded after " << totalDuration << "s.\n";
            return FALSE;
          }
        }
      }
    }
  }
  Duration totalDuration = getDuration(startTime);
  cout << "Each grid 4-tuple is joinable.\nEnded after " <<
    totalDuration << "s.\n";
  return TRUE;
}

Bool Space::are4wayJoinable(Long gridIndex0,
    Long gridIndex1, Long gridIndex2, Long gridIndex3) {
  EdgeMapFiber &topRight2 =
    topRightEdgeMapPreImage[gridIndex2],
    &leftBottom1 = leftBottomEdgeMapPreImage[gridIndex1];
  for (Long preGridIndex0 : preImage[gridIndex0]) {
    Long b0 = getBottomEdgeIndex(preGridIndex0, PRE_ORDER);
    if (topRight2.find(b0) != topRight2.end()) {
      Long r0 = getRightEdgeIndex(preGridIndex0, PRE_ORDER);
      if (leftBottom1.find(r0) != leftBottom1.end()) {
        LongSet &lefts3 = topRight2[b0],
          &tops3 = leftBottom1[r0];
        for (Long preGridIndex3 : preImage[gridIndex3]) {
          Long t3 = getTopEdgeIndex(preGridIndex3, PRE_ORDER);
          if (tops3.find(t3) != tops3.end()) {
            Long l3 = getLeftEdgeIndex(preGridIndex3, PRE_ORDER);
            if (lefts3.find(l3) != lefts3.end()) {
              return TRUE;
            }
          }
        }
      }
    }
  }
  return FALSE;
}

Bool Space::isEachGrid3tupleJoinable() {
  cout << "isEachGrid3tupleJoinable\n";
  Time startTime = getTime();
  setEdgePreImages();
  Long totalCount = pow(SPACE_SIZE, 3), currentCount = 0;
  for (Long gridIndex = 0; gridIndex < SPACE_SIZE;
      gridIndex++, currentCount++) {
    for (Long rightGridIndex = 0;
        rightGridIndex < SPACE_SIZE;
        rightGridIndex++, currentCount++) {
      for (Long bottomGridIndex = 0;
          bottomGridIndex < SPACE_SIZE;
          bottomGridIndex++, currentCount++) {
        if (!(currentCount & COUT_PERIOD)) {
          Float currentPercentage = 100.0 *
            currentCount / totalCount;
          Duration remainingDuration =
            getRemainingDuration(startTime,
              currentPercentage);
          cout << "Grid 3-tuple"
            << COUT_WIDTH << currentCount <<
            COUT_WIDTH << COUT_PRECISION << fixed <<
            currentPercentage << "%" <<
            COUT_WIDTH << remainingDuration << "h left.\n";
        }
        if (!(are3wayJoinable(gridIndex,
            rightGridIndex, bottomGridIndex))) {
          Duration totalDuration = getDuration(startTime);
          cout << "Unjoinable grid 3-tuple: "
            << gridIndex <<
            ", right " << rightGridIndex <<
            ", bottom " << bottomGridIndex <<
            ".\nEnded after " << totalDuration << "s.\n";
          return FALSE;
        }
      }
    }
  }
  Duration totalDuration = getDuration(startTime);
  cout << "Each grid 3-tuple is joinable.\nEnded after " <<
    totalDuration << "s.\n";
  return TRUE;
}

Bool Space::are3wayJoinable(Long gridIndex,
    Long rightGridIndex, Long bottomGridIndex) {
  const EdgeFiber &topsOfBottom =
    topEdgePreImage[bottomGridIndex],
    &leftsOfRight = leftEdgePreImage[rightGridIndex];
  for (Long preGridIndex : preImage[gridIndex]) {
    Long bottom = getBottomEdgeIndex(preGridIndex,
      PRE_ORDER);
    if (topsOfBottom.find(bottom) != topsOfBottom.end()) {
      Long right = getRightEdgeIndex(preGridIndex,
        PRE_ORDER);
      if (leftsOfRight.find(right) != leftsOfRight.end()) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

Bool Space::isEachGrid2tupleJoinable() {
  setEdgePreImages();
  for (Long gridIndex = 0; gridIndex < SPACE_SIZE;
      gridIndex++) {
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

Bool Space::are2wayJoinable(Long gridIndex,
    Long rightGridIndex) {
  for (Long right : rightEdgePreImage[gridIndex]) {
    for (Long left : leftEdgePreImage[rightGridIndex]) {
      if (right == left) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

void Space::setEdgeMapPreImages() {
  setPreImage();
  leftBottomEdgeMapPreImage = topRightEdgeMapPreImage =
    EdgeMapPreImage(SPACE_SIZE, EdgeMapFiber());
  for (Long gi = 0; gi < SPACE_SIZE; gi++) {
    EdgeMapFiber &leftBottomEdgeMapFiber =
        leftBottomEdgeMapPreImage[gi],
      &topRightEdgeMapFiber = topRightEdgeMapPreImage[gi];
    for (Long pgi : preImage[gi]) {
      Long left = getLeftEdgeIndex(pgi, PRE_ORDER),
        bottom = getBottomEdgeIndex(pgi, PRE_ORDER),
        top = getTopEdgeIndex(pgi, PRE_ORDER),
        right = getRightEdgeIndex(pgi, PRE_ORDER);
      if (leftBottomEdgeMapFiber.find(left) ==
          leftBottomEdgeMapFiber.end()) {
        leftBottomEdgeMapFiber[left] = LongSet();
      }
      leftBottomEdgeMapFiber[left].insert(bottom);
      if (topRightEdgeMapFiber.find(top) ==
          topRightEdgeMapFiber.end()) {
        topRightEdgeMapFiber[top] = LongSet();
      }
      topRightEdgeMapFiber[top].insert(right);
    }
  }
}

void Space::setEdgePreImages() {
  setPreImage();
  rightEdgePreImage = leftEdgePreImage =
    bottomEdgePreImage = topEdgePreImage =
    EdgePreImage(SPACE_SIZE, EdgeFiber());
  for (Long gridIndex = 0; gridIndex < SPACE_SIZE;
      gridIndex++) {
    for (Long preGridIndex : preImage[gridIndex]) {
      Long rightPreEdgeIndex = getRightEdgeIndex(
          preGridIndex, PRE_ORDER),
        leftPreEdgeIndex = getLeftEdgeIndex(preGridIndex,
          PRE_ORDER),
        bottomPreEdgeIndex = getBottomEdgeIndex(
          preGridIndex, PRE_ORDER),
        topPreEdgeIndex = getTopEdgeIndex(preGridIndex,
          PRE_ORDER);
      rightEdgePreImage[gridIndex].insert(
        rightPreEdgeIndex);
      leftEdgePreImage[gridIndex].insert(leftPreEdgeIndex);
      bottomEdgePreImage[gridIndex].insert(
        bottomPreEdgeIndex);
      topEdgePreImage[gridIndex].insert(topPreEdgeIndex);
    }
  }
}

void Space::setPreImage() {
  cout << "Started setting pre-image.\n";
  if (PRE_ORDER > 7) {
     cout << "Too big for std::vector.\n";
  }
  preImage = PreImage(SPACE_SIZE, Fiber());
  cout << "RAM needed: " <<
    getSpaceSize(PRE_ORDER) / pow(2, 30) << "GB.\n";
  if (PRE_ORDER > 5) {
    throw exception();
  }
  for (Long preGridIndex = 0;
      preGridIndex < PRE_SPACE_SIZE; preGridIndex++) {
    Long gridIndex = getPostGridIndex(preGridIndex,
      preGrid, grid);
    preImage[gridIndex].push_back(preGridIndex);
  }
  cout << "Ended setting pre-image.\n";
}

Float Space::getImageProportion() {
  Float imageProportion = 1.0 *
    getImageSize() / POST_SPACE_SIZE;
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
  Time startTime = getTime();
  for (Long gridIndex = 0; gridIndex < SPACE_SIZE;
      gridIndex++) {
    if (!(gridIndex & COUT_PERIOD)) {
      Float currentPercentage = 100.0 *
        gridIndex / SPACE_SIZE;
      Duration remainingDuration =
        getRemainingDuration(startTime, currentPercentage);
      cout << "Grid state index"
        << COUT_WIDTH << gridIndex <<
        COUT_WIDTH << COUT_PRECISION << fixed <<
        currentPercentage << "%" <<
        COUT_WIDTH << remainingDuration << "h left.\n";
    }
    image[getPostGridIndex(gridIndex, grid, postGrid)] =
      TRUE;
  }
  Duration totalDuration = getDuration(startTime);
  cout << "Ended setting image after " << totalDuration <<
    "s.\n";
}

////////////////////////////////////////////////////////////
// global functions:

Long getEdgeSpaceSize(Char order) {
  return pow(2, 2 * order);
}
Long getSpaceSize(Char order) {
  return pow(2, pow(order, 2));
}

void testGettingEdgeIndices() {
  cout << "Testing getting edge indices.\n";
  Grid grid{{0, 1, 0}, {0, 0, 1}, {1, 0, 0}};
  Char order = grid.size();
  Long gridIndex = getGridIndex(grid); // 98
  if (getRightEdgeIndex(gridIndex, order) == 9 &&
      getLeftEdgeIndex(gridIndex, order) == 18 &&
      getBottomEdgeIndex(gridIndex, order) == 12 &&
      getTopEdgeIndex(gridIndex, order) == 34) {
    cout << "Test passed.\n";
  } else {
    cout << "Test failed.\n";
  }
}

Long getRightEdgeIndex(Long gridIndex, Char order) {
  Long rightEdgeIndex = 0, mask = 3 << (order - 2);
  Char multiplier = 0;
  while (gridIndex > 0) {
    Char rightBitPair = (gridIndex & mask) >> (order - 2);
    rightEdgeIndex |= rightBitPair << multiplier;
    multiplier += 2;
    gridIndex >>= order;
  }
  return rightEdgeIndex;
}
Long getLeftEdgeIndex(Long gridIndex, Char order) {
  Long leftEdgeIndex = 0;
  Char multiplier = 0;
  while (gridIndex > 0) {
    Char leftBitPair = gridIndex & 3;
    leftEdgeIndex |= leftBitPair << multiplier;
    multiplier += 2;
    gridIndex >>= order;
  }
  return leftEdgeIndex;
}

Long getBottomEdgeIndex(Long gridIndex, Char order) {
  return gridIndex >> ((order - 2) * order);
}
Long getTopEdgeIndex(Long gridIndex, Char order) {
  Long mask = (1 << (2 * order)) - 1;
  return gridIndex & mask;
}

Long getPostGridIndex(Long gridIndex,
    Grid &grid, Grid &postGrid) {
  setGrid(grid, gridIndex);
  setPostGrid(postGrid, grid);
  return getGridIndex(postGrid);
}

Long getGridIndex(const Grid &grid) {
  Char order = grid.size();
  return getMatrixIndex(grid, 0, order, 0, order);
}

Long getMatrixIndex(const Matrix &matrix,
    Char startRow, Char endRow,
    Char startColumn, Char endColumn) {
  Long matrixIndex = 0;
  Int cellIndex = 0;
  for (Char ri = startRow; ri < endRow; ri++) {
    for (Char ci = startColumn; ci < endColumn; ci++) {
      matrixIndex |= matrix[ri][ci] << cellIndex;
      cellIndex++;
    }
  }
  return matrixIndex;
}

void setGrid(Grid &grid, Long gridIndex) {
  setMatrix(grid, gridIndex);
}

void setMatrix(Matrix &matrix, Long matrixIndex) {
  for (Char ri = 0; ri < matrix.size(); ri++) {
    for (Char ci = 0; ci < matrix[0].size(); ci++) {
      Char leastBit = matrixIndex & 1;
      matrix[ri][ci] = leastBit;
      matrixIndex >>= 1;
    }
  }
}

void setPostGrid(Grid &postGrid, const Grid &grid) {
  Char order = grid.size();
  for (Char ri = 1; ri < order - 1; ri++) {
    for (Char ci = 1; ci < order - 1; ci++) {
      postGrid[ri - 1][ci - 1] =
        getPostCellState(grid, ri, ci);
    }
  }
}

CellState getPostCellState(const Grid &grid,
    Char rowIndex, Char columnIndex) {
  Char i = getCellState(grid, rowIndex, columnIndex);
  Char j = getAliveNeighborCount(grid, rowIndex,
    columnIndex);
  return RULES[i][j];
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
