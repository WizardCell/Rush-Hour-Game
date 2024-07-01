//
// Created by MM on 07/04/2024.
//

#ifndef OOP5_RUSHHOUR_H
#define OOP5_RUSHHOUR_H

#include "List.h"
#include "BoardCell.h"
#include "CellType.h"
#include "Direction.h"
#include "GameBoard.h"
#include "MoveVehicle.h"
#include "Utilities.h"

template<typename gameBoard, typename moves>
struct GetBoardAfterMoves;

template<typename gameBoard, typename move, typename... remainingMoves>
struct GetBoardAfterMoves<gameBoard, List<move, remainingMoves...>>
{
    static constexpr int row = FindCar<move::type, gameBoard>::X_row_idx;
    static constexpr int col = FindCar<move::type, gameBoard>::X_col_idx;
    typedef typename MoveVehicle<gameBoard, row, col, move::direction, move::amount>::board boardAfterSingleMove;
    typedef typename GetBoardAfterMoves<boardAfterSingleMove, List<remainingMoves...>>::boardAfterMoves boardAfterMoves;
};

template<typename gameBoard>
struct GetBoardAfterMoves<gameBoard, List<>>
{
    typedef gameBoard boardAfterMoves;
};

template<typename gameBoard, typename subList, int redCarRow, int colToStartCheckFrom, int remainingCellsToCheck>
struct CheckWinAux
{
    typedef typename GetAtIndex<colToStartCheckFrom, typename subList::value>::value check_cell;
    constexpr static bool value = (check_cell::type == EMPTY || check_cell::type == X) && CheckWinAux<gameBoard, subList, redCarRow, colToStartCheckFrom + 1, remainingCellsToCheck - 1>::value;
};

template<typename gameBoard, typename subList, int redCarRow, int colToStartCheckFrom>
struct CheckWinAux<gameBoard, subList, redCarRow, colToStartCheckFrom, 0>
{
    constexpr static bool value = true;
};

template<typename gameBoard>
struct CheckWin
{
    static constexpr int row = FindCar<CellType::X, gameBoard>::X_row_idx;
    static constexpr int col = FindCar<CellType::X, gameBoard>::X_col_idx;

    typedef GetAtIndex<row, typename gameBoard::board> subList;
    typedef GetAtIndex<col, typename subList::value> cell;
    typedef typename cell::value my_cell;
    static constexpr int last_col = gameBoard::board::head::size - 1;
    static constexpr int car_right_end_col = Dir<LEFT, row, col, my_cell::length>::col_i;

    constexpr static bool result = CheckWinAux<gameBoard, subList, row, car_right_end_col, last_col - car_right_end_col + 1>::value;
};

template<typename gameBoard, typename Moves>
struct CheckSolution
{
    typedef typename GetBoardAfterMoves<gameBoard, Moves>::boardAfterMoves newBoard;
    constexpr static bool result = CheckWin<newBoard>::result;
};

#endif //OOP5_RUSHHOUR_H
