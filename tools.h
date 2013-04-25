#ifndef TOOLS_H
#define TOOLS_H

#include <QString>

struct QPairFirstStringComparer
{
    bool operator()(const QPair<QString,QString> & a, const QPair<QString,QString> & b) const
    {
        return a.first.toLower() < b.first.toLower();
    }
};
struct QPairSecondStringComparer
{
    bool operator()(const QPair<QString,QString> & a, const QPair<QString,QString> & b) const
    {
        return a.second.toLower() < b.second.toLower();
    }
};
struct QPairFirstComparer
{
    template<typename T1, typename T2>
    bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const
    {
        return a.first < b.first;
    }
};
struct QPairSecondComparer
{
    template<typename T1, typename T2>
    bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const
    {
        return a.second < b.second;
    }
};
struct QPairFirstComparerInverse
{
    template<typename T1, typename T2>
    bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const
    {
        return a.first > b.first;
    }
};
struct QPairSecondComparerInverse
{
    template<typename T1, typename T2>
    bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const
    {
        return a.second > b.second;
    }
};

#endif // TOOLS_H
