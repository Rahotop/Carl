#ifndef LOCALSEARCH_INCLUDED_HPP
#define LOCALSEARCH_INCLUDED_HPP

#include <cstdlib>
#include <vector>
#include <algorithm>
#include "maxsat.hpp"
#include "msmemory.hpp"
#include "fnarray.hpp"
#include "nk.hpp"

bool* localsearch(MaxSat& ms, unsigned int *nbEval = nullptr);
bool* localsearch(NK& nk, unsigned int *nbEval = nullptr);
bool* localsearch(MSmemory& mm, unsigned int *nbEval = nullptr);
bool* localsearch(const FnArray& fn, unsigned int *nbEval = nullptr);
bool* localsearch(FnArrayInc& fn, unsigned int *nbEval = nullptr);

bool* ils(MaxSat& ms, unsigned int cycles, unsigned int *nbEval = nullptr);
bool* ils(NK& nk, unsigned int cycles, unsigned int *nbEval = nullptr);
bool* ils(MSmemory& mm, unsigned int cycles, unsigned int *nbEval = nullptr);

#endif