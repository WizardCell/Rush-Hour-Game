//
// Created by MM on 06/04/2024.
//

#ifndef CODE_MOVEVEHICLE_H
#define CODE_MOVEVEHICLE_H

#include "CellType.h"
#include "Direction.h"
#include "GameBoard.h"
#include "Utilities.h"
#include "List.h"
#include "TransposeList.h"

// Find_Car_Helper Class Declaration
// This class recursively iterates over the board's cells and finds the coordinates of a cell containing an end (the front or back) of the car "type" (note that the first end to be found depends on the direction of the search).
// type - the car to find
// type2 - the car on the current cell
// (row, col) - the coordinates of the current cell
// done - search is done
// B - main list of the board
template <CellType type, CellType type2, int row, int col, bool done, typename B>
struct Find_Car_Helper
{
    typedef typename GameBoard<B>::board mainList;
    static constexpr bool last_row = row == mainList::size - 1;
    static constexpr bool last_col = col == mainList::head::size - 1;

    static constexpr bool found = type == type2;
    static constexpr bool last_cell_in_board = last_row && last_col;

    static_assert(!(!found && last_cell_in_board), "Type was not found!");

    // if we're at the last cell in board, and we got past the assert, then it means we found the desired type, so no need to jump to the next row, zero column
    static constexpr int next_row = ConditionalInteger<last_col, ConditionalInteger<last_cell_in_board, row, row + 1>::value, row>::value; // this is the next cell's row
    static constexpr int next_col = ConditionalInteger<last_col, ConditionalInteger<last_cell_in_board, col, 0>::value, col + 1>::value; // this is the next cell's column

    typedef typename GetAtIndex<next_row, mainList>::value next_row_list;
    typedef typename GetAtIndex<next_col, next_row_list>::value next_cell;
    typedef Find_Car_Helper<type, next_cell::type, next_row, next_col, found, mainList> next_helper;

    static constexpr int X_row = ConditionalInteger<found, row, next_helper::X_row>::value;
    static constexpr int X_col = ConditionalInteger<found, col, next_helper::X_col>::value;
};

// Find_Car_Helper Specialization - stopping condition
template <CellType type, CellType type2, int row, int col, typename B>
struct Find_Car_Helper<type, type2, row, col, true, B>
{
    static constexpr int X_row = row;
    static constexpr int X_col = col;
};

// FindCar Class Declaration
// (uses Find_Car_Helper to find car "type" in board "Bo" - read Find_Car_Helper for more info)
template<CellType type, typename Bo>
struct FindCar
{
    typedef Bo game_board;
    typedef typename game_board::board mainList;

    typedef typename GetAtIndex<0, typename mainList::head>::value first_cell;

    typedef Find_Car_Helper<type, first_cell::type, 0, 0, false, mainList> car_loc;
    static constexpr int X_row_idx = car_loc::X_row;
    static constexpr int X_col_idx = car_loc::X_col;
};

// Dir Class Declaration
// This class computes the further end of a car respect to "c" given the end found using FindCar.
// (e.g. if "c"=RIGHT, the further end of the car is the left end...)
// c - a direction
// (Ro, Col) - a random end of the car
// len - the car's length
template<Direction c, int Ro, int Col, int len>
struct Dir {};

// Dir Specialization (you need to implement more specializations similarly)
template<int Ro, int Col, int len>
struct Dir<RIGHT, Ro, Col, len>
{
    static constexpr int row_i = Ro;
    static constexpr int col_i = Col + len - 1;
};

template<int Ro, int Col, int len>
struct Dir<LEFT, Ro, Col, len>
{
    static constexpr int row_i = Ro;
    static constexpr int col_i = Col + len - 1;
};

template<int Ro, int Col, int len>
struct Dir<UP, Ro, Col, len>
{
    static constexpr int row_i = Ro + len - 1;
    static constexpr int col_i = Col;
};

template<int Ro, int Col, int len>
struct Dir<DOWN, Ro, Col, len>
{
    static constexpr int row_i = Ro + len - 1;
    static constexpr int col_i = Col;
};

// direct Class Declaration
// This class recursively moves a car "Am" steps on the board in a certain direction
// d - the direction of the movement
// counter - recursive counter (remaining amount of steps)
// myL - main list of the board
// my_cell - a cell on the current board containing the car to be moved
// (Ro1, Co1) - coordinates of the closer (leftist/upper) end of the car respect to (0,0) cell
// (Ro2, Co2) - coordinates of the further (rightist/lower) end of the car respect to (0,0) cell
template<Direction d, int counter, typename myL, typename my_cell, int Ro1, int Co1, int Ro2, int Co2>
struct direct {};

// direct Specialization (you need to implement more specializations similarly)
template<int counter, typename myL, typename my_cell, int Ro1, int Co1, int Ro2, int Co2>
struct direct<RIGHT, counter, myL, my_cell, Ro1, Co1, Ro2, Co2>
{
    typedef typename direct<RIGHT, counter - 1, myL, my_cell, Ro1, Co1, Ro2, Co2>::moved mainList; // main list of the board after we moved the car "count"-1 steps
    typedef typename GetAtIndex<Ro1, mainList>::value subList;
    typedef typename GetAtIndex<Co2 + counter, subList>::value celli; // this is the cell we want to move the car to
    static_assert(celli::type == EMPTY, "Error, Collision cell MoveVehicle");
    typedef BoardCell<EMPTY, RIGHT, 0> empty_cell;
    typedef typename SetAtIndex<Co1 + counter - 1, empty_cell, subList>::list first; // first we empty a cell the car was in
    typedef typename SetAtIndex<Co2 + counter, my_cell, first>::list second; // second we fill a cell the car moved to
    typedef typename SetAtIndex<Ro1, second, mainList>::list LL;
    typedef LL moved;
};

