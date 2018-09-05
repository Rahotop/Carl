#ifndef MSALGOGEN_HPP_INCLUDED
#define MSALGOGEN_HPP_INCLUDED

#include <utility>
#include <vector>
#include <iterator>
#include <algorithm>
#include <fstream>
#include "localsearch.hpp"
#include "maxsat.hpp"
#include "fnarray.hpp"
#include "msmemory.hpp"

FnArray msalgogen(MaxSat& ms, unsigned int indSize, unsigned int indWidth, unsigned int newSize, unsigned int nbIt, unsigned int popSize);
FnArrayInc msalgogeninc(MaxSat& ms, unsigned int indSize, unsigned int indWidth, unsigned int newSize, unsigned int nbIt, unsigned int popSize);



class MSalgogenincpar
{
	public:

	MSalgogenincpar(unsigned int popsize, unsigned int maxsize, unsigned int width);
	~MSalgogenincpar();

	void run(const MaxSat& ms, unsigned int newSize, unsigned int nbIt);

	private:
	
	void crossover(unsigned int p1, unsigned int p2, unsigned int next);

	void addRandom(unsigned int index);
	void addRandom(unsigned int index, unsigned int next);

	void deleteRandom(unsigned int index, unsigned int next);
	void deleteTree(unsigned int index, unsigned int next, unsigned int tree);

	void copy(unsigned int index, unsigned int next);
	void copy(unsigned int index, unsigned int next, unsigned int tree);

	float evaluate(unsigned int index, bool *s);
	float evaluateinc(unsigned int index, bool *s, unsigned int changed);
	void acceptCurr(unsigned int ind);
	bool* localsearch(unsigned int index);

	void show(unsigned int index);

	// PARAM
	std::vector<unsigned int> m_fnset;
	unsigned int m_n;

	// SIZE
	unsigned int m_popsize; // number of ind
	unsigned int m_maxsize; // number max of subtrees
	unsigned int m_width; // size of subtrees

	// POPULATION
	unsigned int *m_size; // m_size[i] is the number of subtrees in the i-th ind 
	unsigned int *m_trees; // m_trees[i*maxsize*width+j*width+k] is the k-th element of the j-th subtree of i-th ind
	bool *m_not; // m_not[i*maxsize*width+j*width+k] is the sign of the k-th element of the j-th subtree of i-th ind
	bool *m_prec;
	bool *m_curr;
	float *m_weights; // m_weights[i*maxsize+j] is the weight of the j-th subtree of the i-th ind
	float *m_fitness; // m_fitness[i] is the fitness of the i-th ind

	// NEXT GENERATION
	unsigned int *m_Gsize;
	unsigned int *m_Gtrees;
	bool *m_Gnot;
	float *m_Gweights;
};

#endif