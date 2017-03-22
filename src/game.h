#ifndef GAME_H
#define GAME_H

////////////////////////////////////////////////////////////

using namespace std;

#include <cmath>
#include <iomanip>
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

using EdgeFiber = unordered_set<Long>;
using EdgePreImage = vector<EdgeFiber>;
  // edgePreImage[gridIndex][preEdgeIndex]

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
// set by: setImage, setPreImage, setEdgePreImages:
  Image image; // 2^(n-2)^2
  PreImage preImage; // 2^(n+2)^2
  EdgePreImage rightEdgePreImage, // 2^(2*(n+2))
    leftEdgePreImage, bottomEdgePreImage, topEdgePreImage;

public:
  Space(Char order);

  void inspectPreImage();

  Bool isEachGrid3tupleJoinable();

  Bool are3wayJoinable(Long gridIndex,
    Long rightGridIndex, Long bottomGridIndex);

  Bool isEachGrid3tuplePossiblyJoinable();

  Bool arePossibly3wayJoinable(Long gridIndex,
    Long rightGridIndex, Long bottomGridIndex);

  Bool isEachGrid2tupleJoinable();

  Bool are2wayJoinable(Long gridIndex, Long rightGridIndex);

  void setEdgePreImages();

  void setPreImage();

  Float getImageProportion();

  Long getImageSize();

  void setImage();
};

////////////////////////////////////////////////////////////

Long getEdgeSpaceSize(Char order);
Long getSpaceSize(Char order);

Bool canOverlap(Long rightEdgeIndex, Long bottomEdgeIndex,
  Edge &rightEdge, Edge &bottomEdge);

Long getRightEdgeIndex(const Grid &grid);
Long getLeftEdgeIndex(const Grid &grid);
Long getBottomEdgeIndex(const Grid &grid);
Long getTopEdgeIndex(const Grid &grid);

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

const Bool FALSE = false;
const Bool TRUE = true;

// inclusive:
const Char LOWER_BIRTH = 3;
const Char UPPER_BIRTH = 3;
const Char LOWER_SURVIVAL = 2;
const Char UPPER_SURVIVAL = 3;

const Long COUNT_CUTOFF = pow(2, 41);
const Long COUT_PERIOD = pow(2, 22) - 1;
const auto COUT_WIDTH = setw(15);
const auto COUT_WIDE_WIDTH = setw(25);
const auto COUT_PRECISION = setprecision(4);

////////////////////////////////////////////////////////////

#endif // GAME_H
