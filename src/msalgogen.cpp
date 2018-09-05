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


MSalgogenincpar::MSalgogenincpar(unsigned int popsize, unsigned int maxsize, unsigned int width) :	m_fnset({1,7,9,11,13}),
																									m_n(0),

																									m_popsize(popsize),
																									m_maxsize(maxsize),
																									m_width(width),

																									m_size(new unsigned int[popsize]),
																									m_trees(new unsigned int[popsize*maxsize*width]),
																									m_not(new bool[popsize*maxsize*width]),
																									m_weights(new float[popsize*maxsize]),
																									m_fitness(new float[popsize]),

																									m_Gsize(new unsigned int[popsize]),
																									m_Gtrees(new unsigned int[popsize*maxsize*width]),
																									m_Gnot(new bool[popsize*maxsize*width]),
																									m_Gweights(new float[popsize*maxsize])
{
	for(unsigned int i(0); i < popsize; ++i) m_size[i] = 0;
	for(unsigned int i(0); i < popsize; ++i) m_fitness[i] = 0.;

	for(unsigned int i(0); i < popsize; ++i) m_Gsize[i] = 0;
}

MSalgogenincpar::~MSalgogenincpar()
{
	delete[] m_size;
	delete[] m_trees;
	delete[] m_not;
	delete[] m_weights;
	delete[] m_fitness;

	delete[] m_Gsize;
	delete[] m_Gtrees;
	delete[] m_Gnot;
	delete[] m_Gweights;
}

void MSalgogenincpar::run(const MaxSat& ms, unsigned int newSize, unsigned int nbIt)
{
	// PARAM
	m_n = ms.getN();
	unsigned int *sorted = new unsigned int[m_popsize];
	for(unsigned int i(0); i < m_popsize; ++i) sorted[i] = i;


	// INIT POP
	for(unsigned int i(0); i < m_popsize; ++i)
	{
		for(unsigned int j(0); j < newSize; ++j)
		{
			addRandom(i);
		}
	}

	std::ofstream o("test");


	// ALGOGEN
	for(unsigned int it(0); it < nbIt; ++it)
	{
		// NEW POP
		unsigned int cp = m_popsize;
		for(unsigned int i(0); i < m_popsize; ++i)
		{
			if(!(rand()%3))
			{
				//crossover
				crossover(rand()%m_popsize, rand()%m_popsize, i);
			}
			else if(rand()%2)
			{
				//mutation
				if(rand()%2)
				{
					addRandom(rand()%m_popsize, i);
				}
				else
				{
					deleteRandom(rand()%m_popsize, i);
				}
			}
			else
			{
				//copy
				copy(sorted[--cp], i);
			}
		}


		// SWAP
		std::swap(m_size, m_Gsize);
		std::swap(m_trees, m_Gtrees);
		std::swap(m_not, m_Gnot);
		std::swap(m_weights, m_Gweights);


		// EVAL
		//#pragma omp parallel for
		for(unsigned int i = 0; i < m_popsize; ++i)
		{
			float tmp = 0.;
			for(unsigned int j(0); j < 10; ++j)
			{
				bool *s = localsearch(i);
				tmp += ms.evaluate(s);
				delete[] s;
			}
			m_fitness[i] = tmp / 10.;
		}


		// SORT

		bool tmp = true;
		while(tmp)
		{
			tmp = false;
			for(unsigned int i(1); i < m_popsize; ++i)
			{
				if(m_fitness[sorted[i-1]] > m_fitness[sorted[i]])
				{
					std::swap(sorted[i-1], sorted[i]);
					tmp = true;
				}
			}
		}

		// SAVE
		o << it << " ";
		for(unsigned int i(0); i < m_popsize; ++i)
		{
			o << m_fitness[sorted[i]] << " ";
		}
		o << "\n";
	}

	delete[] sorted;
}

void MSalgogenincpar::crossover(unsigned int p1, unsigned int p2, unsigned next)
{
	copy(p1,next);
	for(unsigned int i(0); i < m_size[p1] && i < m_size[p2]; ++i)
	{
		if(rand()%2)
		{
			copy(p2,next,i);
		}
	}
}

void MSalgogenincpar::addRandom(unsigned int index)
{
	if(m_size[index] < m_maxsize)
	{
		unsigned int width = (rand()%((m_width+1)/2)) * 2 + 1;
		unsigned int stack = 0;

		unsigned int subtree = index*m_maxsize*m_width + m_size[index]*m_width;
		for(unsigned int i(0); i < width; ++i)
		{
			if((stack-1) == (width-i))
			{
				m_trees[subtree + i] = m_fnset[rand()%m_fnset.size()];
				--stack;
			}
			else if((stack < 2) || (rand()%2))
			{
				m_trees[subtree + i] = (rand()%m_n) + 16;
				++stack;
			}
			else
			{
				m_trees[subtree + i] = m_fnset[rand()%m_fnset.size()];
				--stack;
			}
		}
		if(width < m_width)
		{
			m_trees[subtree + width] = m_n+16;
		}

		for(unsigned int i(0); i < width; ++i) m_not[subtree + i] = rand()%2;

		m_weights[index*m_maxsize + m_size[index]] = ((float)(rand()%10000))/10000.;
		++m_size[index];
	}
}

