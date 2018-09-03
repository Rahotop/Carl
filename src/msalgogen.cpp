#include "msalgogen.hpp"

FnArray msalgogen(MaxSat& ms, unsigned int indSize, unsigned int indWidth, unsigned int newSize, unsigned int nbIt, unsigned int popSize)
{
	// INIT POP

	std::vector<FnArray> pop;
	for(unsigned int i(0); i < popSize; ++i)
	{
		pop.push_back(FnArray(indSize, indWidth, ms.getN(), {1,7,9,11,13}));
		for(unsigned int j(0); j < newSize; ++j)
		{
			pop[i].addRandom();
		}
	}

	std::vector<FnArray> newGen;
	for(unsigned int i(0); i < popSize; ++i)
		newGen.push_back(FnArray(indSize, indWidth, ms.getN(), {1,7,9,11,13}));


	// ALGOGEN
	std::ofstream o("test");

	for(unsigned int it(0); it < nbIt; ++it)
	{
		std::cout << it << "    \r" << std::flush;
		// NEW POP

		unsigned int cp = popSize;
		for(unsigned int i(0); i < popSize; ++i)
		{
			if(!(rand()%3))
			{
				newGen[i].crossover(pop[rand()%popSize], pop[rand()%popSize]);
			}
			else if(rand()%2)
			{
				newGen[i] = pop[rand()%popSize];
				if(rand()%2)
				{
					newGen[i].addRandom();
				}
				else
				{
					newGen[i].deleteRandom();
				}
			}
			else
			{
				newGen[i] = pop[--cp];
			}
		}
		pop.swap(newGen);


		// EVAL POP

		for(unsigned int i(0); i < popSize; ++i)
		{
			float score = 0.;
			for(unsigned int j(0); j < 10; ++j)
			{
				bool *s = localsearch(pop[i]);
				score += ms.evaluate(s);
				delete[] s;
			}
			pop[i].setScore(score/10.);
		}

		std::sort(pop.begin(), pop.end());

		o << it << " ";
		for(unsigned int i(0); i < popSize; ++i) o << pop[i].size() << " ";
		//o << pop.back().getScore() << " ";
		o << std::endl;
	}

	return pop.back();
}

FnArrayInc msalgogeninc(MaxSat& ms, unsigned int indSize, unsigned int indWidth, unsigned int newSize, unsigned int nbIt, unsigned int popSize)
{
	// INIT POP

	std::vector<FnArrayInc> pop;
	for(unsigned int i(0); i < popSize; ++i)
	{
		pop.push_back(FnArrayInc(indSize, indWidth, ms.getN(), {1,7,9,11,13}));
		for(unsigned int j(0); j < newSize; ++j)
		{
			pop[i].addRandom();
		}
	}

	std::vector<FnArrayInc> newGen;
	for(unsigned int i(0); i < popSize; ++i)
		newGen.push_back(FnArrayInc(indSize, indWidth, ms.getN(), {1,7,9,11,13}));


	// ALGOGEN
	std::ofstream o("test");

	for(unsigned int it(0); it < nbIt; ++it)
	{
		std::cout << it << "    \r" << std::flush;
		// NEW POP

		unsigned int cp = popSize;
		for(unsigned int i(0); i < popSize; ++i)
		{
			if(!(rand()%3))
			{
				newGen[i].crossover(pop[rand()%popSize], pop[rand()%popSize]);
			}
			else if(rand()%2)
			{
				newGen[i] = pop[rand()%popSize];
				if(rand()%2)
				{
					newGen[i].addRandom();
				}
				else
				{
					newGen[i].deleteRandom();
				}
			}
			else
			{
				newGen[i] = pop[--cp];
			}
		}
		pop.swap(newGen);


		// EVAL POP

		for(unsigned int i(0); i < popSize; ++i)
		{
			float score = 0.;
			for(unsigned int j(0); j < 10; ++j)
			{
				bool *s = localsearch(pop[i]);
				score += ms.evaluate(s);
				delete[] s;
			}
			pop[i].setScore(score/10.);
		}

		std::sort(pop.begin(), pop.end());

		o << it << " ";
		for(unsigned int i(0); i < popSize; ++i)// o << pop[i].size() << " ";
			o << pop[i].getScore() << " ";
		o << std::endl;
	}

	return pop.back();
}