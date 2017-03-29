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

Bool Space::isEachGrid3tupleJoinable() {
  cout << "isEachGrid3tupleJoinable\n";
  setPreImage();
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
          cout << "Grid 3-tuple:"
            << COUT_WIDTH << currentCount <<
            COUT_WIDTH << COUT_PRECISION << fixed <<
            currentPercentage << "%.\n";
        }
        if (!(are3wayJoinable(gridIndex,
            rightGridIndex, bottomGridIndex))) {
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
    Long rightGridIndex, Long bottomGridIndex,
    Bool verbose) {
  Time startTime = getTime();
  Long fiberSize = preImage[gridIndex].size(),
    rightFiberSize = preImage[rightGridIndex].size(),
    bottomFiberSize = preImage[bottomGridIndex].size(),
    totalCount = fiberSize * rightFiberSize *
      bottomFiberSize,
    currentCount = 0;
  if (verbose) {
    cout << "Fiber size: " << fiberSize <<
    ".\nRight fiber size: " << rightFiberSize <<
    ".\nBottom fiber size: " << bottomFiberSize <<
    ".\nTotal count: " << totalCount << ".\n";
  }
  for (Long preGridIndex : preImage[gridIndex]) {
    Long right = getRightEdgeIndex(preGridIndex, PRE_ORDER),
      bottom = getBottomEdgeIndex(preGridIndex, PRE_ORDER);
    for (Long rightPreGridIndex : preImage[rightGridIndex]) {
      Long left = getLeftEdgeIndex(rightPreGridIndex,
        PRE_ORDER);
      if (right == left) {
        for (Long bottomPreGridIndex :
            preImage[bottomGridIndex]) {
          Long top = getTopEdgeIndex(bottomPreGridIndex,
            PRE_ORDER);
          if (bottom == top) {
            return TRUE;
          }
          currentCount++;
        }
      } else {
        currentCount += bottomFiberSize;
      }
      if (currentCount > COUNT_CUTOFF) {
        cout << "Skipped grid 3-tuple" <<
          COUT_WIDTH << gridIndex <<
          COUT_WIDTH << rightGridIndex <<
          COUT_WIDTH << bottomGridIndex << ".\n";
        return TRUE;
      }
      if (!(currentCount & COUT_PERIOD)) {
        cout << "are3wayJoinable\n";
        Float currentPercentage = 100.0 *
          currentCount / totalCount;
        Duration remainingDuration =
          getRemainingDuration(startTime,
          currentPercentage);
        cout << "\tCurrent count"
          << COUT_WIDTH << currentCount <<
          COUT_WIDTH << COUT_PRECISION << fixed <<
          currentPercentage << "%" <<
          COUT_WIDTH << remainingDuration << "h left.\n";
      }
      currentCount++;
    }
    currentCount++;
  }
  return FALSE;
}

Bool Space::isEachGrid3tuplePossiblyJoinable() {
  setEdgePreImages();
  Time startTime = getTime();
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
        if (!(arePossibly3wayJoinable(gridIndex,
            rightGridIndex, bottomGridIndex))) {
          cout << "Not possibly joinable grid 3-tuple: " <<
            gridIndex <<
            ", right " << rightGridIndex <<
            ", bottom " << bottomGridIndex << ".\n";
          return FALSE;
        }
      }
    }
  }
  cout << "Each grid 3-tuple is possibly joinable.\n";
  return TRUE;
}

Bool Space::arePossibly3wayJoinable(Long gridIndex,
    Long rightGridIndex, Long bottomGridIndex) {
  for (Long right : rightEdgePreImage[gridIndex]) {
    for (Long left : leftEdgePreImage[rightGridIndex]) {
      if (right == left) {
        for (Long bottom : bottomEdgePreImage[gridIndex]) {
          for (Long top : topEdgePreImage[bottomGridIndex]) {
            if (bottom == top) {
              if (canOverlap(right, bottom,
                  verticalPreEdge, horizontalPreEdge)) {
                return TRUE;
              }
            }
          }
        }
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
      rightEdgePreImage[gridIndex].insert(rightPreEdgeIndex);
      leftEdgePreImage[gridIndex].insert(leftPreEdgeIndex);
      bottomEdgePreImage[gridIndex].insert(
        bottomPreEdgeIndex);
      topEdgePreImage[gridIndex].insert(topPreEdgeIndex);
    }
  }
}

void Space::setSortedPreImages() {
  setPreImage();
  rightSortedPreImage = leftSortedPreImage =
    // bottomSortedPreImage =
    topSortedPreImage =
    SortedPreImage(SPACE_SIZE, SortedFiber());
  for (Long gridIndex = 0; gridIndex < SPACE_SIZE;
      gridIndex++) {
    for (Long preGridIndex : preImage[gridIndex]) {
      Long rightPreEdgeIndex = getRightEdgeIndex(
          preGridIndex, PRE_ORDER),
        leftPreEdgeIndex = getLeftEdgeIndex(preGridIndex,
          PRE_ORDER),
        // bottomPreEdgeIndex = getBottomEdgeIndex(
        //   preGridIndex, PRE_ORDER),
        topPreEdgeIndex = getTopEdgeIndex(preGridIndex,
          PRE_ORDER);
      rightSortedPreImage[gridIndex][rightPreEdgeIndex].
        push_back(preGridIndex);
      leftSortedPreImage[gridIndex][leftPreEdgeIndex].
        push_back(preGridIndex);
      // bottomSortedPreImage[gridIndex][bottomPreEdgeIndex].
      //   push_back(preGridIndex);
      topSortedPreImage[gridIndex][topPreEdgeIndex].
        push_back(preGridIndex);
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
  cout << "Ended setting image after "
    << totalDuration << " seconds.\n";
}

////////////////////////////////////////////////////////////
// global functions:

Long getEdgeSpaceSize(Char order) {
  return pow(2, 2 * order);
}
Long getSpaceSize(Char order) {
  return pow(2, pow(order, 2));
}

Bool canOverlap(Long rightEdgeIndex, Long bottomEdgeIndex,
    Edge &rightEdge, Edge &bottomEdge) {
  setMatrix(rightEdge, rightEdgeIndex);
  setMatrix(bottomEdge, bottomEdgeIndex);
  Char order = rightEdge.size();
  return
    rightEdge[order - 2][0] == bottomEdge[0][order - 2] &&
    rightEdge[order - 2][1] == bottomEdge[0][order - 1] &&
    rightEdge[order - 1][0] == bottomEdge[1][order - 2] &&
    rightEdge[order - 1][1] == bottomEdge[1][order - 1];
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
    rightEdgeIndex += rightBitPair << multiplier;
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
    leftEdgeIndex += leftBitPair << multiplier;
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
      matrixIndex += matrix[ri][ci] << cellIndex;
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
  Char j =
    getAliveNeighborCount(grid, rowIndex, columnIndex);
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
