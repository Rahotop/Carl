#include "hyperheuristic.hpp"

FnArray::FnArray(unsigned int maxsize, unsigned int width, unsigned int n) :	m_n(n),
																				m_maxsize(maxsize),
																				m_width(width),
																				m_size(0),
																				m_steps(0),
																				m_trees(new unsigned int[maxsize*width]),
																				m_not(new bool[maxsize*width]),
																				m_prec(new bool[maxsize]),
																				m_curr(new bool[maxsize]),
																				m_in(new bool[maxsize*n]),
																				m_s(new bool[n]),
																				m_weights(new float[maxsize]),
																				m_fitness(0.)
{
	for(unsigned int i(0); i < m_maxsize*m_width; ++i) m_trees[i] = 0;
}

FnArray::FnArray(const FnArray& fn) :	m_n(fn.m_n),
										m_maxsize(fn.m_maxsize),
										m_width(fn.m_width),
										m_size(fn.m_size),
										m_steps(0),
										m_trees(new unsigned int[fn.m_maxsize*fn.m_width]),
										m_not(new bool[fn.m_maxsize*fn.m_width]),
										m_prec(new bool[fn.m_maxsize]),
										m_curr(new bool[fn.m_maxsize]),
										m_in(new bool[fn.m_maxsize*fn.m_n]),
										m_s(new bool[fn.m_n]),
										m_weights(new float[fn.m_maxsize]),
										m_fitness(0.)
{
	for(unsigned int i(0); i < m_maxsize*m_width; ++i) m_trees[i] = fn.m_trees[i];
	for(unsigned int i(0); i < m_maxsize*m_width; ++i) m_not[i] = fn.m_not[i];
	for(unsigned int i(0); i < m_maxsize*m_n; ++i) m_in[i] = fn.m_in[i];
	for(unsigned int i(0); i < m_maxsize; ++i) m_weights[i] = fn.m_weights[i];
}

FnArray::~FnArray()
{
	delete[] m_trees;
	delete[] m_not;
	delete[] m_prec;
	delete[] m_curr;
	delete[] m_in;
	delete[] m_s;
	delete[] m_weights;
}

void FnArray::add(unsigned int var, bool n)
{
	if(m_size < m_maxsize)
	{
		for(unsigned int i(0); i < m_n; ++i) m_in[i*m_maxsize + m_size] = false;
		m_in[(var-16)*m_maxsize + m_size] = true;

		m_trees[m_size*m_width] = var;
		m_trees[m_size*m_width + 1] = m_n+16;

		m_not[m_size*m_width] = n;
		
		m_weights[m_size] = 1;
		++m_size;
	}
}

void FnArray::addRandom(const std::vector<unsigned int>& fnset)
{
	if(m_size < m_maxsize)
	{
		for(unsigned int i(0); i < m_n; ++i) m_in[i*m_maxsize + m_size] = false;

		unsigned int width = (rand()%((m_width+1)/2)) * 2 + 1;
		unsigned int stack = 0;

		unsigned int subtree = m_size*m_width;
		for(unsigned int i(0); i < width; ++i)
		{
			if((stack-1) == (width-i))
			{
				m_trees[subtree + i] = fnset[rand()%fnset.size()];
				--stack;
			}
			else if((stack < 2) || (rand()%2))
			{
				unsigned int tmp = (rand()%m_n);
				m_trees[subtree + i] = tmp + 16;
				m_in[tmp*m_maxsize + m_size] = true;
				++stack;
			}
			else
			{
				m_trees[subtree + i] = fnset[rand()%fnset.size()];
				--stack;
			}
		}
		if(width < m_width)
		{
			m_trees[subtree + width] = m_n+16;
		}

		for(unsigned int i(0); i < width; ++i) m_not[subtree + i] = rand()%2;

		//m_weights[m_size] = ((float)(rand()%10000))/10000.;
		m_weights[m_size] = 1.;
		++m_size;
	}
}

void FnArray::deleteRandom()
{
	if(m_size)
		deleteTree(rand()%m_size);
}

void FnArray::deleteTree(unsigned int tree)
{
	--m_size;
	if(m_size)
	{
		unsigned int replace = tree*m_width;
		unsigned int end = m_size*m_width;

		for(unsigned int i(0); i < m_width; ++i) m_trees[replace + i] = m_trees[end + i];
		for(unsigned int i(0); i < m_width; ++i) m_not[replace + i] = m_not[end + i];

		for(unsigned int i(0); i < m_n; ++i) m_in[i*m_n + tree] = m_in[i*m_n + m_size];
		m_weights[tree] = m_weights[m_size];
	}
}

