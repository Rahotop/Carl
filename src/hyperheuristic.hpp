#ifndef HYPERHEURISTIC_HPP_INCLUDED
#define HYPERHEURISTIC_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include "algogen.hpp"

class FnArray
{
	public:

	FnArray(unsigned int maxsize, unsigned int width, unsigned int n);
	FnArray(const FnArray& fn);
	~FnArray();

	void addRandom(const std::vector<unsigned int>& fnset);
	void deleteRandom();
	void deleteTree(unsigned int tree);
	void mutate(unsigned int tree, const std::vector<unsigned int>& fnset);

	float evaluate(bool *s);
	float evaluateinc(bool *s, unsigned int changed);
	void acceptCurr();
	bool* ls(bool *s, unsigned int *nbEval = nullptr);

	inline void setfitness(float fit) { m_fitness = fit; }
	inline float getfitness() const { return m_fitness; }
	inline bool* getSol() const { return m_s; }
	inline unsigned int size() const { return m_size; }

	void show(std::ostream& out);

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

	inline float finalScore() const { return m_pop.back()->getfitness(); }

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
		unsigned int it(1);
		for(; improved; ++it)
		{
			improved = false;
			unsigned int tmpnbeval = 0;
			float score = m_pop.back()->getfitness();
			bool *s = m_pop.back()->getSol();
			unsigned int same = 0;

			unsigned int i(0);
			for(; i < visit; ++i)
			{/*
				m_pop.push_back(new FnArray(m_maxsize, m_width, m_n));
				for(unsigned int i(0); i < newSize; ++i) m_pop.back()->addRandom(m_fnset);*/

				m_pop.push_back(new FnArray(*m_pop.back()));
				if(m_pop.back()->size())
					m_pop.back()->mutate(rand()%m_pop.back()->size(), m_fnset);
/*
				m_pop.push_back(new FnArray(*m_pop.back()));
				m_pop.back()->addRandom(m_fnset);*/

/*
				if(rand()%3)
				{
					m_pop.back()->addRandom(m_fnset);
				}
				else
				{
					m_pop.back()->deleteRandom();
				}*//*
				unsigned int tmp = rand()%10;
				for(unsigned int i(0); i < tmp; ++i) m_pop.back()->addRandom(m_fnset);*/

				tmpnbeval = evalPop(pb, s);
				nbEval += tmpnbeval;

				if(!distance(s,m_pop.back()->getSol(),m_n))
					++same;

				if(m_pop.back()->getfitness() > score)
				{
					improved = true;
					break;
				}
				else
				{
					delete m_pop.back();
					m_pop.pop_back();
				}
			}
/*
			if(!improved)
			{
				for(unsigned int i(0); i < visit; ++i)
				{
					m_pop.push_back(new FnArray(*m_pop.back()));
					m_pop.back()->addRandom(m_fnset);

					tmpnbeval = evalPop(pb, s);
					nbEval += tmpnbeval;
					
					if(distance(s,m_pop.back()->getSol(),m_n))
						++same;

					if(m_pop.back()->getfitness() > score)
					{
						improved = true;
						break;
					}
					else
					{
						delete m_pop.back();
						m_pop.pop_back();
					}
				}
			}*/


			// SAVE
			data << it << " " << m_pop.back()->getfitness() << " " << m_pop.back()->size() << " " << tmpnbeval << " " << same << " " << i << std::endl;
		}
		data << std::endl;

		res << "nb eval (solutions) : " << nbEval << std::endl;
		res << "nb eval (f obj) : " << pb.getnbeval() << std::endl;
		res << "\n#ILS :\n";

		unsigned int ilsnbeval = 0;
		bool *bestils = ils(data, pb, 10000, &ilsnbeval);
		data << "\n0 " << pb.evaluate(bestils) << "\n" << it << " " << pb.evaluate(bestils) << std::endl;
		res << "#fitness max : " << pb.evaluate(bestils) << std::endl;
		res << "#nb eval : " << ilsnbeval << std::endl;
		delete[] bestils;

		res << std::endl << "x : " << pb.evaluate(m_s) << " (" << pb.islocopt(m_s) << ")\n\n";
		for(unsigned int i(0); i < m_pop.size(); ++i)
		{
			res << "x" << i << " : " << pb.evaluate(m_pop[i]->getSol()) << " (" << pb.islocopt(m_pop[i]->getSol()) << ")\n\t" << distance(m_pop[i]->getSol(),m_s,m_n);
			for(unsigned int j(0); j < i; ++j)
				res << ", " << distance(m_pop[i]->getSol(),m_pop[j]->getSol(),m_n);
			res << std::endl;
		}
		res << std::endl;

		for(unsigned int i(0); i < m_pop.size(); ++i)
		{
			res << "========================================" << std::endl;
			m_pop[i]->show(res);
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
















