#include <iostream>
#include <cstdlib>
#include <ctime>
#include "fnarray.hpp"

int main(int argc, char **argv)
{
	//srand(123456);

	//MaxSat test("uf50-01.cnf");
	/*MSmemory test("uf50-01.cnf", 100);

	bool *s = new bool[test.getN()*1000];

	for(unsigned int i(0); i < test.getN()*1000; ++i)
		s[i] = rand()%2;

	for(unsigned int i = 0; i < 10000000; ++i)
		test.evaluate(&s[rand()%1000]);

	std::cout << "11...1 : " << test.evaluate(s) << " - nbeval : " << test.getnbeval() << std::endl;

	delete[] s;*/


	FnArray test(400,15,128,{1,7,9,11,13});

	for(unsigned int i(0); i < 400; ++i)
		test.addRandom();

	bool s[128];
	for(unsigned int i(0); i < 128; ++i)
		s[i] = rand()%2;

	#pragma omp parallel for
	for(unsigned int i = 0; i < 1000000; ++i)
		test.evaluate(s);

	std::cout << test.evaluate(s) << std::endl;

	return 0;
}