void FnArray::mutate(unsigned int tree, const std::vector<unsigned int>& fnset)
{
	unsigned int width = 0;
	for(;m_trees[tree*m_width + width] < m_n+16 && width < m_width; ++width);
/*
	if(!rand()%(width+1))
	{
		m_weights[tree] += (float)(rand()%1000)/10000. - 0.05;
	}
	else*/
	{
		unsigned int mutate = rand()%width;

		if(rand()%2)
		{
			m_not[tree*m_width + mutate] = !m_not[tree*m_width + mutate];
		}
		else
		{
			m_not[tree*m_width + mutate] = rand()%2;
			if(m_trees[tree*m_width + mutate] < 16)
			{
				if(rand()%2)
				{
					m_trees[tree*m_width + mutate] = fnset[rand()%fnset.size()];
				}
				else
				{
					unsigned int *tmp = new unsigned int[m_width];
					bool *tmpn = new bool[m_width];

					unsigned int tmp2 = 2;
					unsigned int start = mutate;
					while(tmp2)
					{
						--start;
						tmp2 += m_trees[tree*m_width + start] < 16;
						tmp2 -= m_trees[tree*m_width + start] >= 16;
					}

					unsigned int save = (rand()%(mutate-start))+start;

					tmp2 = 2*(m_trees[tree*m_width + save] < 16);
					unsigned int start2 = save;
					while(tmp2)
					{
						--start2;
						tmp2 += m_trees[tree*m_width + start2] < 16;
						tmp2 -= m_trees[tree*m_width + start2] >= 16;
					}

					unsigned int i(0);
					for(; i < start; ++i) tmp[i] = m_trees[tree*m_width + i];
					for(unsigned int j(start2); j <= save; ++i,++j) tmp[i] = m_trees[tree*m_width + j];
					for(unsigned int j(1); j+mutate < m_width; ++i,++j) tmp[i] = m_trees[tree*m_width + mutate+j];
					if(i < m_width)
						tmp[i] = m_n+16;

					i = 0;
					for(; i < start; ++i) tmpn[i] = m_not[tree*m_width + i];
					for(unsigned int j(start2); j <= save; ++i,++j) tmpn[i] = m_not[tree*m_width + j];
					for(unsigned int j(1); j+mutate < m_width; ++i,++j) tmpn[i] = m_not[tree*m_width + mutate+j];


					for(unsigned int j(0); j < m_width; ++j) m_trees[tree*m_width + j] = tmp[j];
					for(unsigned int i(0); i < m_width; ++i) m_not[tree*m_width + i] = tmpn[i];
				}
			}
			else
			{
				if(rand()%2 || m_size == m_maxsize)
				{
					m_trees[tree*m_width + mutate] = rand()%m_n + 16;
				}
				else
				{
					unsigned int *tmp = new unsigned int[m_width];
					bool *tmpn = new bool[m_width];
					for(unsigned int i(0); i < mutate; ++i) tmp[i] = m_trees[tree*m_width + i];
					for(unsigned int i(0); i < mutate; ++i) tmpn[i] = m_not[tree*m_width + i];
					if(rand()%2)
					{
						tmp[mutate] = m_trees[tree*m_width + mutate];
						tmp[mutate+1] = rand()%m_n + 16;

						tmpn[mutate] = m_not[tree*m_width + mutate];
						tmpn[mutate+1] = rand()%2;
					}
					else
					{
						tmp[mutate] = rand()%m_n + 16;
						tmp[mutate+1] = m_trees[tree*m_width + mutate];

						tmpn[mutate] = rand()%2;
						tmpn[mutate+1] = m_not[tree*m_width + mutate];
					}
					tmp[mutate+2] = fnset[rand()%fnset.size()];
					tmpn[mutate+2] = rand()%2;
					for(unsigned int i(mutate+1); i < m_width-2; ++i) tmp[i+2] = m_trees[tree*m_width + i];
					for(unsigned int i(mutate+1); i < m_width-2; ++i) tmpn[i+2] = m_not[tree*m_width + i];

					for(unsigned int i(0); i < m_width; ++i) m_trees[tree*m_width + i] = tmp[i];
					for(unsigned int i(0); i < m_width; ++i) m_not[tree*m_width + i] = tmpn[i];
				}

				for(unsigned int i(0); i < m_n; ++i) m_in[i*m_maxsize + tree] = false;
				for(unsigned int i(0); i < width; ++i) if(m_trees[tree*m_width + i] > 15) m_in[(m_trees[tree*m_width + i]-16)*m_maxsize + tree] = true;
			}
		}
	}
}

float FnArray::evaluate(bool *s)
{
	float sum = 0;
	unsigned int end = m_n+16;

	bool *tmp = new bool[m_width];

	for(unsigned int i(0); i < m_size; ++i)
	{
		unsigned int stack = 0;
		unsigned int index = i*m_width;

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
		sum += tmp[0] * m_weights[i];
		m_prec[i] = tmp[0];
	}

	delete[] tmp;
	return sum;
}

float FnArray::evaluateinc(bool *s, unsigned int changed)
{
	float sum = 0;
	unsigned int end = m_n+16;

	bool *tmp = new bool[m_width];

	unsigned int in = changed*m_maxsize;
	for(unsigned int i(0); i < m_size; ++i)
	{
		unsigned int index = i*m_width;
		tmp[0] = m_prec[i];

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
		sum += tmp[0] * m_weights[i];
		m_curr[i] = tmp[0];
	}

	delete[] tmp;
	return sum;
}