template<int counter, typename myL, typename my_cell, int Ro1, int Co1, int Ro2, int Co2>
struct direct<LEFT, counter, myL, my_cell, Ro1, Co1, Ro2, Co2>
{
    typedef typename direct<LEFT, counter - 1, myL, my_cell, Ro1, Co1, Ro2, Co2>::moved mainList; // main list of the board after we moved the car "count"-1 steps
    typedef typename GetAtIndex<Ro1, mainList>::value subList;
    typedef typename GetAtIndex<Co1 - counter, subList>::value celli; // this is the cell we want to move the car to
    static_assert(celli::type == EMPTY, "Error, Collision cell MoveVehicle");
    typedef BoardCell<EMPTY, RIGHT, 0> empty_cell;
    typedef typename SetAtIndex<Co2 - counter + 1, empty_cell, subList>::list first; // first we empty a cell the car was in
    typedef typename SetAtIndex<Co1 - counter, my_cell, first>::list second; // second we fill a cell the car moved to
    typedef typename SetAtIndex<Ro1, second, mainList>::list LL;
    typedef LL moved;
};

template<int counter, typename myL, typename my_cell, int Ro1, int Co1, int Ro2, int Co2>
struct direct<UP, counter, myL, my_cell, Ro1, Co1, Ro2, Co2>
{
    // UP direction is like LEFT direction in a transposed matrix
    typedef typename Transpose<myL>::matrix myL_transposed;
    typedef typename direct<LEFT, counter, myL_transposed, my_cell, Co1, Ro1, Co2, Ro2>::moved myL_transposed_moved;
    typedef typename Transpose<myL_transposed_moved>::matrix moved;
};

template<int counter, typename myL, typename my_cell, int Ro1, int Co1, int Ro2, int Co2>
struct direct<DOWN, counter, myL, my_cell, Ro1, Co1, Ro2, Co2>
{
    // DOWN direction is like RIGHT direction in a transposed matrix
    typedef typename Transpose<myL>::matrix myL_transposed;
    typedef typename direct<RIGHT, counter, myL_transposed, my_cell, Co1, Ro1, Co2, Ro2>::moved myL_transposed_moved;
    typedef typename Transpose<myL_transposed_moved>::matrix moved;
};

// direct Specialization (you need to implement more specializations similarly)
template<typename myL, typename my_cell, int Ro1, int Co1, int Ro2, int Co2>
struct direct<RIGHT, 0, myL, my_cell, Ro1, Co1, Ro2, Co2>
{
    typedef myL moved;
};

template<typename myL, typename my_cell, int Ro1, int Co1, int Ro2, int Co2>
struct direct<LEFT, 0, myL, my_cell, Ro1, Co1, Ro2, Co2>
{
    typedef myL moved;
};

// Move Class Declaration
template<CellType pVehicleType, Direction pDirection, int pAmount>
struct Move
{
    static_assert(pVehicleType != EMPTY, "Error, empty cell Move");

    constexpr static CellType type = pVehicleType;
    constexpr static Direction direction = pDirection;
    constexpr static int amount = pAmount;
};

// MoveVehicle Class Declaration
template<typename gameBoard, int R, int C, Direction D, int A>
struct MoveVehicle {};

// MoveVehicle Specialization
template<typename B, int R1, int C1, Direction Dl, int A>
struct MoveVehicle<GameBoard<B>, R1, C1, Dl, A>
{
    typedef GameBoard<B> PrevBoard;
    typedef typename PrevBoard::board mainList;

    static_assert(R1 >= 0 && R1 < PrevBoard::length, "Error Row, Move");
    static_assert(C1 >= 0 && C1 < PrevBoard::width, "Error column, Move");

    typedef GetAtIndex<R1, mainList> subList;
    typedef GetAtIndex<C1, typename subList::value> cell;
    typedef typename cell::value my_cell;

    static_assert(my_cell::type != EMPTY, "Error, empty cell MoveVehicle");
    static_assert((my_cell::direction <= DOWN && Dl <= DOWN) || (my_cell::direction > DOWN && Dl > DOWN), "Error, direction cell MoveVehicle");

    static constexpr int R2 = FindCar<my_cell::type, PrevBoard>::X_row_idx;
    static constexpr int C2 = FindCar<my_cell::type, PrevBoard>::X_col_idx;
    // the further end:
    static constexpr int R3 = Dir<Dl, R2, C2, my_cell::length>::row_i;
    static constexpr int C3 = Dir<Dl, R2, C2, my_cell::length>::col_i;

    typedef typename direct<Dl, A, B, my_cell, R2, C2, R3, C3>::moved o1;

    typedef GameBoard<o1> board;
};

#endif //CODE_MOVEVEHICLE_H
