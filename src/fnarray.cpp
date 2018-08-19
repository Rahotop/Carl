#include "fnarray.hpp"

FnArray::FnArray(unsigned int size, unsigned int width, unsigned int n, const std::vector<unsigned int>& fnset) : m_fnset(fnset), m_n(n), m_maxsize(size), m_size(0), m_width(width), m_trees(nullptr), m_not(nullptr), m_weights(nullptr)
{
	m_trees = new unsigned int[size*width];
	m_not = new bool[size*width];
	m_weights = new float[size];
}

FnArray::~FnArray()
{
	delete[] m_trees;
	delete[] m_not;
	delete[] m_weights;
}

float FnArray::evaluate(bool *s) const
{
	float sum = 0;
	unsigned int end = m_n+16;
	for(unsigned int i(0); i < m_size; ++i)
	{
		std::stack<bool> tmp;
		for(unsigned int j(0); j < m_width && m_trees[i*m_width+j] < end; ++j)
		{
			unsigned int op = m_trees[i*m_width+j];
			if(op >= 16)
			{
				tmp.push(s[op-16] == m_not[i*m_width+j]);
			}
			else
			{
				unsigned int offset = tmp.top();
				tmp.pop();
				offset += 2*tmp.top();
				tmp.pop();

				op <<= offset;
				op >>= 3;

				tmp.push(op%2 == m_not[i*m_width+j]);
			}
		}
		sum += tmp.top() * m_weights[i];
	}
	return sum;
}

void FnArray::addRandom()
{
	if(m_size < m_maxsize)
	{
		unsigned int width = (rand()%((m_width-1)/2)) * 2 + 1;
		unsigned int stack = 0;

		for(unsigned int i(0); i < width; ++i)
		{
			if(stack-1 == width-i)
			{
				m_trees[m_size*m_width+i] = m_fnset[rand()%m_fnset.size()];
				--stack;
			}
			else if(stack < 2 || rand()%2)
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

