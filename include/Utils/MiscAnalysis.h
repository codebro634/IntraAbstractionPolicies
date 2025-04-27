#pragma once

#ifndef MISCANALYSIS_H
#define MISCANALYSIS_H

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ValueIteration.h"
#include "../../include/Agents/Agent.h"

namespace MISC
{
    void measureIntraAbsRate();


    template <class T>
struct PointedHash
    {
        size_t operator()(const T* p) const
        {
            return p->hash();
        }
    };

    template <class T>
    struct PointedCompare
    {
        bool operator()(const T* lhs, const T* rhs) const
        {
            return lhs == rhs || *lhs == *rhs;
        }
    };

    template <class T>
    using Set = std::unordered_set<T*, PointedHash<T>, PointedCompare<T>>;

}


#endif //MISCANALYSIS_H
