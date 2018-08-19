#ifndef FNARRAY_HPP_INCLUDED
#define FNARRAY_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <stack>

class FnArray
{
	public:

	FnArray(unsigned int size, unsigned int width, unsigned int n, const std::vector<unsigned int>& fnset);
	~FnArray();

	float evaluate(bool *s) const;

	private:

	std::vector<unsigned int> m_fnset;
	unsigned int m_n;

	unsigned int m_maxsize;	//number of sub trees
	unsigned int m_size;
	unsigned int m_width;	//size of sub trees
	unsigned int *m_trees;
	bool *m_not;
	float *m_weights;
};

#endif