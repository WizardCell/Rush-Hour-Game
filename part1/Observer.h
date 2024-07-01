//
// Created by MM on 11/04/2024
//

#ifndef OOP5_OBSERVER_H
#define OOP5_OBSERVER_H

template<typename T>
class Observer
{
public:
    Observer() = default;
    virtual void handleEvent(const T&) = 0;
};

#endif //OOP5_OBSERVER_H
