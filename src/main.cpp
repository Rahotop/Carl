#include <iostream>
#include <cstdlib>
#include <ctime>
#include "fnarray.hpp"
#include "maxsat.hpp"
#include "localsearch.hpp"
#include "msalgogen.hpp"

int main(int argc, char **argv)
{
	srand(time(nullptr));

	MaxSat test("uf50-01.cnf");

	MSalgogenincpar algo(100,100,15);

	algo.run(test, 5, 1000);


	//msalgogeninc(test, 100, 15, 5, 1000, 100);

	//MSmemory test("uf50-01.cnf", 100);
/*
	bool *s = new bool[test.getN()];

	for(unsigned int i(0); i < test.getN(); ++i)
		s[i] = rand()%2;

	for(unsigned int i = 0; i < 10000000; ++i)
		test.evaluate(s);*/

	//bool *s = localsearch(test);

	//std::cout << "11...1 : " << test.evaluate(s) << " - nbeval : " << test.getnbeval() << std::endl;


/*
	FnArrayInc test(400,15,256,{1,7,9,11,13});

	for(unsigned int i(0); i < 400; ++i)
		test.addRandom();

	for(unsigned int i(0); i < 1000; ++i)
	{
		delete[] localsearch(test);
	}

	bool *s = localsearch(test);

	std::cout << test.evaluate(s) << std::endl;*/

	//delete[] s;

/*
	bool *s = new bool[256];
	for(unsigned int i(0); i < 256; ++i)
		s[i] = rand()%2;

	for(unsigned int i = 0; i < 1000000; ++i)
		test.evaluate(s);*/


	return 0;
}