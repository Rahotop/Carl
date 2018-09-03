#include "fnarray.hpp"

FnArray::FnArray() : m_fnset(), m_n(0), m_maxsize(0), m_size(0), m_width(0), m_trees(nullptr), m_not(nullptr), m_weights(nullptr), m_score(0.)
{

}

FnArray::FnArray(unsigned int size, unsigned int width, unsigned int n, const std::vector<unsigned int>& fnset) : m_fnset(fnset), m_n(n), m_maxsize(size), m_size(0), m_width(width), m_trees(nullptr), m_not(nullptr), m_weights(nullptr), m_score(0.)
{
	m_trees = new unsigned int[m_maxsize*m_width];
	m_not = new bool[m_maxsize*m_width];
	m_weights = new float[m_maxsize];
}

FnArray::FnArray(const FnArray& fn) : m_fnset(fn.m_fnset), m_n(fn.m_n), m_maxsize(fn.m_maxsize), m_size(fn.m_size), m_width(fn.m_width), m_trees(nullptr), m_not(nullptr), m_weights(nullptr), m_score(fn.m_score)
{
	m_trees = new unsigned int[fn.m_maxsize*fn.m_width];
	m_not = new bool[fn.m_maxsize*fn.m_width];
	m_weights = new float[fn.m_maxsize];

	for(unsigned int i(0); i < fn.m_size*fn.m_width; ++i) m_trees[i] = fn.m_trees[i];
	for(unsigned int i(0); i < fn.m_size*fn.m_width; ++i) m_not[i] = fn.m_not[i];
	for(unsigned int i(0); i < fn.m_size; ++i) m_weights[i] = fn.m_weights[i];
}

FnArray::~FnArray()
{
	if(m_trees)
		delete[] m_trees;
	if(m_not)
		delete[] m_not;
	if(m_weights)
		delete[] m_weights;
}
/*
float FnArray::evaluate(bool *s) const
{
	float sum = 0;
	unsigned int end = m_n+16;
	for(unsigned int i(0); i < m_size; ++i)
	{
		std::stack<bool> tmp;
		unsigned int index = i*m_width;
		for(unsigned int j(0); j < m_width && m_trees[index+j] < end; ++j)
		{
			unsigned int op = m_trees[index+j];
			if(op >= 16)
			{
				tmp.push(s[op-16] == m_not[index+j]);
			}
			else
			{
				unsigned int offset = tmp.top();
				tmp.pop();
				offset += 2*tmp.top();
				tmp.pop();

				op <<= offset;
				op >>= 3;

				tmp.push(op%2 == m_not[index+j]);
			}
		}
		sum += tmp.top() * m_weights[i];
	}
	return sum;
}*/

float FnArray::evaluate(bool *s) const
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
				unsigned int offset = tmp[--stack] + 2*tmp[--stack];

				op <<= offset;
				op >>= 3;

				tmp[stack++] = (op%2 == notop);
			}
		}
		sum += tmp[0] * m_weights[i];
	}

	delete[] tmp;
	return sum;
}

void FnArray::crossover(const FnArray& f1, const FnArray& f2)
{
	if(m_maxsize != f1.m_maxsize || m_width != f1.m_width)
	{
		m_maxsize = f1.m_maxsize;
		m_width = f1.m_width;

		if(m_trees)
			delete[] m_trees;
		if(m_not)
			delete[] m_not;
		if(m_weights)
			delete[] m_weights;

		m_trees = new unsigned int[m_size*m_width];
		m_not = new bool[m_size*m_width];
		m_weights = new float[m_size];
	}

	m_fnset = f1.m_fnset;
	m_n = f1.m_n;
	m_score = 0.;

	if(rand()%2)
	{
		m_size = f1.m_size;
		for(unsigned int i(0); i < m_size && i < f2.m_size; ++i)
		{
			if(rand()%2)
			{
				for(unsigned int j(0); j < m_width; ++j)
				{
					m_trees[i*m_width+j] = f1.m_trees[i*m_width+j];
					m_not[i*m_width+j] = f1.m_not[i*m_width+j];
				}
				m_weights[i] = f1.m_weights[i];
			}
			else
			{
				for(unsigned int j(0); j < m_width; ++j)
				{
					m_trees[i*m_width+j] = f2.m_trees[i*m_width+j];
					m_not[i*m_width+j] = f2.m_not[i*m_width+j];
				}
				m_weights[i] = f2.m_weights[i];
			}
		}

		for(unsigned int i(f2.m_size*m_width); i < m_size*m_width; ++i) m_trees[i] = f1.m_trees[i];
		for(unsigned int i(f2.m_size*m_width); i < m_size*m_width; ++i) m_not[i] = f1.m_not[i];
		for(unsigned int i(f2.m_size); i < m_size; ++i) m_weights[i] = f1.m_weights[i];
	}
	else
	{
		m_size = f2.m_size;
		for(unsigned int i(0); i < m_size && i < f1.m_size; ++i)
		{
			if(rand()%2)
			{
				for(unsigned int j(0); j < m_width; ++j)
				{
					m_trees[i*m_width+j] = f1.m_trees[i*m_width+j];
					m_not[i*m_width+j] = f1.m_not[i*m_width+j];
				}
				m_weights[i] = f1.m_weights[i];
			}
			else
			{
				for(unsigned int j(0); j < m_width; ++j)
				{
					m_trees[i*m_width+j] = f2.m_trees[i*m_width+j];
					m_not[i*m_width+j] = f2.m_not[i*m_width+j];
				}
				m_weights[i] = f2.m_weights[i];
			}
		}

		for(unsigned int i(f1.m_size*m_width); i < m_size*m_width; ++i) m_trees[i] = f2.m_trees[i];
		for(unsigned int i(f1.m_size*m_width); i < m_size*m_width; ++i) m_not[i] = f2.m_not[i];
		for(unsigned int i(f1.m_size); i < m_size; ++i) m_weights[i] = f2.m_weights[i];
	}
}

