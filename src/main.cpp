#include <iostream>
#include "maxsat.hpp"

int main(int argc, char **argv)
{
	MaxSat test("uf50-01.cnf");

	bool *s = new bool[test.getN()];

	for(unsigned int i(0); i < test.getN(); ++i)
		s[i] = true;

	for(unsigned int i(0); i < 2000000; ++i)
		test.evaluate(s);		

	std::cout << "11...1 : " << test.evaluate(s) << std::endl;

	delete[] s;

	return 0;
}