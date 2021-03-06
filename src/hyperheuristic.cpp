#include "hyperheuristic.hpp"

FnArray::FnArray(unsigned int maxsize, unsigned int width, unsigned int n) :	m_n(n),
																				m_maxsize(maxsize),
																				m_width(width),
																				m_size(0),
																				m_steps(0),
																				//m_maths(new unsigned int[maxsize+1]),
																				m_trees(new unsigned int[maxsize*width]),
																				m_not(new bool[maxsize*width]),
																				m_prec(new bool[maxsize]),
																				m_curr(new bool[maxsize]),
																				m_in(new bool[maxsize*n]),
																				m_s(new bool[n]),
																				m_weights(new float[maxsize]),
																				m_fitness(0.)
{
	//for(unsigned int i(0); i < m_maxsize+1; ++i) m_maths[i] = rand()%4;
	for(unsigned int i(0); i < m_maxsize*m_width; ++i) m_trees[i] = 0;
	for(unsigned int i(0); i < m_maxsize*m_width; ++i) m_not[i] = 0;
}

FnArray::FnArray(const FnArray& fn) :	m_n(fn.m_n),
										m_maxsize(fn.m_maxsize),
										m_width(fn.m_width),
										m_size(fn.m_size),
										m_steps(0),
										//m_maths(new unsigned int [fn.m_maxsize+1]),
										m_trees(new unsigned int[fn.m_maxsize*fn.m_width]),
										m_not(new bool[fn.m_maxsize*fn.m_width]),
										m_prec(new bool[fn.m_maxsize]),
										m_curr(new bool[fn.m_maxsize]),
										m_in(new bool[fn.m_maxsize*fn.m_n]),
										m_s(new bool[fn.m_n]),
										m_weights(new float[fn.m_maxsize]),
										m_fitness(0.)
{
	//for(unsigned int i(0); i < m_maxsize+1; ++i) m_maths[i] = fn.m_maths[i];
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
		
		m_weights[m_size] = ((float)(rand()%10000))/10000.;
		//m_weights[m_size] = 1;
		++m_size;
	}
}

void FnArray::addRandom(const std::vector<unsigned int>& fnset)
{
	if(m_size < m_maxsize)
	{
		for(unsigned int i(0); i < m_n; ++i) m_in[i*m_maxsize + m_size] = false;

		unsigned int width = (rand()%((m_width+1)/2)) * 2 + 1;
		//unsigned int width = ((rand()%(((m_width/2)+1)/2)) * 2 + 1)*2;
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

		m_weights[m_size] = ((float)(rand()%10000))/10000.;
		//m_weights[m_size] = 1.;
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

unsigned int FnArray::mutate(const std::vector<unsigned int>& fnset)
{
	if(m_size)
	{
		unsigned int tmp = 1+rand()%8;
		if(tmp == 1)
			changeweight();
		else if(tmp == 2)
			swapweights();
		else if(tmp == 3)
			changesign();
		else if(tmp == 4)
			changeop(fnset);
		else if(tmp == 5)
			changevar();
		else if(tmp == 6)
			swapvar();
		else if(tmp == 7)
			addvar(fnset);
		else if(tmp == 8)
			delvar();/*
		else
			m_maths[rand()%(m_maxsize+1)] = rand()%4;*/
		return tmp;
	}
	return 0;
}

float FnArray::propSat(bool *s) const
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
		sum += tmp[0];
	}
	sum /= (float)m_size;

	delete[] tmp;
	return sum;
}

float FnArray::evaluate(bool *s)
{
	float sum = 0;
	unsigned int end = m_n+16;

	bool *tmp = new bool[m_width];
	//float *tmpm = new float[m_maxsize*2+1];
	//unsigned int stackm = 0;

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
		//tmpm[stackm++] = tmp[0] * m_weights[i];
		sum += tmp[0] * m_weights[i];
		m_prec[i] = tmp[0];
	}
