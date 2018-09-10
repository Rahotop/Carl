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
																									m_prec(new bool[popsize*maxsize]),
																									m_curr(new bool[popsize*maxsize]),
																									m_in(nullptr),
																									m_weights(new float[popsize*maxsize]),
																									m_fitness(new float[popsize]),

																									m_Gsize(new unsigned int[popsize]),
																									m_Gtrees(new unsigned int[popsize*maxsize*width]),
																									m_Gnot(new bool[popsize*maxsize*width]),
																									m_Gin(nullptr),
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
	delete[] m_prec;
	delete[] m_curr;
	if(m_in)
		delete[] m_in;
	delete[] m_weights;
	delete[] m_fitness;

	delete[] m_Gsize;
	delete[] m_Gtrees;
	delete[] m_Gnot;
	if(m_Gin)
		delete[] m_Gin;
	delete[] m_Gweights;
}

void MSalgogenincpar::run(MaxSat& ms, unsigned int newSize, unsigned int nbIt)
{
	// PARAM
	m_n = ms.getN();
	unsigned int *sorted = new unsigned int[m_popsize];
	for(unsigned int i(0); i < m_popsize; ++i) sorted[i] = i;
	m_in = new bool[m_popsize*m_maxsize*m_n];
	m_Gin = new bool[m_popsize*m_maxsize*m_n];


	// INIT POP
	for(unsigned int i(0); i < m_popsize; ++i)
	{
		for(unsigned int j(0); j < newSize; ++j)
		{
			addRandom(i);
		}
	}


	// OUTPUT
	std::ofstream data("data");
	std::ofstream res("results.txt");
	unsigned int nbEval = 0;


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
				if(!(rand()%3))
				{
					addRandom(rand()%m_popsize, i);
				}
				else if(rand()%2)
				{
					deleteRandom(rand()%m_popsize, i);
				}
				else
				{
					copy(rand()%m_popsize, i);
					if(m_Gsize[i])
						m_Gweights[i*m_maxsize+rand()%m_Gsize[i]] += ((float)(rand()%1000))/10000.-0.05;
					else
						addRandom(rand()%m_popsize, i);
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
		std::swap(m_in, m_Gin);
		std::swap(m_weights, m_Gweights);


		// EVAL
		unsigned int meanNbEval = 0;
		//#pragma omp parallel for
		for(unsigned int i = 0; i < m_popsize; ++i)
		{/*
			bool **act = new bool*[10];
			float *score = new float[10];
			float *scoreobj = new float[10];

			for(unsigned int j(0); j < 10; ++j)
			{
				bool *s = localsearchind(i, &meanNbEval);
				score[j] = evaluate(i, s);
				act[j] = getActivation(i);
				scoreobj[j] = ms.evaluate(s);
				delete[] s;
			}


			float *adj = new float[m_size[i]];
			for(unsigned int j(0); j < m_size[i]; ++j) adj[j] = 0.;
			unsigned int count = 0;

			for(unsigned int j(0); j < 10; ++j)
			{
				for(unsigned int k(j+1); k < 10; ++k)
				{
					++count;
					float delta = score[j]/score[k] - scoreobj[j]/scoreobj[k];
					delta /= 2;

					unsigned int jnk = 0;
					unsigned int njk = 0;
					for(unsigned int l(0); l < m_size[i]; ++l)
					{
						jnk += (act[j][l] && !act[k][l]);
						njk += (!act[j][l] && act[k][l]);
					}
					if(jnk) for(unsigned int l(0); l < m_size[i]; ++l) adj[l] += delta/jnk * score[j] * (act[j][l] && !act[k][l]);
					if(njk) for(unsigned int l(0); l < m_size[i]; ++l) adj[l] -= delta/njk * score[j] * (!act[j][l] && act[k][l]);
				}
			}

			for(unsigned int j(0); j < m_size[i]; ++j) m_weights[i*m_maxsize + j] += adj[j]/count;
			for(unsigned int j(0); j < m_size[i]; ++j) m_weights[i*m_maxsize + j] = (m_weights[i*m_maxsize + j] > 1) ? 1 : m_weights[i*m_maxsize + j];
			for(unsigned int j(0); j < m_size[i]; ++j) m_weights[i*m_maxsize + j] = (m_weights[i*m_maxsize + j] < -1) ? -1 : m_weights[i*m_maxsize + j];

			delete[] adj;


			delete[] scoreobj;
			delete[] score;
			delete[] act;*/

			float tmp = 0;
			for(unsigned int j(0); j < 10; ++j)
			{
				bool *s = localsearchind(i, &meanNbEval);
				tmp += ms.evaluate(s);
				delete s;
			}
			m_fitness[i] = tmp/10.;
		}
		nbEval += meanNbEval;
		meanNbEval /= (m_popsize*10);


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
		data << it << " " << m_fitness[sorted[m_popsize-1]] << " " << m_size[sorted[m_popsize-1]] << " " << meanNbEval << "\n";
	}

	// RES
	res << "nb eval (solutions) : " << nbEval << "\nnb eval (f obj) : " << ms.getnbeval() << "\n\n#ILS :\n#Fitness max : ";


	// CORRELATION
	data << "\n";
	for(unsigned int i(0); i < 1000; ++i) //rand
	{
		bool *s = new bool[m_n];
		for(unsigned int j(0); j < m_n; ++j) s[j] = rand()%2;
		data << evaluate(sorted[m_popsize-1], s) << " " << ms.evaluate(s) << "\n";
		delete[] s;
	}
	data << "\n";
	for(unsigned int i(0); i < 1000; ++i) //ls f obj
	{
		bool *s = localsearch(ms);
		data << evaluate(sorted[m_popsize-1], s) << " " << ms.evaluate(s) << "\n";
	}
	data << "\n";
	for(unsigned int i(0); i < 1000; ++i) //ls f ind
	{
		bool *s = localsearchind(sorted[m_popsize-1]);
		data << evaluate(sorted[m_popsize-1], s) << " " << ms.evaluate(s) << "\n";
	}
	data << "\n";

	// ILS
	bool *bestils = nullptr;
	{
		unsigned int nbeval = 0;
		bool *s = ils(ms, 5000, &nbeval);
		std::ofstream ilsout("ils", std::ios::app);
		ilsout << "\n0 " << ms.evaluate(s) << "\n" << nbIt << " " << ms.evaluate(s) << std::endl;
		res << ms.evaluate(s) << "\n#nb eval : " << nbeval;
		data << evaluate(sorted[m_popsize-1], s) << " " << ms.evaluate(s) << "\n\n"; //opt glb
		bestils = s;
	}

	// FREQ
	std::vector<bool*> lsobj;
	std::vector<bool*> lsind;
	std::vector<unsigned int> fobj;
	std::vector<unsigned int> find;

	for(unsigned int i(0); i < 100; ++i)
	{
		bool *s = localsearch(ms);
		bool tmp = true;
		for(unsigned int j(0); j < lsobj.size(); ++j)
		{
			if(!distance(s,lsobj[j],m_n))
			{
				delete[] s;
				++fobj[j];
				tmp = false;
				break;
			}
		}
		if(tmp)
		{
			lsobj.push_back(s);
			fobj.push_back(1);
		}
	}
	for(unsigned int i(0); i < 100; ++i)
	{
		bool *s = localsearchind(sorted[m_popsize-1]);
		bool tmp = true;
		for(unsigned int j(0); j < lsind.size(); ++j)
		{
			if(!distance(s,lsind[j],m_n))
			{
				delete[] s;
				++find[j];
				tmp = false;
				break;
			}
		}
		if(tmp)
		{
			lsind.push_back(s);
			find.push_back(1);
		}
	}

	// SORT
	unsigned int *sortobj = new unsigned int[lsobj.size()];
	for(unsigned int i(0); i < lsobj.size(); ++i) sortobj[i] = i;
	unsigned int *sortind = new unsigned int[lsind.size()];
	for(unsigned int i(0); i < lsind.size(); ++i) sortind[i] = i;

	bool tmp = true;
	while(tmp)
	{
		tmp = false;
		for(unsigned int i(1); i < lsobj.size(); ++i)
		{
			if(ms.evaluate(lsobj[sortobj[i-1]]) < ms.evaluate(lsobj[sortobj[i]]))
			{
				std::swap(sortobj[i-1], sortobj[i]);
				tmp = true;
			}
		}
	}
	tmp = true;
	while(tmp)
	{
		tmp = false;
		for(unsigned int i(1); i < lsind.size(); ++i)
		{
			if(ms.evaluate(lsind[sortind[i-1]]) < ms.evaluate(lsind[sortind[i]]) || (ms.evaluate(lsind[sortind[i-1]]) == ms.evaluate(lsind[sortind[i]]) && find[sortind[i-1]] < find[sortind[i]]))
			{
				std::swap(sortind[i-1], sortind[i]);
				tmp = true;
			}
		}
	}

	// FREQ RES
	res << "\n\nFreq :\n";
	res << std::setw(21) << "Fonction objectif" << " " << std::setw(55) << "Fonction trouvée" << "\n\n";
	res << std::setw(21) << "Fitness moyenne :" << " " << std::setw(54) << "Fitness moyenne :" << "\n";
	float moyennescore = 0;
	for(unsigned int i(0); i < lsobj.size(); ++i) moyennescore += ms.evaluate(lsobj[i]);
	res << std::setw(21) << moyennescore/lsobj.size() << " ";
	moyennescore = 0;
	for(unsigned int i(0); i < lsind.size(); ++i) moyennescore += ms.evaluate(lsind[i]);
	res << std::setw(54) << moyennescore/lsind.size() << "\n\n";
	res << std::setw(10) << "Fitness" << " " << std::setw(10) << "Frequence" << " " << std::setw(10) << "Fitness" << " " << std::setw(10) << "Frequence" << " ";
	res << std::setw(10) << "opt loc" << " " << std::setw(10) << "Dist ils" << " " << std::setw(10) << "Dist best" << "\n";


	for(unsigned int i(0); i < lsobj.size(); ++i)
	{
		res << std::setw(10) << ms.evaluate(lsobj[sortobj[i]]) << " " << std::setw(10)  << fobj[sortobj[i]] << " ";
		if(i < lsind.size())
		{
			res << std::setw(10)  << ms.evaluate(lsind[sortind[i]]) << " " << std::setw(10)  << find[sortind[i]] << " " << std::setw(10) << ms.islocopt(lsind[sortind[i]]);
			res << " " << std::setw(10) << distance(bestils,lsind[sortind[i]],m_n) << " " << std::setw(10) << distance(lsind[sortind[0]],lsind[sortind[i]],m_n);
		}
		res << "\n";
	}

	// BEST
	res << "\n\nFitness max : " << m_fitness[sorted[m_popsize-1]] << "\n";
	show(sorted[m_popsize-1], res);

	delete[] sorted;
	delete[] bestils;
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
		for(unsigned int i(0); i < m_n; ++i) m_in[index*m_maxsize*m_n + i*m_n + m_size[index]] = false;

		unsigned int width = (rand()%((m_width+1)/2)) * 2 + 1;
		unsigned int stack = 0;

		unsigned int indexmaxsizen = index*m_maxsize*m_n;
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
				unsigned int tmp = (rand()%m_n);
				m_trees[subtree + i] = tmp + 16;
				m_in[indexmaxsizen + tmp*m_n + m_size[index]] = true;
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
		for(unsigned int i(0); i < m_n; ++i) m_Gin[next*m_maxsize*m_n + i*m_n + m_Gsize[next]] = false;

		unsigned int width = (rand()%((m_width+1)/2)) * 2 + 1;
		unsigned int stack = 0;

		unsigned int nextmaxsizen = next*m_maxsize*m_n;
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
				unsigned int tmp = (rand()%m_n);
				m_Gtrees[subtree + i] = tmp + 16;
				m_Gin[nextmaxsizen + tmp*m_n + m_Gsize[next]] = true;
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

void MSalgogenincpar::add(unsigned int index, unsigned int tree, unsigned int next)
{
	if(m_Gsize[next] < m_maxsize)
	{
		unsigned int subtree = index*m_maxsize*m_width + tree*m_width;
		unsigned int subtreeG = next*m_maxsize*m_width + m_Gsize[next]*m_width;
		for(unsigned int i(0); i < m_width; ++i) m_Gtrees[subtreeG + i] = m_trees[subtree + i];
		for(unsigned int i(0); i < m_width; ++i) m_Gnot[subtreeG + i] = m_not[subtree + i];

		subtree = index*m_maxsize*m_n;
		subtreeG = next*m_maxsize*m_n;
		for(unsigned int i(0); i < m_n; ++i) m_Gin[subtreeG + i*m_n + m_Gsize[next]] = m_in[subtree + i*m_n + tree];

		m_Gweights[next*m_maxsize + m_Gsize[next]] = m_weights[index*m_maxsize + tree];

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

		replace = next*m_maxsize*m_n;
		end = next*m_maxsize*m_n;
		for(unsigned int i(0); i < m_n; ++i) m_Gin[replace + i*m_n + tree] = m_Gin[end + i*m_n + m_Gsize[next]];
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

	subtree = index*m_maxsize*m_n;
	subtreeG = next*m_maxsize*m_n;
	for(unsigned int i(0); i < m_maxsize*m_n; ++i) m_Gin[subtreeG + i] = m_in[subtree + i];

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

	subtree = index*m_maxsize*m_n;
	subtreeG = next*m_maxsize*m_n;
	for(unsigned int i(0); i < m_n; ++i) m_Gin[subtreeG + i*m_n + tree] = m_in[subtree + i*m_n + tree];

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
		m_prec[ind*m_maxsize + i] = tmp[0];
	}

	delete[] tmp;
	return sum;
}

float MSalgogenincpar::evaluateinc(unsigned int ind, bool *s, unsigned int changed)
{
	float sum = 0;
	unsigned int end = m_n+16;

	bool *tmp = new bool[m_width];

	unsigned int indmaxsize = ind*m_maxsize;
	unsigned int indmaxsizewidth = indmaxsize*m_width;
	unsigned int in = indmaxsize*m_n + changed*m_n;
	for(unsigned int i(0); i < m_size[ind]; ++i)
	{
		unsigned int index = indmaxsizewidth + i*m_width;
		tmp[0] = m_prec[indmaxsize + i];

		if(m_in[in + i])
		{
			unsigned int stack = 0;
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
		}
		sum += tmp[0] * m_weights[indmaxsize + i];
		m_curr[indmaxsize + i] = tmp[0];
	}

	delete[] tmp;
	return sum;
}

void MSalgogenincpar::acceptCurr(unsigned int ind)
{
	for(unsigned int i(0); i < m_size[ind]; ++i)
	{
		m_prec[ind*m_maxsize + i] = m_curr[ind*m_maxsize + i];
	}
}

bool* MSalgogenincpar::localsearchind(unsigned int index, unsigned int *nbEval)
{
	bool *s = new bool[m_n];
	for(unsigned int i(0); i < m_n; ++i) s[i] = rand()%2;
	float score = evaluate(index, s);
	unsigned int tmpEval = 1;

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
			float tmp = evaluateinc(index, s, next[i]);
			++tmpEval;

			if(tmp > score)
			{
				score = tmp;
				improved = true;
				acceptCurr(index);
				break;
			}
			else
			{
				s[next[i]] = !s[next[i]];
			}
		}
	}

	if(nbEval)
		*nbEval += tmpEval;

	return s;
}

bool* MSalgogenincpar::getActivation(unsigned int index) const
{
	bool *act = new bool[m_maxsize];
	for(unsigned int i(0); i < m_maxsize; ++i)
	{
		act[i] = m_prec[index*m_maxsize + i];
	}
	return act;
}

void MSalgogenincpar::show(unsigned int index, std::ostream& out)
{
	out << m_size[index] << std::endl;
	for(unsigned int i(0); i < m_size[index]; ++i)
	{
		out << m_weights[index*m_maxsize + i] << std::endl;

		for(unsigned int j(0); j < m_width; ++j) out << m_trees[index*m_maxsize*m_width + i*m_width + j] << " ";
		out << std::endl;
		for(unsigned int j(0); j < m_width; ++j) out << m_not[index*m_maxsize*m_width + i*m_width + j] << " ";
		out << std::endl << std::endl;
	}
}

unsigned int distance(bool *s1, bool *s2, unsigned int n)
{
	unsigned int tmp = 0;
	for(unsigned int i(0); i < n; ++i)
	{
		tmp += s1[i] != s2[i];
	}
	return tmp;
}









