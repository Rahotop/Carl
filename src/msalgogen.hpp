#ifndef MSALGOGEN_HPP_INCLUDED
#define MSALGOGEN_HPP_INCLUDED

#include <vector>
#include <iterator>
#include <algorithm>
#include <fstream>
#include "localsearch.hpp"
#include "maxsat.hpp"
#include "fnarray.hpp"
#include "msmemory.hpp"

FnArray msalgogen(MaxSat& ms, unsigned int indSize, unsigned int indWidth, unsigned int newSize, unsigned int nbIt, unsigned int popSize);
FnArrayInc msalgogeninc(MaxSat& ms, unsigned int indSize, unsigned int indWidth, unsigned int newSize, unsigned int nbIt, unsigned int popSize);

#endif