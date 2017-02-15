#ifndef GAME_H
#define GAME_H

////////////////////////////////////////////////////////////

using Char = unsigned char; // n
using Int = unsigned int; // n^2
using Long = unsigned long long; // 2^n^2

using CellState = bool;
using Row = vector<CellState>;
using Grid = vector<Row>;

using GridState = bool;
using Image = vector<GridState>;

////////////////////////////////////////////////////////////

const Char LOWER_SURVIVAL = 2;
const Char UPPER_SURVIVAL = 3;
const Char LOWER_BIRTH = 3;
const Char UPPER_BIRTH = 3;

////////////////////////////////////////////////////////////

void setImage(Image &image, const Char &order);

void setNextGrid(Grid &nextGrid, const Grid &grid);

CellState getNextCellState(const Grid &grid,
  const Char rowIndex, const Char columnIndex);

Char getAliveNeighborCount(const Grid &grid,
  const Char rowIndex, const Char columnIndex);

CellState isAlive(const Grid &grid,
  const Char rowIndex, const Char columnIndex);

////////////////////////////////////////////////////////////

#endif // GAME_H
