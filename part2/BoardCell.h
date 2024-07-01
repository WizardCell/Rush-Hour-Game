//
// Created by MM on 07/04/2024.
//

#ifndef OOP5_BOARDCELL_H
#define OOP5_BOARDCELL_H

#include "CellType.h"
#include "Direction.h"

template<CellType pCellType, Direction pDirection, int pLength>
struct BoardCell
{
    constexpr static CellType type = pCellType;
    constexpr static Direction direction = pDirection;
    constexpr static int length = pLength;
};

#endif //OOP5_BOARDCELL_H