void FnArray::addRandom()
{
	if(m_size < m_maxsize)
	{
		m_score = 0.;
		unsigned int width = (rand()%((m_width-1)/2)) * 2 + 1;
		unsigned int stack = 0;

		for(unsigned int i(0); i < width; ++i)
		{
			if((stack-1) == (width-i))
			{
				m_trees[m_size*m_width+i] = m_fnset[rand()%m_fnset.size()];
				--stack;
			}
			else if((stack < 2) || (rand()%2))
			{
				m_trees[m_size*m_width+i] = (rand()%m_n) + 16;
				++stack;
			}
			else
			{
				m_trees[m_size*m_width+i] = m_fnset[rand()%m_fnset.size()];
				--stack;
			}
		}
		if(width < m_width)
		{
			m_trees[m_size*m_width+width] = m_n+16;
		}


		for(unsigned int i(0); i < width; ++i)
		{
			m_not[m_size*m_width+i] = rand()%2;
		}


		m_weights[m_size] = ((float)(rand()%10000))/10000.;


		++m_size;
	}
}

void FnArray::deleteRandom()
{
	if(m_size)
	{
		deleteTree(rand()%m_size);
	}
}

void FnArray::deleteTree(unsigned int index)
{
	m_score = 0.;
	--m_size;

	for(unsigned int i(0); i < m_width; ++i) m_trees[index*m_width+i] = m_trees[m_size*m_width+i];
	for(unsigned int i(0); i < m_width; ++i) m_not[index*m_width+i] = m_not[m_size*m_width+i];
	m_weights[index] = m_weights[m_size];
}

void FnArray::deleteAll()
{
	m_score = 0.;
	m_size = 0;
}

FnArray& FnArray::operator=(const FnArray& fn)
{
	if(m_maxsize != fn.m_maxsize || m_width != fn.m_width)
	{
		m_maxsize = fn.m_maxsize;
		m_width = fn.m_width;

		if(m_trees)
			delete[] m_trees;
		if(m_not)
			delete[] m_not;
		if(m_weights)
			delete[] m_weights;

		m_trees = new unsigned int[m_maxsize*m_width];
		m_not = new bool[m_maxsize*m_width];
		m_weights = new float[m_maxsize];
	}

	m_fnset = fn.m_fnset;
	m_n = fn.m_n;
	m_size = fn.m_size;
	m_score = fn.m_score;

	for(unsigned int i(0); i < m_size*m_width; ++i) m_trees[i] = fn.m_trees[i];
	for(unsigned int i(0); i < m_size*m_width; ++i) m_not[i] = fn.m_not[i];
	for(unsigned int i(0); i < m_size; ++i) m_weights[i] = fn.m_weights[i];

	return *this;
}

