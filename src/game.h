#ifndef GAME_H
#define GAME_H

////////////////////////////////////////////////////////////

using namespace std;

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

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

class Game {
private:
  vector<vector<CellState>> NEXT_CELL_STATES;
  Char ORDER, NEXT_ORDER;
  Long DOMAIN_SIZE, CODOMAIN_SIZE;
  Image image;
  Grid grid, nextGrid;

public:
  Game(Char order);

  Float getImageProportion();

  Long getImageSize();

  void setImage();

  Long getNextGridStateIndex(Long gridStateIndex);

  void setGrid(Long gridStateIndex);

  void setNextGrid();

  CellState getNextCellState(Char rowIndex, Char columnIndex);

  Char getAliveNeighborCount(Char rowIndex, Char columnIndex);

  CellState getCellState(Char rowIndex, Char columnIndex);
};

////////////////////////////////////////////////////////////

Long getGridStateCount(Char order);

Long getGridStateIndex(const Grid &grid);

////////////////////////////////////////////////////////////

#endif // GAME_H
