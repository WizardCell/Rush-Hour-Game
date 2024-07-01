//
// Created by MM on 07/04/2024.
//

#ifndef OOP5_UTILITIES_H
#define OOP5_UTILITIES_H

template<bool Eval, typename T, typename F>
struct Conditional
{
    typedef T value;
};

template<typename T, typename F>
struct Conditional<false, T, F>
{
    typedef F value;
};

template<bool eval, int num_if_true, int num_if_false>
struct ConditionalInteger
{
    constexpr static int value = num_if_true;
};

template<int num_if_true, int num_if_false>
struct ConditionalInteger<false, num_if_true, num_if_false>
{
    constexpr static int value = num_if_false;
};

#endif //OOP5_UTILITIES_H