std::ostream& operator<<(std::ostream& o, const FnArray& fn)
{
	o << fn.m_maxsize << " " << fn.m_size << " " << fn.m_width << std::endl;

	for(unsigned int i(0); i < fn.m_size; ++i)
	{
		o << fn.m_weights[i] << std::endl;
		for(unsigned int j(0); j < fn.m_width; ++j)
		{
			o << std::setw(3) << fn.m_trees[i*fn.m_width+j] << " ";
		}
		o << std::endl;
		for(unsigned int j(0); j < fn.m_width; ++j)
		{
			o << std::setw(3) << fn.m_not[i*fn.m_width+j] << " ";
		}
		o << std::endl;
	}

	return o;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////FNARRAYINC/////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////




FnArrayInc::FnArrayInc() : m_fnset(), m_n(0), m_maxsize(0), m_size(0), m_width(0), m_trees(nullptr), m_not(nullptr), m_prec(nullptr), m_weights(nullptr), m_score(0.)
{

}

FnArrayInc::FnArrayInc(unsigned int size, unsigned int width, unsigned int n, const std::vector<unsigned int>& fnset) : m_fnset(fnset), m_n(n), m_maxsize(size), m_size(0), m_width(width), m_trees(nullptr), m_not(nullptr), m_prec(nullptr), m_weights(nullptr), m_score(0.)
{
	m_trees = new unsigned int[m_maxsize*m_width];
	m_not = new bool[m_maxsize*m_width];
	m_prec = new bool[m_maxsize];
	m_weights = new float[m_maxsize];
}

FnArrayInc::FnArrayInc(const FnArrayInc& fn) : m_fnset(fn.m_fnset), m_n(fn.m_n), m_maxsize(fn.m_maxsize), m_size(fn.m_size), m_width(fn.m_width), m_trees(nullptr), m_not(nullptr), m_prec(nullptr), m_weights(nullptr), m_score(fn.m_score)
{
	m_trees = new unsigned int[fn.m_maxsize*fn.m_width];
	m_not = new bool[fn.m_maxsize*fn.m_width];
	m_prec = new bool[fn.m_maxsize];
	m_weights = new float[fn.m_maxsize];

	for(unsigned int i(0); i < fn.m_size*fn.m_width; ++i) m_trees[i] = fn.m_trees[i];
	for(unsigned int i(0); i < fn.m_size*fn.m_width; ++i) m_not[i] = fn.m_not[i];
	for(unsigned int i(0); i < fn.m_size; ++i) m_weights[i] = fn.m_weights[i];
}

FnArrayInc::~FnArrayInc()
{
	if(m_trees)
		delete[] m_trees;
	if(m_not)
		delete[] m_not;
	if(m_prec)
		delete[] m_prec;
	if(m_weights)
		delete[] m_weights;
}

float FnArrayInc::evaluate(bool *s) const
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
				unsigned int offset = tmp[--stack] + 2*tmp[--stack];

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

float FnArrayInc::evaluateInc(bool *s, unsigned int changed) const
{
	s[changed] = ! s[changed];
	float sum = 0;
	unsigned int end = m_n+16;

	bool *tmp = new bool[m_width];
	
	for(unsigned int i(0); i < m_size; ++i)
	{
		unsigned int index = i*m_width;
		bool update = false;
		for(unsigned int j(0); j < m_width && m_trees[index+j] < end; ++j)
		{
			if((m_trees[index+j] - 16) == changed)
			{
				update = true;
				break;
			}
		}
		tmp[0] = m_prec[i];

		if(update)
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
					unsigned int offset = tmp[--stack] + 2*tmp[--stack];

					op <<= offset;
					op >>= 3;

					tmp[stack++] = (op%2 == notop);
				}
			}
		}
		sum += tmp[0] * m_weights[i];
	}

	s[changed] = ! s[changed];
	delete[] tmp;
	return sum;
}

void FnArrayInc::crossover(const FnArrayInc& f1, const FnArrayInc& f2)
{
	if(m_maxsize != f1.m_maxsize || m_width != f1.m_width)
	{
		m_maxsize = f1.m_maxsize;
		m_width = f1.m_width;

		if(m_trees)
			delete[] m_trees;
		if(m_not)
			delete[] m_not;
		if(m_prec)
			delete[] m_prec;
		if(m_weights)
			delete[] m_weights;

		m_trees = new unsigned int[m_size*m_width];
		m_not = new bool[m_size*m_width];
		m_prec = new bool[m_size];
		m_weights = new float[m_size];
	}

	m_fnset = f1.m_fnset;
	m_n = f1.m_n;
	m_score = 0.;

	if(rand()%2)
	{
		m_size = f1.m_size;
		for(unsigned int i(0); i < m_size && i < f2.m_size; ++i)
		{
			if(rand()%2)
			{
				for(unsigned int j(0); j < m_width; ++j)
				{
					m_trees[i*m_width+j] = f1.m_trees[i*m_width+j];
					m_not[i*m_width+j] = f1.m_not[i*m_width+j];
				}
				m_weights[i] = f1.m_weights[i];
			}
			else
			{
				for(unsigned int j(0); j < m_width; ++j)
				{
					m_trees[i*m_width+j] = f2.m_trees[i*m_width+j];
					m_not[i*m_width+j] = f2.m_not[i*m_width+j];
				}
				m_weights[i] = f2.m_weights[i];
			}
		}

		for(unsigned int i(f2.m_size*m_width); i < m_size*m_width; ++i) m_trees[i] = f1.m_trees[i];
		for(unsigned int i(f2.m_size*m_width); i < m_size*m_width; ++i) m_not[i] = f1.m_not[i];
		for(unsigned int i(f2.m_size); i < m_size; ++i) m_weights[i] = f1.m_weights[i];
	}
	else
	{
		m_size = f2.m_size;
		for(unsigned int i(0); i < m_size && i < f1.m_size; ++i)
		{
			if(rand()%2)
			{
				for(unsigned int j(0); j < m_width; ++j)
				{
					m_trees[i*m_width+j] = f1.m_trees[i*m_width+j];
					m_not[i*m_width+j] = f1.m_not[i*m_width+j];
				}
				m_weights[i] = f1.m_weights[i];
			}
			else
			{
				for(unsigned int j(0); j < m_width; ++j)
				{
					m_trees[i*m_width+j] = f2.m_trees[i*m_width+j];
					m_not[i*m_width+j] = f2.m_not[i*m_width+j];
				}
				m_weights[i] = f2.m_weights[i];
			}
		}

		for(unsigned int i(f1.m_size*m_width); i < m_size*m_width; ++i) m_trees[i] = f2.m_trees[i];
		for(unsigned int i(f1.m_size*m_width); i < m_size*m_width; ++i) m_not[i] = f2.m_not[i];
		for(unsigned int i(f1.m_size); i < m_size; ++i) m_weights[i] = f2.m_weights[i];
	}
}

