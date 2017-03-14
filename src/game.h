#ifndef GAME_H
#define GAME_H

////////////////////////////////////////////////////////////

using namespace std;

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "../cplusplus/src/vectors.h"

////////////////////////////////////////////////////////////

using Char = unsigned char; // n
using Int = unsigned int; // n^2
using Long = unsigned long long; // 2^n^2
using Float = long double;

using Bool = Char;

using CellState = Bool;
using Row = vector<CellState>;
using Grid = vector<Row>;

using Image = vector<Bool>; // image[postGridIndex]

using Fiber = vector<Long>;
using PreImage = vector<Fiber>; // preImage[gridIndex][preGridIndex]

using EdgeFiber = unordered_set<Long>;
using EdgePreImage = vector<EdgeFiber>;

////////////////////////////////////////////////////////////

extern vector<vector<CellState>> RULE_MATRIX;

void setRULE_MATRIX();

////////////////////////////////////////////////////////////

class Space {
private:
  Char ORDER, POST_ORDER, PRE_ORDER;
  Long SPACE_SIZE, POST_SPACE_SIZE, PRE_SPACE_SIZE,
    EDGE_PRE_SPACE_SIZE;
  Grid grid, postGrid, preGrid; // (n -|+ 1)^2, temporary
  Image image; // 2^(n-2)^2, on demand
  PreImage preImage; // 2^(n+2)^2, on demand
  EdgePreImage rightEdgePreImage, leftEdgePreImage;
    // 2^(2*(n+2)), on demand

public:
  Space(Char order);

  void inspectSpace();

  Bool isEachGrid3tupleJoinable();

  Bool are3wayJoinable(Long gridIndex,
    Long rightGridIndex, Long bottomGridIndex);

  Bool isEachGrid2tupleJoinable();

  Bool are2wayJoinable(Long leftGridIndex, Long rightGridIndex);

  void setEdgePreImages();

  void setPreImage();

  Float getImageProportion();

  Long getImageSize();

  void setImage();
};

////////////////////////////////////////////////////////////

Long getPostGridIndex(Long gridIndex, Grid &grid, Grid &postGrid);

void setPostGrid(Grid &postGrid, const Grid &grid);

CellState getPostCellState(const Grid &grid, Char rowIndex, Char columnIndex);

Char getAliveNeighborCount(const Grid &grid, Char rowIndex, Char columnIndex);

CellState getCellState(const Grid &grid, Char rowIndex, Char columnIndex);

////////////////////////////////////////////////////////////

Long getEdgeSpaceSize(Char order);

Long getSpaceSize(Char order);

Long getBottomEdgeIndex(const Grid &grid);

Long getTopEdgeIndex(const Grid &grid);

Long getRightEdgeIndex(const Grid &grid);

Long getLeftEdgeIndex(const Grid &grid);

Long getGridIndex(const Grid &grid);

Long getSubGridIndex(const Grid &grid,
  Char startRow, Char endRow,
  Char startColumn, Char endColumn);

void setGrid(Grid &grid, Long gridIndex);

////////////////////////////////////////////////////////////

const Bool FALSE = false;
const Bool TRUE = true;

const Char LOWER_BIRTH = 3;
const Char UPPER_BIRTH = 3;
const Char LOWER_SURVIVAL = 2;
const Char UPPER_SURVIVAL = 3;

const Long COUT_PERIOD = pow(2, 23) - 1;
const auto COUT_WIDTH = setw(12);
const auto COUT_PRECISION = setprecision(4);

////////////////////////////////////////////////////////////

#endif // GAME_H
