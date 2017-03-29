#ifndef GAME_H
#define GAME_H

////////////////////////////////////////////////////////////

using namespace std;

#include <cmath>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>

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

using Image = vector<Bool>; // image[postGridIndex]

using Fiber = vector<Long>;
using PreImage = vector<Fiber>;
  // preImage[gridIndex][preGridIndex]

using SortedFiber = unordered_map<Long, vector<Long>>;
using SortedPreImage = vector<SortedFiber>;
  // sortedPreImage[gridIndex][preEdgeIndex][preGridIndex]

using EdgeFiber = unordered_set<Long>;
using EdgePreImage = vector<EdgeFiber>;
  // edgePreImage[gridIndex][preEdgeIndex]

////////////////////////////////////////////////////////////

const Bool FALSE = false;
const Bool TRUE = true;

// inclusive:
const Char LOWER_BIRTH = 3;
const Char UPPER_BIRTH = 3;
const Char LOWER_SURVIVAL = 2;
const Char UPPER_SURVIVAL = 3;

const Long COUNT_CUTOFF = pow(2, 40);
const Long COUT_PERIOD = pow(2, 15) - 1;
const auto COUT_WIDTH = setw(15);
const auto COUT_WIDE_WIDTH = setw(25);
const auto COUT_PRECISION = setprecision(4);

////////////////////////////////////////////////////////////

extern Matrix RULES; // const after being set

void setRULES();

////////////////////////////////////////////////////////////

class Space {
private:
  Char ORDER, POST_ORDER, PRE_ORDER;
  Long SPACE_SIZE, POST_SPACE_SIZE, PRE_SPACE_SIZE,
    EDGE_PRE_SPACE_SIZE;
// temporary:
  Grid grid, postGrid, preGrid; // (n+{0,-1,1})^2
  Edge verticalPreEdge, horizontalPreEdge; // 2*(n+2)
// containers:
  Image image; // 2^(n-2)^2, set by setImage
  PreImage preImage; // 2^(n+2)^2, set by setPreImage
  SortedPreImage rightSortedPreImage, leftSortedPreImage,
    // bottomSortedPreImage,
    topSortedPreImage;
    // 2^(2*(n+2)), set by setSortedPreImages
  EdgePreImage rightEdgePreImage, leftEdgePreImage,
    bottomEdgePreImage, topEdgePreImage;
    // 2^(2*(n+2)), set by setEdgePreImages

public:
  Space(Char order);

  void inspectPreImage();

  Bool isEachGrid3tupleJoinable();

  Bool are3wayJoinable(Long gridIndex,
    Long rightGridIndex, Long bottomGridIndex,
    Bool verbose = FALSE);

  Bool isEachGrid2tupleJoinable();

  Bool are2wayJoinable(Long gridIndex, Long rightGridIndex);

  void setEdgePreImages();

  void setSortedPreImages();

  void setPreImage();

  Float getImageProportion();

  Long getImageSize();

  void setImage();
};

////////////////////////////////////////////////////////////
// global functions:

Long getEdgeSpaceSize(Char order);
Long getSpaceSize(Char order);

Bool canOverlap(Long rightEdgeIndex, Long bottomEdgeIndex,
  Edge &rightEdge, Edge &bottomEdge);

void testGettingEdgeIndices();

// \Theta(n)
Long getRightEdgeIndex(Long gridIndex, Char order);
Long getLeftEdgeIndex(Long gridIndex, Char order);

// \Theta(1)
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
