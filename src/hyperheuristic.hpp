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

	void add(unsigned int var, bool n);
	void addRandom(const std::vector<unsigned int>& fnset);
	void deleteRandom();
	void deleteTree(unsigned int tree);
	void mutate(unsigned int tree, const std::vector<unsigned int>& fnset);

	float propSat(bool *s) const;
	float evaluate(bool *s);
	float evaluateinc(bool *s, unsigned int changed);
	void acceptCurr();
	bool* ls(bool *s, unsigned int *nbEval = nullptr);

	inline void setfitness(float fit) { m_fitness = fit; }
	inline float getfitness() const { return m_fitness; }
	inline bool* getSol() const { return m_s; }
	inline unsigned int size() const { return m_size; }
	inline unsigned int getsteps() const { return m_steps; }

	void show(std::ostream& out);

	private:

	unsigned int randtree() const;
	void changeweight();
	void swapweights();
	void changesign();
	void changeop(const std::vector<unsigned int>& fnset);
	void changevar();
	void swapvar();
	void addvar(const std::vector<unsigned int>& fnset);
	void delvar();

	// PARAM
	unsigned int m_n;

	// SIZE
	unsigned int m_maxsize; // number max of subtrees
	unsigned int m_width; // size of subtrees
	unsigned int m_size; // m_size is the number of subtrees

	unsigned int m_steps;

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

class HyperHeuritic;

typedef void (HyperHeuritic::*INIT)(unsigned int);
typedef bool (HyperHeuritic::*COND)();
typedef void (HyperHeuritic::*NEXT)(unsigned int);

class HyperHeuritic
{
	public:

	HyperHeuritic(unsigned int maxsize, unsigned int width, const std::vector<unsigned int>& fnset);
	~HyperHeuritic();

	inline float finalScore() const { return m_pop.back()->getfitness(); }
	void initrand(unsigned int newSize);
	void initall(unsigned int newSize);
	void mut1(unsigned int newSize);
	void mut2(unsigned int newSize);
	void mut3(unsigned int newSize);
	void mut1add(unsigned int newSize);
	void mutadd(unsigned int newSize);
	void mutnew(unsigned int newSize);
	bool sup();
	bool supeq();

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
	void run(PB& pb, INIT in, COND co, NEXT ne, unsigned int newSize, unsigned int visit, unsigned int nbit, const std::string& file, bool everyoutputs)
	{
		// PARAM
		m_n = pb.getN();
		m_s = new bool[m_n];
		for(unsigned int i(0); i < m_n; ++i) m_s[i] = rand()%2;
		bool *bestSol = new bool[m_n];
		for(unsigned int i(0); i < m_n; ++i) bestSol[i] = m_s[i];


		// OUTPUT
		std::ofstream data(file);
		std::ofstream res(file+".txt");
		unsigned int nbEval = 0;
		float best = 0.;
		FnArray *bestInd = nullptr;


		// INIT POP
		((*this).*in)(newSize);
		nbEval = evalPop(pb, m_s);
		data << "0 " << m_pop.back()->getfitness() << " " << m_pop.back()->size() << " " << nbEval << "\n";
		bestInd = new FnArray(*m_pop.back());


		// LS
		bool improved = true;
		unsigned int it(1);
		for(; improved && it <= nbit; ++it)
		{
			unsigned int tmpnbeval = 0;
			float same = 0.;

			improved = next(pb, visit, newSize, tmpnbeval, nbEval, same, co, ne);

			if(improved && !everyoutputs)
			{
				delete m_pop[0];
				m_pop[0] = m_pop[1];
				m_pop.pop_back();
			}
			if(best <= m_pop.back()->getfitness())
			{
				best = m_pop.back()->getfitness();
				delete bestInd;
				bestInd = new FnArray(*m_pop.back());
				for(unsigned int i(0); i < m_n; ++i) bestSol[i] = m_pop.back()->getSol()[i];
			}
			
			// SAVE
			data << it << " " << m_pop.back()->getfitness() << " " << m_pop.back()->size() << " " << tmpnbeval << " " << same << " ";
			data << m_pop.back()->propSat(m_pop.back()->getSol()) << " " << best << std::endl;
		}
		data << std::endl;

		res << "nb eval (solutions) : " << nbEval << std::endl;
		res << "nb eval (f obj) : " << pb.getnbeval() << std::endl;
		res << "fitness max : " << best << std::endl;

		res << std::endl << "Sol : ";
		for(unsigned int i(0); i < m_n; ++i) res << bestSol[i];
		res << std::endl;
		delete[] bestSol;


		res << std::endl << "x : " << pb.evaluate(m_s) << " (" << pb.islocopt(m_s) << ")\n\n";
		for(unsigned int i(0); i < m_pop.size(); ++i)
		{
			res << "x" << i << " : " << pb.evaluate(m_pop[i]->getSol()) << " (" << pb.islocopt(m_pop[i]->getSol()) << ")\n\t" << distance(m_pop[i]->getSol(),m_s,m_n);
			for(unsigned int j(0); j < i; ++j)
				res << ", " << distance(m_pop[i]->getSol(),m_pop[j]->getSol(),m_n);
			res << std::endl;
		}
		res << std::endl;

		bestInd->show(res);
		delete bestInd;

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
/*
		unsigned int tmp = 0;
		bool *ns = new bool[m_n];

		float score = 0.;
		for(unsigned int i(0); i < 10; ++i)
		{
			for(unsigned int i(0); i < m_n; ++i) ns[i] = rand()%2;
			bool *opt = m_pop.back()->ls(ns, &tmp);
			score += pb.evaluate(opt);
		}
		m_pop.back()->setfitness(score/10.);

		delete[] ns;

		return tmp/10;*/
	}