void MSalgogenincpar::addRandom(unsigned int index, unsigned int next)
{
	copy(index, next);
	if(m_Gsize[next] < m_maxsize)
	{
		unsigned int width = (rand()%((m_width+1)/2)) * 2 + 1;
		unsigned int stack = 0;

		unsigned int subtree = next*m_maxsize*m_width + m_Gsize[next]*m_width;
		for(unsigned int i(0); i < width; ++i)
		{
			if((stack-1) == (width-i))
			{
				m_Gtrees[subtree + i] = m_fnset[rand()%m_fnset.size()];
				--stack;
			}
			else if((stack < 2) || (rand()%2))
			{
				m_Gtrees[subtree + i] = (rand()%m_n) + 16;
				++stack;
			}
			else
			{
				m_Gtrees[subtree + i] = m_fnset[rand()%m_fnset.size()];
				--stack;
			}
		}
		if(width < m_width)
		{
			m_Gtrees[subtree + width] = m_n+16;
		}

		for(unsigned int i(0); i < width; ++i) m_Gnot[subtree + i] = rand()%2;

		m_Gweights[next*m_maxsize + m_Gsize[next]] = ((float)(rand()%10000))/10000.;
		++m_Gsize[next];
	}
}

void MSalgogenincpar::deleteRandom(unsigned int index, unsigned int next)
{
	if(m_size[index])
		deleteTree(index, next, rand()%m_size[index]);
}

void MSalgogenincpar::deleteTree(unsigned int index, unsigned int next, unsigned int tree)
{
	copy(index, next);

	--m_Gsize[next];
	if(m_Gsize[next])
	{
		unsigned int replace = next*m_maxsize*m_width + tree*m_width;
		unsigned int end = next*m_maxsize*m_width + m_Gsize[next]*m_width;
		for(unsigned int i(0); i < m_width; ++i) m_Gtrees[replace + i] = m_Gtrees[end + i];
		for(unsigned int i(0); i < m_width; ++i) m_Gnot[replace + i] = m_Gnot[end + i];
		m_Gweights[next*m_maxsize + tree] = m_Gweights[next*m_maxsize + m_Gsize[next]];
	}
}

void MSalgogenincpar::copy(unsigned int index, unsigned int next)
{
	m_Gsize[next] = m_size[index];

	unsigned int tmp = m_maxsize*m_width;
	unsigned int subtree = index*tmp;
	unsigned int subtreeG = next*tmp;

	for(unsigned int i(0); i < tmp; ++i) m_Gtrees[subtreeG + i] = m_trees[subtree + i];
	for(unsigned int i(0); i < tmp; ++i) m_Gnot[subtreeG + i] = m_not[subtree + i];

	subtree = index*m_maxsize;
	subtreeG = next*m_maxsize;

	for(unsigned int i(0); i < m_maxsize; ++i) m_Gweights[subtreeG + i] = m_weights[subtree + i];
}

void MSalgogenincpar::copy(unsigned int index, unsigned int next, unsigned int tree)
{
	unsigned int subtree = index*m_maxsize*m_width + tree*m_width;
	unsigned int subtreeG = next*m_maxsize*m_width + tree*m_width;
	for(unsigned int i(0); i < m_width; ++i) m_Gtrees[subtreeG + i] = m_trees[subtree + i];
	for(unsigned int i(0); i < m_width; ++i) m_Gnot[subtreeG + i] = m_not[subtree + i];

	m_Gweights[next*m_maxsize + tree] = m_weights[index*m_maxsize + tree];
}

float MSalgogenincpar::evaluate(unsigned int ind, bool *s)
{
	float sum = 0;
	unsigned int end = m_n+16;

	bool *tmp = new bool[m_width];

	for(unsigned int i(0); i < m_size[ind]; ++i)
	{
		unsigned int stack = 0;
		unsigned int index = ind*m_maxsize*m_width + i*m_width;
		for(unsigned int j(0); j < m_width && m_trees[index+j] < end; ++j)
		{
			unsigned int op = m_trees[index+j];
			bool notop = m_not[index+j];
			if(op >= 16)
			{
				tmp[stack++] = (s[op-16] == notop);
			}
			else
			{
				--stack;
				unsigned int offset = tmp[stack];
				--stack;
				offset += 2*tmp[stack];

				op <<= offset;
				op >>= 3;

				tmp[stack++] = (op%2 == notop);
			}
		}
		sum += tmp[0] * m_weights[ind*m_maxsize + i];
	}

	delete[] tmp;
	return sum;
}

bool* MSalgogenincpar::localsearch(unsigned int index)
{
	bool *s = new bool[m_n];
	for(unsigned int i(0); i < m_n; ++i) s[i] = rand()%2;
	float score = evaluate(index, s);

	std::vector<unsigned int> next;
	for(unsigned int i(0); i < m_n; ++i) next.push_back(i);

	bool improved = true;

	while(improved)
	{
		std::random_shuffle(next.begin(), next.end());
		improved = false;

		for(unsigned int i(0); i < m_n; ++i)
		{
			s[next[i]] = !s[next[i]];
			float tmp = evaluate(index, s);

			if(tmp > score)
			{
				score = tmp;
				improved = true;
				break;
			}
			else
			{
				s[next[i]] = !s[next[i]];
			}
		}
	}

	return s;
}

void MSalgogenincpar::show(unsigned int index)
{
	std::cout << m_size[index] << std::endl;
	for(unsigned int i(0); i < m_size[index]; ++i)
	{
		std::cout << m_weights[index*m_maxsize + i] << std::endl;

		for(unsigned int j(0); j < m_width; ++j) std::cout << m_trees[index*m_maxsize*m_width + i*m_width + j] << " ";
		std::cout << std::endl;
		for(unsigned int j(0); j < m_width; ++j) std::cout << m_not[index*m_maxsize*m_width + i*m_width + j] << " ";
		std::cout << std::endl << std::endl;
	}
}









