//
// Created by MM on 11/04/2024
//

#ifndef OOP5_SUBJECT_H
#define OOP5_SUBJECT_H

#include "OOP5EventException.h"
#include "Observer.h"
#include <list>
#include <algorithm>

using namespace std;

template<typename T>
class Subject
{
private:
    list<Observer<T>*> Observers;

public:
    Subject() = default;

    void notify(const T &event)
    {
        for (Observer<T> *i: Observers)
        {
            i->handleEvent(event);
        }
    }

    void addObserver(Observer<T> &observer)
    {
        if (find(Observers.begin(), Observers.end(), &observer) != Observers.end())
        {
            throw ObserverAlreadyKnownToSubject();
        }

        Observers.push_back(&observer);
    }

    void removeObserver(Observer<T> &observer)
    {
        if (find(Observers.begin(), Observers.end(), &observer) == Observers.end())
        {
            throw ObserverUnknownToSubject();
        }

        Observers.remove(&observer);
    }

    Subject<T> &operator+=(Observer<T> &observer)
    {
        this->addObserver(observer);
        return *this;
    }

    Subject<T> &operator-=(Observer<T> &observer)
    {
        this->removeObserver(observer);
        return *this;
    }

    Subject<T> &operator()(const T &observer)
    {
        this->notify(observer);
        return *this;
    }

};

#endif //OOP5_SUBJECT_H
