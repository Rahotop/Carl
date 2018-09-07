#ifndef LOCALSEARCH_INCLUDED_HPP
#define LOCALSEARCH_INCLUDED_HPP

#include <cstdlib>
#include <vector>
#include <algorithm>
#include "maxsat.hpp"
#include "msmemory.hpp"
#include "fnarray.hpp"

bool* localsearch(MaxSat& ms);
bool* localsearch(MSmemory& mm);
bool* localsearch(const FnArray& fn);
bool* localsearch(FnArrayInc& fn);

bool* ils(MaxSat& ms, unsigned int cycles);

#endif