void FnArray::acceptCurr()
{
	++m_steps;
	for(unsigned int i(0); i < m_size; ++i)
	{
		m_prec[i] = m_curr[i];
	}
}

bool* FnArray::ls(bool *s, unsigned int *nbEval)
{
	for(unsigned int i(0); i < m_n; ++i) m_s[i] = s[i];
	float score = evaluate(m_s);
	unsigned int tmpEval = 1;

	std::vector<unsigned int> next(m_n,0);
	for(unsigned int i(0); i < m_n; ++i) //next.push_back(i);
		next[i] = i;

	bool improved = true;

	while(improved)
	{
		std::random_shuffle(next.begin(), next.end());
		improved = false;

		for(unsigned int i(0); i < m_n; ++i)
		{
			m_s[next[i]] = !m_s[next[i]];
			float tmp = evaluateinc(m_s, next[i]);
			++tmpEval;

			if(tmp > score)
			{
				score = tmp;
				improved = true;
				acceptCurr();
				break;
			}
			else
			{
				m_s[next[i]] = !m_s[next[i]];
			}
		}
	}

	if(nbEval)
		*nbEval += tmpEval;

	return m_s;
}

void FnArray::show(std::ostream& out)
{
	out << m_size << " " << m_width << std::endl << std::endl;
	for(unsigned int i(0); i < m_size; ++i)
	{
		out << m_weights[i] << std::endl;
		for(unsigned int j(0); j < m_width; ++j)
		{
			out << m_trees[i*m_width+j] << " ";
		}
		out << std::endl;
		for(unsigned int j(0); j < m_width; ++j)
		{
			out << m_not[i*m_width+j] << " ";
		}
		out << std::endl << std::endl;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


HyperHeuritic::HyperHeuritic(unsigned int maxsize, unsigned int width) : 	m_fnset({1,7,9,11,13}),
																			m_n(0),
																			m_s(nullptr),
																			m_maxsize(maxsize),
																			m_width(width),
																			m_pop()
{

}

HyperHeuritic::~HyperHeuritic()
{
	for(unsigned int i(0); i < m_pop.size(); ++i)
	{
		delete m_pop[i];
	}
	if(m_s)
		delete[] m_s;
}

void HyperHeuritic::initrand(unsigned int newSize)
{
	m_pop.push_back(new FnArray(m_maxsize, m_width, m_n));
	for(unsigned int i(0); i < newSize; ++i) m_pop.back()->addRandom(m_fnset);
}

void HyperHeuritic::initall(unsigned int newSize)
{
	newSize = newSize;
	m_pop.push_back(new FnArray(m_maxsize, m_width, m_n));
	for(unsigned int i(0); i < m_n; ++i) m_pop.back()->add(i+16,rand()%2);
}

void HyperHeuritic::mut1(unsigned int newSize)
{
	newSize = newSize;
	m_pop.push_back(new FnArray(*m_pop.back()));
	m_pop.back()->mutate(rand()%m_pop.back()->size(), m_fnset);
}

void HyperHeuritic::mut2(unsigned int newSize)
{
	newSize = newSize;
	m_pop.push_back(new FnArray(*m_pop.back()));
	m_pop.back()->mutate(rand()%m_pop.back()->size(), m_fnset);
	m_pop.back()->mutate(rand()%m_pop.back()->size(), m_fnset);
}

void HyperHeuritic::mut3(unsigned int newSize)
{
	newSize = newSize;
	m_pop.push_back(new FnArray(*m_pop.back()));
	m_pop.back()->mutate(rand()%m_pop.back()->size(), m_fnset);
	m_pop.back()->mutate(rand()%m_pop.back()->size(), m_fnset);
	m_pop.back()->mutate(rand()%m_pop.back()->size(), m_fnset);
}

void HyperHeuritic::mut1add(unsigned int newSize)
{
	newSize = newSize;
	m_pop.push_back(new FnArray(*m_pop.back()));
	if(!(rand()%3))
		m_pop.back()->mutate(rand()%m_pop.back()->size(), m_fnset);
	else if(rand()%2 || m_pop.back()->size())
		m_pop.back()->addRandom(m_fnset);
	else
		m_pop.back()->deleteRandom();
}

void HyperHeuritic::mutadd(unsigned int newSize)
{
	newSize = newSize;
	m_pop.push_back(new FnArray(*m_pop.back()));
	m_pop.back()->addRandom(m_fnset);
}

void HyperHeuritic::mutnew(unsigned int newSize)
{
	m_pop.push_back(new FnArray(m_maxsize, m_width, m_n));
	for(unsigned int i(0); i < newSize; ++i) m_pop.back()->addRandom(m_fnset);
}

bool HyperHeuritic::sup()
{
	return m_pop.back()->getfitness() > m_pop[m_pop.size()-2]->getfitness();
}

bool HyperHeuritic::supeq()
{
	return m_pop.back()->getfitness() >= m_pop[m_pop.size()-2]->getfitness();
}















