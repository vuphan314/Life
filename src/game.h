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

using CellState = bool;
using Row = vector<CellState>;
using Grid = vector<Row>;

using GridState = bool;
using Image = vector<GridState>;

////////////////////////////////////////////////////////////

const Long COUT_PERIOD = pow(10, 6);
const auto COUT_WIDTH = setw(12);
const auto COUT_PRECISION = setprecision(4);

const Char LOWER_SURVIVAL = 2;
const Char UPPER_SURVIVAL = 3;
const Char LOWER_BIRTH = 3;
const Char UPPER_BIRTH = 3;

////////////////////////////////////////////////////////////

Float getImageProportion(Char order);

Long getImageSize(Char order);

void setImage(Image &image, Char order);

Long getGridStateCount(Char order);

void setGrid(Grid &grid, Long gridStateIndex);

Long getGridStateIndex(const Grid &grid);

void setNextGrid(Grid &nextGrid, const Grid &grid);

CellState getNextCellState(const Grid &grid,
  Char rowIndex, Char columnIndex);

Char getAliveNeighborCount(const Grid &grid,
  Char rowIndex, Char columnIndex);

CellState getCellState(const Grid &grid,
  Char rowIndex, Char columnIndex);

////////////////////////////////////////////////////////////

#endif // GAME_H
