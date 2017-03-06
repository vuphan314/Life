#ifndef GAME_H
#define GAME_H

////////////////////////////////////////////////////////////

using namespace std;

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "../cplusplus/src/vectors.h"

////////////////////////////////////////////////////////////

using Char = unsigned char; // n
using Int = unsigned int; // n^2
using Long = unsigned long long; // 2^n^2
using Float = long double;

using Bool = Char;
using CellState = Bool;
using GridState = Bool;

using Row = vector<CellState>;
using Grid = vector<Row>;

using Image = vector<GridState>;

using Fiber = vector<Long>;
using PreImage = vector<Fiber>;

////////////////////////////////////////////////////////////

class Game {
private:
  vector<vector<CellState>> RULE_MATRIX;
  Char ORDER, POST_ORDER, PRE_ORDER;
  Long SPACE_SIZE, POST_SPACE_SIZE;
  Image image;
  PreImage preImage;
  Grid grid, postGrid;

public:
  Game(Char order);

  void setPreImage();

  Float getImageProportion();

  Long getImageSize();

  void setImage();

  Long getPostGridStateIndex(Long gridStateIndex);

  void setGrid(Long gridStateIndex);

  void setPostGrid();

  CellState getPostCellState(Char rowIndex, Char columnIndex);

  Char getAliveNeighborCount(Char rowIndex, Char columnIndex);

  CellState getCellState(Char rowIndex, Char columnIndex);
};

////////////////////////////////////////////////////////////

Long getGridStateCount(Char order);

Long getGridStateIndex(const Grid &grid);

////////////////////////////////////////////////////////////

const Bool TRUE = true;
const Bool FALSE = false;

const Char LOWER_SURVIVAL = 2;
const Char UPPER_SURVIVAL = 3;
const Char LOWER_BIRTH = 3;
const Char UPPER_BIRTH = 3;

const Long COUT_PERIOD = pow(2, 23) - 1;
const auto COUT_WIDTH = setw(12);
const auto COUT_PRECISION = setprecision(4);

////////////////////////////////////////////////////////////

#endif // GAME_H
