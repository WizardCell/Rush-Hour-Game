//
// Created by MM on 06/04/2024.
//

#ifndef OOP5_LIST_H
#define OOP5_LIST_H

template<typename... TT>
struct List;

template<typename Head, typename... Tail>
struct List<Head, Tail...>
{
    typedef Head head;
    typedef List<Tail...> next;
    constexpr static int size = 1 + sizeof...(Tail);
};

template<>
struct List<>
{
    constexpr static int size = 0;
};

template<typename T, typename List>
struct PrependList;

template<typename T, typename... TT>
struct PrependList<T, List<TT...>>
{
    typedef List<T, TT...> list;
};

template<int N, typename List>
struct GetAtIndex;

template<int N, typename T, typename... TT>
struct GetAtIndex<N, List<T, TT...>>
{
    typedef typename GetAtIndex<N - 1, List<TT...>>::value value;
};

template<typename T, typename... TT>
struct GetAtIndex<0, List<T, TT...>>
{
    typedef T value;
};

template<int N, typename ToAdd, typename List>
struct SetAtIndex;

template<int N, typename ToAdd, typename T, typename... TT>
struct SetAtIndex<N, ToAdd, List<T, TT...>>
{
    typedef typename PrependList<T, typename SetAtIndex<N - 1, ToAdd, List<TT...>>::list>::list list;
};

template<typename ToAdd, typename ToRemove, typename... TT>
struct SetAtIndex<0, ToAdd, List<ToRemove, TT...>>
{
    typedef List<ToAdd, TT...> list;
};

#endif //OOP5_LIST_H
