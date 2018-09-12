#include <iostream>
#include <cstdlib>
#include <ctime>
#include "fnarray.hpp"
#include "maxsat.hpp"
#include "localsearch.hpp"
#include "msalgogen.hpp"
#include "algogen.hpp"
#include "msmemory.hpp"
#include "nk.hpp"

int main(int argc, char **argv)
{
	srand(time(nullptr));

	MaxSat test("uf50-01.cnf");
	//NK test("nk_64_6.0");

	AlgoGen algo(100,100,15);

	algo.run(test, 5, 1000,"data");

	return 0;
}