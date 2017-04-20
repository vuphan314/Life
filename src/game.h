#ifndef GAME_H
#define GAME_H

////////////////////////////////////////////////////////////

using namespace std;

#include <cmath>
#include <iomanip>
#include <unordered_set>
#include <unordered_map>

#include "../cplusplus/src/timing.h"

////////////////////////////////////////////////////////////

using Char = unsigned char; // n
using Int = unsigned int; // n^2
using Long = unsigned long long; // 2^n^2
using Float = long double;

using Bool = Char;

using CellState = Bool;
using Row = vector<CellState>;
using Matrix = vector<Row>;
using Grid = Matrix;
using Edge = Matrix;

// containers:

using Image = vector<Bool>; // image[postGridIndex]

using Fiber = vector<Long>;
using PreImage = vector<Fiber>;
  // preImage[gridIndex][preGridIndex]

using LongSet = unordered_set<Long>;

using EdgeFiber = LongSet;
using EdgePreImage = vector<EdgeFiber>;
  // edgePreImage[gridIndex][preEdgeIndex]

using EdgeMapFiber = unordered_map<Long, LongSet>;
using EdgeMapPreImage = vector<EdgeMapFiber>;
  // edgeMapPreImage[gridIndex][preEdgeIndex][preEdgeIndex]

////////////////////////////////////////////////////////////

const Bool FALSE = false;
const Bool TRUE = true;

// inclusive:
const Char LOWER_BIRTH = 3;
const Char UPPER_BIRTH = 3;
const Char LOWER_SURVIVAL = 2;
const Char UPPER_SURVIVAL = 3;

const Long COUT_PERIOD = pow(2, 15) - 1;
const auto COUT_WIDTH = setw(12);
const auto COUT_PRECISION = setprecision(1);

const Long START_COUNT = 520978432; // log.txt

////////////////////////////////////////////////////////////

extern Matrix RULES; // const after being set

void setRULES();

////////////////////////////////////////////////////////////

class Space {
// private:
public:
  Char ORDER, POST_ORDER, PRE_ORDER;
  Long SPACE_SIZE, POST_SPACE_SIZE, PRE_SPACE_SIZE,
    EDGE_PRE_SPACE_SIZE;
// temporary:
  Grid grid, postGrid, preGrid; // (n+{0,-1,1})^2
  Edge verticalPreEdge, horizontalPreEdge; // 2*(n+2)
// containers:
  Image image; // 2^(n-2)^2, set by setImage
  PreImage preImage; // 2^(n+2)^2, set by setPreImage
  EdgePreImage rightEdgePreImage, leftEdgePreImage,
    bottomEdgePreImage, topEdgePreImage;
    // 2^(2*(n+2)), set by setEdgePreImages
  EdgeMapPreImage leftBottomEdgeMapPreImage,
    topRightEdgeMapPreImage;
    // set by setEdgeMapPreImages

public:
  Space(Char order);

  void inspectPreImage();

  Bool isEachGrid4tupleJoinable();

  Bool are4wayJoinable(Long gridIndex0, Long gridIndex1,
    Long gridIndex2, Long gridIndex3);

  Bool isEachGrid3tupleJoinable();

  Bool are3wayJoinable(Long gridIndex,
    Long rightGridIndex, Long bottomGridIndex);

  Bool isEachGrid2tupleJoinable();

  Bool are2wayJoinable(Long gridIndex, Long rightGridIndex);

  void setEdgeMapPreImages();

  void setEdgePreImages();

  void setPreImage();

  Float getImageProportion();

  Long getImageSize();

  void setImage();
};

////////////////////////////////////////////////////////////
// global functions:

Long getEdgeSpaceSize(Char order);
Long getSpaceSize(Char order);

void testGettingEdgeIndices();
// Theta(n):
Long getRightEdgeIndex(Long gridIndex, Char order);
Long getLeftEdgeIndex(Long gridIndex, Char order);
// Theta(1):
Long getBottomEdgeIndex(Long gridIndex, Char order);
Long getTopEdgeIndex(Long gridIndex, Char order);

Long getPostGridIndex(Long gridIndex,
  Grid &grid, Grid &postGrid);

Long getGridIndex(const Grid &grid);

Long getMatrixIndex(const Matrix &matrix,
  Char startRow, Char endRow, // [startRow, endRow)
  Char startColumn, Char endColumn);

void setGrid(Grid &grid, Long gridIndex);

void setMatrix(Matrix &matrix, Long matrixIndex);

void setPostGrid(Grid &postGrid, const Grid &grid);

CellState getPostCellState(const Grid &grid,
  Char rowIndex, Char columnIndex);

Char getAliveNeighborCount(const Grid &grid,
  Char rowIndex, Char columnIndex);

CellState getCellState(const Grid &grid,
  Char rowIndex, Char columnIndex);

////////////////////////////////////////////////////////////

#endif // GAME_H
