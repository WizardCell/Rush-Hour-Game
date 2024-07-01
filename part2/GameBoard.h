//
// Created by MM on 07/04/2024.
//

#ifndef OOP5_GAMEBOARD_H
#define OOP5_GAMEBOARD_H

template<typename TBoard>
struct GameBoard
{
    typedef TBoard board;
    constexpr static int width = TBoard::head::size;
    constexpr static int length = TBoard::size;
};

#endif //OOP5_GAMEBOARD_H
