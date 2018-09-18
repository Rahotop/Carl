#ifndef HYPERHEURISTIC_HPP_INCLUDED
#define HYPERHEURISTIC_HPP_INCLUDED

#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

class FnArray
{
	public:

	FnArray(unsigned int maxsize, unsigned int width, unsigned int n);
	FnArray(const FnArray& fn);
	~FnArray();

	void addRandom(const std::vector<unsigned int>& fnset);
	void deleteRandom();
	void deleteTree(unsigned int tree);

	float evaluate(bool *s);
	float evaluateinc(bool *s, unsigned int changed);
	void acceptCurr();
	bool* ls(bool *s, unsigned int *nbEval = nullptr);

	inline void setfitness(float fit) { m_fitness = fit; }
	inline float getfitness() const { return m_fitness; }
	inline bool* getSol() const { return m_s; }
	inline unsigned int size() const { return m_size; }

	private:

	// PARAM
	unsigned int m_n;

	// SIZE
	unsigned int m_maxsize; // number max of subtrees
	unsigned int m_width; // size of subtrees
	unsigned int m_size; // m_size is the number of subtrees

	// SUBTREES
	unsigned int *m_trees; // m_trees[i*width+j] is the j-th element of the i-th subtree
	bool *m_not; // m_not[i*width+j] is the sign of the j-th element of the i-th subtree
	bool *m_prec;
	bool *m_curr;
	bool *m_in;
	bool *m_s;
	float *m_weights; // m_weights[i] is the weight of the i-th subtree
	float m_fitness; // m_fitness is the fitness
};

class HyperHeuritic
{
	public:

	HyperHeuritic(unsigned int maxsize, unsigned int width);
	~HyperHeuritic();

	private:

	// PARAMS
	std::vector<unsigned int> m_fnset;
	unsigned int m_n;
	bool *m_s;

	// SIZE
	unsigned int m_maxsize; // number max of subtrees
	unsigned int m_width; // size of subtrees

	// POPULATION
	std::vector<FnArray*> m_pop;

	public:

	template<class PB>
	void run(PB& pb, unsigned int newSize, unsigned int visit, const std::string& file)
	{
		// PARAM
		m_n = pb.getN();
		m_s = new bool[m_n];
		for(unsigned int i(0); i < m_n; ++i) m_s[i] = rand()%2;


		// OUTPUT
		std::ofstream data(file);
		std::ofstream res(file+".txt");
		unsigned int nbEval = 0;


		// INIT POP
		m_pop.push_back(new FnArray(m_maxsize, m_width, m_n));
		for(unsigned int i(0); i < newSize; ++i) m_pop.back()->addRandom(m_fnset);
		nbEval = evalPop(pb, m_s);
		data << "0 " << m_pop.back()->getfitness() << " " << m_pop.back()->size() << " " << nbEval << "\n";


		// LS
		bool improved = true;
		for(unsigned int it(1); improved; ++it)
		{
			improved = false;
			unsigned int tmpnbeval = 0;
			float score = m_pop.back()->getfitness();
			bool *s = m_pop.back()->getSol();

			for(unsigned int i(0); i < visit; ++i)
			{/*
				m_pop.push_back(new FnArray(m_maxsize, m_width, m_n));
				for(unsigned int i(0); i < newSize; ++i) m_pop.back()->addRandom(m_fnset);*/

				m_pop.push_back(new FnArray(*m_pop.back()));

				if(rand()%3)
				{
					m_pop.back()->addRandom(m_fnset);
				}
				else
				{
					m_pop.back()->deleteRandom();
				}/*
				unsigned int tmp = rand()%10;
				for(unsigned int i(0); i < tmp; ++i) m_pop.back()->addRandom(m_fnset);*/

				tmpnbeval = evalPop(pb, s);
				nbEval += tmpnbeval;

				if(m_pop.back()->getfitness() > score)
				{
					improved = true;
					break;
				}
				else
				{
					m_pop.pop_back();
				}
			}


			// SAVE
			data << it << " " << m_pop.back()->getfitness() << " " << m_pop.back()->size() << " " << tmpnbeval << std::endl;
		}
	}

	template<class PB>
	unsigned int evalPop(PB& pb, bool *s)
	{
		unsigned int tmp = 0;
		bool *ns = m_pop.back()->ls(s, &tmp);
		m_pop.back()->setfitness(pb.evaluate(ns));
		return tmp;
	}
};

#endif
