void FnArrayInc::addRandom()
{
	if(m_size < m_maxsize)
	{
		m_score = 0.;
		unsigned int width = (rand()%((m_width-1)/2)) * 2 + 1;
		unsigned int stack = 0;

		for(unsigned int i(0); i < width; ++i)
		{
			if((stack-1) == (width-i))
			{
				m_trees[m_size*m_width+i] = m_fnset[rand()%m_fnset.size()];
				--stack;
			}
			else if((stack < 2) || (rand()%2))
			{
				m_trees[m_size*m_width+i] = (rand()%m_n) + 16;
				++stack;
			}
			else
			{
				m_trees[m_size*m_width+i] = m_fnset[rand()%m_fnset.size()];
				--stack;
			}
		}
		if(width < m_width)
		{
			m_trees[m_size*m_width+width] = m_n+16;
		}


		for(unsigned int i(0); i < width; ++i)
		{
			m_not[m_size*m_width+i] = rand()%2;
		}


		m_weights[m_size] = ((float)(rand()%10000))/10000.;


		++m_size;
	}
}

void FnArrayInc::deleteRandom()
{
	if(m_size)
	{
		deleteTree(rand()%m_size);
	}
}

void FnArrayInc::deleteTree(unsigned int index)
{
	m_score = 0.;
	--m_size;

	for(unsigned int i(0); i < m_width; ++i) m_trees[index*m_width+i] = m_trees[m_size*m_width+i];
	for(unsigned int i(0); i < m_width; ++i) m_not[index*m_width+i] = m_not[m_size*m_width+i];
	m_weights[index] = m_weights[m_size];
}

void FnArrayInc::deleteAll()
{
	m_score = 0.;
	m_size = 0;
}

FnArrayInc& FnArrayInc::operator=(const FnArrayInc& fn)
{
	if(m_maxsize != fn.m_maxsize || m_width != fn.m_width)
	{
		m_maxsize = fn.m_maxsize;
		m_width = fn.m_width;

		if(m_trees)
			delete[] m_trees;
		if(m_not)
			delete[] m_not;
		if(m_prec)
			delete[] m_prec;
		if(m_weights)
			delete[] m_weights;

		m_trees = new unsigned int[m_maxsize*m_width];
		m_not = new bool[m_maxsize*m_width];
		m_prec = new bool[m_maxsize];
		m_weights = new float[m_maxsize];
	}

	m_fnset = fn.m_fnset;
	m_n = fn.m_n;
	m_size = fn.m_size;
	m_score = fn.m_score;

	for(unsigned int i(0); i < m_size*m_width; ++i) m_trees[i] = fn.m_trees[i];
	for(unsigned int i(0); i < m_size*m_width; ++i) m_not[i] = fn.m_not[i];
	for(unsigned int i(0); i < m_size; ++i) m_weights[i] = fn.m_weights[i];

	return *this;
}

std::ostream& operator<<(std::ostream& o, const FnArrayInc& fn)
{
	o << fn.m_maxsize << " " << fn.m_size << " " << fn.m_width << std::endl;

	for(unsigned int i(0); i < fn.m_size; ++i)
	{
		o << fn.m_weights[i] << std::endl;
		for(unsigned int j(0); j < fn.m_width; ++j)
		{
			o << std::setw(3) << fn.m_trees[i*fn.m_width+j] << " ";
		}
		o << std::endl;
		for(unsigned int j(0); j < fn.m_width; ++j)
		{
			o << std::setw(3) << fn.m_not[i*fn.m_width+j] << " ";
		}
		o << std::endl;
	}

	return o;
}