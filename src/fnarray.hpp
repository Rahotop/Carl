#ifndef FNARRAY_HPP_INCLUDED
#define FNARRAY_HPP_INCLUDED

#include <iomanip>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <stack>

class FnArray
{
	public:

	FnArray(unsigned int size, unsigned int width, unsigned int n, const std::vector<unsigned int>& fnset);
	~FnArray();

	float evaluate(bool *s) const;

	void addRandom();

	friend std::ostream& operator<<(std::ostream& o, const FnArray& fn);

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

std::ostream& operator<<(std::ostream& os, const FnArray& fn);

#endif