	template<class PB>
	bool next(PB& pb, unsigned int visit, unsigned int newSize, unsigned int& tmpnbeval, unsigned int& nbEval, float& propsame, COND c, NEXT n)
	{/*
		unsigned int same = 0;
		bool *s = m_pop.back()->getSol();
		unsigned int i = 0;
		for(; i < visit; ++i)
		{
			((*this).*n)(newSize);

			tmpnbeval = evalPop(pb, s);
			nbEval += tmpnbeval;

			if(!distance(s,m_pop.back()->getSol(),m_n))
				++same;

			if(((*this).*c)())
			{
				propsame = same / (i+1);
				return true;
			}
			else
			{
				delete m_pop.back();
				m_pop.pop_back();
			}
		}
		propsame = (float)same / (float)(i+1.);
		return false;*/

		c = c;
		float all = 1.;
		propsame = 0.;
		bool *s = m_pop.back()->getSol();

		((*this).*n)(newSize);
		tmpnbeval = evalPop(pb, s);
		nbEval += tmpnbeval;

		float score = m_pop.back()->getfitness();
		FnArray *ind = m_pop.back();
		m_pop.pop_back();
		while(!distance(ind->getSol(),s,m_n))
		{
			delete ind;

			((*this).*n)(newSize);
			tmpnbeval = evalPop(pb, s);
			nbEval += tmpnbeval;

			score = m_pop.back()->getfitness();
			ind = m_pop.back();
			m_pop.pop_back();

			++all;
			++propsame;
		}

		for(unsigned int i(0); i < visit; ++i)
		{
			((*this).*n)(newSize);

			tmpnbeval = evalPop(pb, s);
			nbEval += tmpnbeval;

			++all;
			if(!distance(m_pop.back()->getSol(),s,m_n))
				++propsame;

			if(m_pop.back()->getfitness() > score && distance(m_pop.back()->getSol(),s,m_n))
			{
				delete ind;
				ind = m_pop.back();
				score = ind->getfitness();
				m_pop.pop_back();
			}
			else
			{
				delete m_pop.back();
				m_pop.pop_back();
			}
		}
		m_pop.push_back(ind);

		propsame /= all;
		return true;
	}
};

#endif
















