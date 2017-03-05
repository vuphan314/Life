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

Long getGridStateCount(Char order);

class Game {
private:
  vector<vector<CellState>> NEXT_CELL_STATES;
  Char order, nextOrder;
  Grid grid, nextGrid;
  Image image;

public:
  Game(Char order);

  Float getImageProportion();

  Long getImageSize();

  void setImage(Image &image);

  Long getNextGridStateIndex(Long gridStateIndex,
    Grid &grid, Grid &nextGrid);

  void setGrid(Grid &grid, Long gridStateIndex);

  Long getGridStateIndex(const Grid &grid);

  void setNextGrid(Grid &nextGrid, const Grid &grid);

  CellState getNextCellState(const Grid &grid,
    Char rowIndex, Char columnIndex);

  Char getAliveNeighborCount(const Grid &grid,
    Char rowIndex, Char columnIndex);

  CellState getCellState(const Grid &grid,
    Char rowIndex, Char columnIndex);
};

////////////////////////////////////////////////////////////

#endif // GAME_H
