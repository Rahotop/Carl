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
	for(unsigned int i(0); i < m_size; ++i)
	{
		for(unsigned int j(0); j < m_width; ++j)
		{
			//
		}
	}
}

