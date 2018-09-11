#ifndef ALGOGEN_HPP_INCLUDED
#define ALGOGEN_HPP_INCLUDED

#include <iomanip>
#include <sstream>
#include <utility>
#include <vector>
#include <iterator>
#include <algorithm>
#include <fstream>
#include "localsearch.hpp"
#include "maxsat.hpp"
#include "fnarray.hpp"
#include "msmemory.hpp"

class AlgoGen
{
	public:

	AlgoGen(unsigned int popsize, unsigned int maxsize, unsigned int width);
	~AlgoGen();

	void run(MaxSat& ms, unsigned int newSize, unsigned int nbIt);

	private:
	void setParams(const MaxSat& ms);
	void initPop(unsigned int newSize);
	void newPop();
	unsigned int evalPop(MaxSat& ms, unsigned int prec);
	void correlation(MaxSat& ms, std::ostream& data, bool* bestils);
	void freq(MaxSat& ms, std::ostream& res, bool* bestils);
	
	void crossover(unsigned int p1, unsigned int p2, unsigned int next);

	void addRandom(unsigned int index);
	void addRandom(unsigned int index, unsigned int next);
	void add(unsigned int index, unsigned int tree, unsigned int next);

	void deleteRandom(unsigned int index, unsigned int next);
	void deleteTree(unsigned int index, unsigned int next, unsigned int tree);

	void copy(unsigned int index, unsigned int next);
	void copy(unsigned int index, unsigned int next, unsigned int tree);

	float evaluate(unsigned int index, bool *s);
	float evaluateinc(unsigned int index, bool *s, unsigned int changed);
	void acceptCurr(unsigned int ind);
	bool* localsearchind(unsigned int index, unsigned int *nbEval = nullptr);
	
	bool* getActivation(unsigned int index) const;

	void show(unsigned int index, std::ostream& out);

	// PARAM
	std::vector<unsigned int> m_fnset;
	unsigned int m_n;

	// SIZE
	unsigned int m_popsize; // number of ind
	unsigned int m_maxsize; // number max of subtrees
	unsigned int m_width; // size of subtrees

	// POPULATION
	unsigned int *m_sort;
	unsigned int *m_size; // m_size[i] is the number of subtrees in the i-th ind 
	unsigned int *m_trees; // m_trees[i*maxsize*width+j*width+k] is the k-th element of the j-th subtree of i-th ind
	bool *m_not; // m_not[i*maxsize*width+j*width+k] is the sign of the k-th element of the j-th subtree of i-th ind
	bool *m_prec;
	bool *m_curr;
	bool *m_in;
	float *m_weights; // m_weights[i*maxsize+j] is the weight of the j-th subtree of the i-th ind
	float *m_fitness; // m_fitness[i] is the fitness of the i-th ind

	// NEXT GENERATION
	unsigned int *m_Gsize;
	unsigned int *m_Gtrees;
	bool *m_Gnot;
	bool *m_Gin;
	float *m_Gweights;
};

unsigned int distance(bool *s1, bool *s2, unsigned int n);

#endif