/*
	for(unsigned int i(0); stackm > 1; ++i)
	{
		--stackm;
		float v1 = tmpm[stackm];
		--stackm;
		float v2 = tmpm[stackm];

		if(m_maths[i] == 0)
			tmpm[stackm++] = v1 + v2;
		else if(m_maths[i] == 1)
			tmpm[stackm++] = v1 * v2;
		else if(m_maths[i] == 2)
			tmpm[stackm++] = (v2 == 0) ? 0 : v1 / v2;
		else if(m_maths[i] == 3)
			tmpm[stackm++] = v1 - v2;
	}

	sum = tmpm[0];
	delete[] tmpm;*/

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
		//	float tmp = evaluate(m_s);
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

unsigned int FnArray::getnbvar() const
{
	bool *vars = new bool[m_n];
	for(unsigned int i(0); i < m_n; ++i) vars[i] = false;

	for(unsigned int i(0); i < m_width*m_size; ++i)
		if(m_trees[i] > 15 && m_trees[i] < m_n+16)
			vars[m_trees[i]-16] = true;

	unsigned int nbvar = 0;
	for(unsigned int i(0); i < m_n; ++i) nbvar += vars[i];
	delete[] vars;

	return nbvar;
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

unsigned int FnArray::randtree() const
{
	return rand()%m_size;
}

void FnArray::changeweight()
{
	unsigned int tree = randtree();

	m_weights[tree] = (float)(rand()%10000)/10000.;
}

void FnArray::swapweights()
{
	if(m_size > 1)
	{
		unsigned int tree1 = randtree();
		unsigned int tree2 = randtree();
		while(tree1 == tree2) tree2 = randtree();


		float tmp = m_weights[tree1];
		m_weights[tree1] = m_weights[tree2];
		m_weights[tree2] = tmp;
	}
}

void FnArray::changesign()
{
	unsigned int tree = randtree();

	unsigned int width = 0;
	for(;width < m_width && m_trees[tree*m_width + width] < m_n+16; ++width);
	unsigned int mutate = rand()%width;


	m_not[tree*m_width + mutate] = !m_not[tree*m_width + mutate];
}

void FnArray::changeop(const std::vector<unsigned int>& fnset)
{
	unsigned int tree = randtree();

	unsigned int width = 0;
	for(;width < m_width && m_trees[tree*m_width + width] < m_n+16; ++width);

	if(width >= 3)
	{
		unsigned int mutate = rand()%width;
		while(m_trees[tree*m_width + mutate] >= 16) mutate = rand()%width;


		m_trees[tree*m_width + mutate] = fnset[rand()%fnset.size()];
		m_not[tree*m_width + mutate] = rand()%2;
	}
}

void FnArray::changevar()
{
	unsigned int tree = randtree();

	unsigned int width = 0;
	for(;width < m_width && m_trees[tree*m_width + width] < m_n+16; ++width);
	unsigned int mutate = rand()%width;
	while(m_trees[tree*m_width + mutate] < 16) mutate = rand()%width;


	m_trees[tree*m_width + mutate] = rand()%m_n + 16;
	m_not[tree*m_width + mutate] = rand()%2;
	

	for(unsigned int i(0); i < m_n; ++i) m_in[i*m_maxsize + tree] = false;
	for(unsigned int i(0); i < width; ++i) if(m_trees[tree*m_width + i] > 15) m_in[(m_trees[tree*m_width + i]-16)*m_maxsize + tree] = true;
}

void FnArray::swapvar()
{
	unsigned int tree = randtree();

	unsigned int width = 0;
	for(;width < m_width && m_trees[tree*m_width + width] < m_n+16; ++width);

	if(width >= 3)
	{
		unsigned int mutate1 = rand()%width;
		while(m_trees[tree*m_width + mutate1] < 16) mutate1 = rand()%width;

		unsigned int mutate2 = rand()%width;
		while(m_trees[tree*m_width + mutate2] < 16 || mutate2 == mutate1) mutate2 = rand()%width;


		unsigned int tmp = m_trees[tree*m_width + mutate1];
		m_trees[tree*m_width + mutate1] = m_trees[tree*m_width + mutate2];
		m_trees[tree*m_width + mutate2] = tmp;

		bool tmpn = m_not[tree*m_width + mutate1];
		m_not[tree*m_width + mutate1] = m_not[tree*m_width + mutate2];
		m_not[tree*m_width + mutate2] = tmpn;
	}
}

void FnArray::addvar(const std::vector<unsigned int>& fnset)
{
	unsigned int tree = randtree();

	unsigned int width = 0;
	for(;width < m_width && m_trees[tree*m_width + width] < m_n+16; ++width);

	if(width < m_width)
	{
		unsigned int mutate = rand()%width;
		while(m_trees[tree*m_width + mutate] < 16) mutate = rand()%width;


		unsigned int *tmp = new unsigned int[m_width];
		bool *tmpn = new bool[m_width];
		for(unsigned int i(0); i < m_width; ++i) tmp[i] = 0;
		for(unsigned int i(0); i < m_width; ++i) tmpn[i] = 0;

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

		delete[] tmp;
		delete[] tmpn;


		for(unsigned int i(0); i < m_n; ++i) m_in[i*m_maxsize + tree] = false;
		for(unsigned int i(0); i < width; ++i) if(m_trees[tree*m_width + i] > 15) m_in[(m_trees[tree*m_width + i]-16)*m_maxsize + tree] = true;
	}
}

void FnArray::delvar()
{
	unsigned int tree = randtree();

	unsigned int width = 0;
	for(;width < m_width && m_trees[tree*m_width + width] < m_n+16; ++width);

	//if(width >= 3)
	if(width >= 50)
	{
		unsigned int mutate = rand()%width;
		while(m_trees[tree*m_width + mutate] >= 16) mutate = rand()%width;


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

		unsigned int save = mutate-1;
		if(m_trees[tree*m_width + mutate-1] >= 16)
		{
			if(rand()%2)
				save = mutate-2;
		}
		else
		{
			if(rand()%2)
			{
				tmp2 = 2;
				unsigned int start2 = save;
				while(tmp2)
				{
					--start2;
					tmp2 += m_trees[tree*m_width + start2] < 16;
					tmp2 -= m_trees[tree*m_width + start2] >= 16;
				}

				save = start2-1;
			}
		}

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


		bool clean = false;
		for(unsigned int j(0); j < m_width; ++j)
		{
			tmp[j] = (clean) ? 0 : tmp[j];
			clean = (tmp[j] == m_n+16) || clean;
			tmpn[j] = (clean) ? 0 : tmpn[j];
		}


		for(unsigned int j(0); j < m_width; ++j) m_trees[tree*m_width + j] = tmp[j];
		for(unsigned int i(0); i < m_width; ++i) m_not[tree*m_width + i] = tmpn[i];

		delete[] tmp;
		delete[] tmpn;


		for(unsigned int i(0); i < m_n; ++i) m_in[i*m_maxsize + tree] = false;
		for(unsigned int i(0); i < width; ++i) if(m_trees[tree*m_width + i] > 15 && m_trees[tree*m_width + i] != m_n+16) m_in[(m_trees[tree*m_width + i]-16)*m_maxsize + tree] = true;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


HyperHeuritic::HyperHeuritic(unsigned int maxsize, unsigned int width, const std::vector<unsigned int>& fnset) : 	m_fnset(fnset),
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
{/*
	m_pop.push_back(new FnArray(m_maxsize, m_width, m_n));
	for(unsigned int i(0); i < newSize; ++i) m_pop.back()->addRandom(m_fnset);*/
	newSize = newSize;
}

void HyperHeuritic::initall(unsigned int newSize)
{/*
	newSize = newSize;
	m_pop.push_back(new FnArray(m_maxsize, m_width, m_n));
	for(unsigned int i(0); i < m_n; ++i) m_pop.back()->add(i+16,rand()%2);*/
	newSize = newSize;
}

unsigned int HyperHeuritic::mut()
{/*
	m_pop.push_back(new FnArray(*m_pop.back()));
	//for(unsigned int i(0); i < 9; ++i)
	//m_pop.back()->mutate(m_fnset);
	return m_pop.back()->mutate(m_fnset);*/
	return 0;
}
















