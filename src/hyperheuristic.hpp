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
	void mutate(const std::vector<unsigned int>& fnset);

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
	void mut();
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
	void run(PB& pb, INIT in, unsigned int newSize, unsigned int visit, unsigned int evalmax, const std::string& file)
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
		float best = 0.;
		FnArray *bestInd = nullptr;


		// INIT POP
		((*this).*in)(newSize);
		evalPop(pb, m_s);
		data << "0 " << m_pop.back()->getfitness() << " " << m_pop.back()->size() << " " << pb.getnbeval() << "\n";
		bestInd = new FnArray(*m_pop.back());


		// LS
		for(unsigned int it(1); pb.getnbeval() < evalmax; ++it)
		{
			float same = 0.;

			next(pb, visit, same);

			delete m_pop[0];
			m_pop[0] = m_pop[1];
			m_pop.pop_back();

			if(best <= m_pop.back()->getfitness())
			{
				best = m_pop.back()->getfitness();
				delete bestInd;
				bestInd = new FnArray(*m_pop.back());
				for(unsigned int i(0); i < m_n; ++i) bestSol[i] = m_pop.back()->getSol()[i];
			}
			
			// SAVE
			data << it << " " << m_pop.back()->getfitness() << " " << m_pop.back()->size() << " " << pb.getnbeval() << " " << same << " ";
			data << m_pop.back()->propSat(m_pop.back()->getSol()) << " " << best << std::endl;
		}
		data << std::endl;

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
	void evalPop(PB& pb, bool *s)
	{
		bool *ns = m_pop.back()->ls(s);
		m_pop.back()->setfitness(pb.evaluate(ns));
	}

	template<class PB>
	void next(PB& pb, unsigned int visit, float& propsame)
	{
		float all = 1.;
		propsame = 0.;
		bool *s = m_pop.back()->getSol();

		mut();
		evalPop(pb, s);

		float score = m_pop.back()->getfitness();
		FnArray *ind = m_pop.back();
		m_pop.pop_back();
		while(!distance(ind->getSol(),s,m_n))
		{
			delete ind;

			mut();
			evalPop(pb, s);

			score = m_pop.back()->getfitness();
			ind = m_pop.back();
			m_pop.pop_back();

			++all;
			++propsame;
		}

		for(unsigned int i(0); i < visit; ++i)
		{
			mut();
			evalPop(pb, s);

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
	}
};

#endif
















