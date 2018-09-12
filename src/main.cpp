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

	if(argc > 1)
	{
		if("-ms" == std::string(argv[1]))
		{
			MaxSat inst(argv[2]);
			AlgoGen algo(100,400,15);
			algo.run(inst, 5, 10000, argv[3], std::stoi(argv[4]), std::stoi(argv[5]));
		}
		else if("-nk" == std::string(argv[1]))
		{
			NK inst(argv[2]);
			AlgoGen algo(100,400,15);
			algo.run(inst, 5, 10000, argv[3], std::stoi(argv[4]), std::stoi(argv[5]));
		}
	}

	return 0;
}