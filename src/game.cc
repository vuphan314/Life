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
    Long rightGridIndex, Long bottomGridIndex) {
  cout << "are3wayJoinable\n";
  auto startTime = getTime();
  Long fiberSize = preImage[gridIndex].size(),
    rightFiberSize = preImage[rightGridIndex].size(),
    bottomFiberSize = preImage[bottomGridIndex].size(),
    totalCount = fiberSize *
      rightFiberSize * bottomFiberSize,
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
        for (Long bottomPreGridIndex :
            preImage[bottomGridIndex]) {
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
      if (!(currentCount & COUT_PERIOD)) {
        Float currentPercentage = 100.0 *
          currentCount / totalCount;
        Float remainingDuration =
          getRemainingDuration(startTime, currentPercentage);
        cout << "Current count"
          << COUT_WIDTH << currentCount <<
          COUT_WIDTH << COUT_PRECISION << fixed <<
          currentPercentage << "%" <<
          COUT_WIDTH << remainingDuration << "h left.\n";
      }
    }
  }
  return FALSE;
}

Bool Space::isEachGrid3tuplePossiblyJoinable() {
  setEdgePreImages();
  auto startTime = getTime();
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
          Float remainingDuration =
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
  rightEdgePreImage = EdgePreImage(SPACE_SIZE, EdgeFiber());
  leftEdgePreImage = EdgePreImage(SPACE_SIZE, EdgeFiber());
  bottomEdgePreImage = EdgePreImage(SPACE_SIZE, EdgeFiber());
  topEdgePreImage = EdgePreImage(SPACE_SIZE, EdgeFiber());
  for (Long gridIndex = 0; gridIndex < SPACE_SIZE;
      gridIndex++) {
    for (Long preGridIndex : preImage[gridIndex]) {
      setGrid(preGrid, preGridIndex);
      Long rightPreEdgeIndex = getRightEdgeIndex(preGrid),
        leftPreEdgeIndex = getLeftEdgeIndex(preGrid),
        bottomPreEdgeIndex = getBottomEdgeIndex(preGrid),
        topPreEdgeIndex = getTopEdgeIndex(preGrid);
      rightEdgePreImage[gridIndex].insert(rightPreEdgeIndex);
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
  auto startTime = getTime();
  for (Long gridIndex = 0; gridIndex < SPACE_SIZE;
      gridIndex++) {
    if (!(gridIndex & COUT_PERIOD)) {
      Float currentPercentage = 100.0 *
        gridIndex / SPACE_SIZE;
      Float remainingDuration =
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
  auto totalDuration = getDuration(startTime);
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

Long getRightEdgeIndex(const Grid &grid) {
  Char order = grid.size();
  return getMatrixIndex(grid, 0, order, order - 2, order);
}
Long getLeftEdgeIndex(const Grid &grid) {
  Char order = grid.size();
  return getMatrixIndex(grid, 0, order, 0, 2);
}
Long getBottomEdgeIndex(const Grid &grid) {
  Char order = grid.size();
  return getMatrixIndex(grid, order - 2, order, 0, order);
}
Long getTopEdgeIndex(const Grid &grid) {
  Char order = grid.size();
  return getMatrixIndex(grid, 0, 2, 0, order);
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
