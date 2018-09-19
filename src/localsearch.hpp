#ifndef LOCALSEARCH_INCLUDED_HPP
#define LOCALSEARCH_INCLUDED_HPP

#include <cstdlib>
#include <vector>
#include <algorithm>
#include "maxsat.hpp"
#include "msmemory.hpp"
#include "nk.hpp"
#include "onemax.hpp"

bool* localsearch(MaxSat& ms, unsigned int *nbEval = nullptr);
bool* localsearch(NK& nk, unsigned int *nbEval = nullptr);
bool* localsearch(MSmemory& mm, unsigned int *nbEval = nullptr);
bool* localsearch(OneMax& om, unsigned int *nbEval = nullptr);

bool* ils(std::ostream& out, MaxSat& ms, unsigned int cycles, unsigned int *nbEval = nullptr);
bool* ils(std::ostream& out, NK& nk, unsigned int cycles, unsigned int *nbEval = nullptr);
bool* ils(std::ostream& out, MSmemory& mm, unsigned int cycles, unsigned int *nbEval = nullptr);
bool* ils(std::ostream& out, OneMax& om, unsigned int cycles, unsigned int *nbEval = nullptr);

#endif