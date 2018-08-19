#include "fnarray.hpp"

FnArray::FnArray(unsigned int size, unsigned int width, unsigned int n, const std::vector<unsigned int>& fnset) : m_fnset(fnset), m_n(n), m_maxsize(size), m_size(0), m_width(width), m_trees(nullptr), m_not(nullptr), m_weights(nullptr)
{
	m_trees = new unsigned int[size*width];
	m_not = new bool[size*width];
	m_weights = new float[size];

	m_trees[0] = 16;
	m_trees[1] = 17;
	m_trees[2] = 9;
	m_trees[3] = m_n+16;

	m_not[0] = 1;
	m_not[1] = 1;
	m_not[2] = 1;
	m_not[3] = 1;

	m_weights[0] = 0.1;

	m_trees[m_width+0] = 16;
	m_trees[m_width+1] = m_n+16;

	m_not[m_width+0] = 0;
	m_not[m_width+1] = 1;

	m_weights[1] = 0.2;

	m_size = 2;
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
			std::cout << op;
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
			std::cout << " : " << tmp.top() << std::endl;
		}
		sum += tmp.top() * m_weights[i];
	}
	return sum;
}

