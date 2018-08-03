#include <iostream>
#include "msmemory.hpp"

int main(int argc, char **argv)
{
	//MaxSat test("uf50-01.cnf");
	MSmemory test("uf50-01.cnf", 1);

	bool *s = new bool[test.getN()];

	for(unsigned int i(0); i < test.getN(); ++i)
		s[i] = true;

	for(unsigned int i(0); i < 10000000; ++i)
		test.evaluate(s);		

	std::cout << "11...1 : " << test.evaluate(s) << " - nbeval : " << test.getnbeval() << std::endl;

	delete[] s;

	return 0;
}