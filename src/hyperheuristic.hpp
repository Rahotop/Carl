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
	unsigned int mutate(const std::vector<unsigned int>& fnset);

	float propSat(bool *s) const;
	float evaluate(bool *s);
	float evaluateinc(bool *s, unsigned int changed);
	void acceptCurr();
	bool* ls(bool *s, unsigned int *nbEval = nullptr);

	template<class PB>
	float sw(bool *sol, PB& pb)
	{
		float score = pb.evaluate(sol);

		bool *s = new bool[m_n];
		for(unsigned int i(0); i < m_n; ++i)
			s[i] = sol[i];

		bool *tmpnext = new bool[m_n];
		for(unsigned int i(0); i < m_n; ++i)
		{
			tmpnext[i] = false;
			for(unsigned int j(0); j < m_size; ++j)
			{
				tmpnext[i] = tmpnext[i] || m_in[i*m_maxsize+j];
			}
		}
		std::vector<unsigned int> next;
		for(unsigned int i(0); i < m_n; ++i)
		{
			if(tmpnext[i])
			{
				next.push_back(i);
			}
		}

		bool improved = true;
		while(improved)
		{
			improved = false;
			std::random_shuffle(next.begin(), next.end());

			for(unsigned int i(0); i < next.size(); ++i)
			{
				s[next[i]] = !s[next[i]];
				float tmp = pb.evaluate(s);

				if(tmp > score)
				{
					score = tmp;
					improved = true;
					break;
				}
				else
				{
					s[next[i]] = !s[next[i]];
				}
			}
		}

		delete[] s;

		return score;
	}

	inline void setfitness(float fit) { m_fitness = fit; }
	inline float getfitness() const { return m_fitness; }
	inline bool* getSol() const { return m_s; }
	inline unsigned int size() const { return m_size; }
	inline unsigned int getsteps() const { return m_steps; }
	unsigned int getnbvar() const;

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
	//unsigned int *m_maths;
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
	unsigned int mut();
	private:

	// PARAMS
	std::vector<unsigned int> m_fnset;
	unsigned int m_n;
	bool *m_s;

	// SIZE
	unsigned int m_maxsize; // number max of subtrees
	unsigned int m_width; // size of subtrees

	// POPULATION
	//std::vector<FnArray*> m_pop;
	std::vector<NK*> m_pop;

	public:
/*
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
		float bestsw = 0.;
		FnArray *bestInd = nullptr;
		unsigned int minstep = 0;
		unsigned int maxstep = 0;
		unsigned int avgstep = 0;


		// INIT POP
		((*this).*in)(newSize);
		evalPop(pb, m_s);
		data << "0 " << m_pop.back()->getfitness() << " " << m_pop.back()->size() << " " << pb.getnbeval() << "\n";
		bestInd = new FnArray(*m_pop.back());
		minstep = m_pop.back()->getsteps();
		avgstep += m_pop.back()->getsteps();


		// LS
		unsigned int it(1);
		for(; pb.getnbeval() < evalmax; ++it)
		{
			float same = 0.;

			unsigned int mutation = next(pb, visit, same);
			float floatsw = m_pop.back()->sw(m_pop[0]->getSol(), pb);

			if(m_pop[0]->getfitness() > m_pop[1]->getfitness())
			{
				data << it << " " << m_pop[0]->getfitness() << " " << pb.islocopt(m_pop[0]->getSol()) << " ";

				bool better = false;

				for(unsigned int i(0); i < evalmax && !better; ++i)
				{
					((*this).*in)(newSize);
					evalPop(pb, m_pop[0]->getSol());

					if(m_pop[0]->getfitness() < m_pop[2]->getfitness())
						better = true;

					delete m_pop[2];
					m_pop.pop_back();
				}

				data << better << " " << m_pop[1]->getsteps() << std::endl;
			}

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

			if(bestsw < floatsw)
				bestsw = floatsw;
			
			// SAVE
			
			//data << it << " " << m_pop.back()->getfitness() << " " << m_pop.back()->size() << " " << pb.getnbeval() << " " << same << " ";
			//data << m_pop.back()->propSat(m_pop.back()->getSol()) << " " << best << " " << mutation << " " << m_pop.back()->getnbvar();
			//data << " " << floatsw << " " << bestsw << " " << m_pop.back()->getsteps() << std::endl;

			minstep = (minstep > m_pop.back()->getsteps()) ? m_pop.back()->getsteps() : minstep;
			maxstep = (maxstep < m_pop.back()->getsteps()) ? m_pop.back()->getsteps() : maxstep;
			avgstep += m_pop.back()->getsteps();
		}
		data << std::endl;

		res << "nb eval (f obj) : " << pb.getnbeval() << std::endl;
		res << "fitness max : " << best << std::endl;
		res << "minimal step : " << minstep << std::endl;
		res << "maximal step : " << maxstep << std::endl;
		res << "average step : " << (double)avgstep/(double)it << std::endl;

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
	unsigned int next(PB& pb, unsigned int visit, float& propsame)
	{
		float all = 1.;
		propsame = 0.;
		bool *s = m_pop.back()->getSol();

		unsigned int mutation = mut();
		evalPop(pb, s);

		float score = m_pop.back()->getfitness();
		FnArray *ind = m_pop.back();
		m_pop.pop_back();
		while(!distance(ind->getSol(),s,m_n))
		{
			delete ind;

			mutation = mut();
			evalPop(pb, s);

			score = m_pop.back()->getfitness();
			ind = m_pop.back();
			m_pop.pop_back();

			++all;
			++propsame;
		}

		FnArray *ind2 = ind;

		for(unsigned int i(0); i < visit; ++i)
		{
			unsigned int tmpmutation = mut();

			++all;
			if(!distance(m_pop.back()->getSol(),s,m_n))
				++propsame;

			for(unsigned int j(0); !distance(m_pop.back()->getSol(),s,m_n) && j < 10; ++j)
			{
				tmpmutation = mut();
				delete m_pop[m_pop.size()-2];
				m_pop[m_pop.size()-2] = m_pop.back();
				m_pop.pop_back();
			}

			evalPop(pb, s);

			if(m_pop.back()->getfitness() > score && distance(m_pop.back()->getSol(),s,m_n))
			{
				if(ind != ind2)
					delete ind;
				ind = m_pop.back();
				score = ind->getfitness();
				m_pop.pop_back();
				mutation = tmpmutation;
				//
				break;
			}
			else if((ind2->getsteps() < m_pop.back()->getsteps()) || (ind2->getsteps() == m_pop.back()->getsteps() && ind2->getfitness() < m_pop.back()->getfitness()))
			{
				if(ind != ind2)
					delete ind2;
				ind2 = m_pop.back();
				m_pop.pop_back();
			}
			else
			{
				delete m_pop.back();
				m_pop.pop_back();
			}
		}
		if(score > m_pop.back()->getfitness())
		{
			m_pop.push_back(ind);
			if(ind != ind2)
				delete ind2;
		}
		else
		{
			m_pop.push_back(ind2);
			if(ind != ind2)
				delete ind;
		}

		propsame /= all;

		return mutation;
	}*/
/*
	template<class PB>
	unsigned int next(PB& pb, unsigned int visit, float& propsame)
	{
		float all = 1.;
		propsame = 0.;
		bool *s = m_pop.back()->getSol();
		float score = m_pop.back()->getfitness();

		unsigned int mutation = mut();
		evalPop(pb, s);

		float score2 = m_pop.back()->getfitness();
		FnArray *ind = m_pop.back();
		m_pop.pop_back();
		while((!distance(ind->getSol(),s,m_n)))
		{
			delete ind;

			mutation = mut();
			evalPop(pb, s);

			score2 = m_pop.back()->getfitness();
			ind = m_pop.back();
			m_pop.pop_back();

			++all;
			++propsame;
		}

		unsigned int dist = distance(ind->getSol(),s,m_n);

		for(unsigned int i(0); i < visit; ++i)
		{
			unsigned int tmpmutation = mut();
			evalPop(pb, s);
			
			for(unsigned int j(0); !distance(m_pop.back()->getSol(),s,m_n) && j < 10; ++j)
			{
				tmpmutation = mut();
				delete m_pop[m_pop.size()-2];
				m_pop[m_pop.size()-2] = m_pop.back();
				m_pop.pop_back();
				evalPop(pb, s);
			}

			++all;
			if(!distance(m_pop.back()->getSol(),s,m_n))
				++propsame;

			if(((m_pop.back()->getfitness() > score) && (distance(m_pop.back()->getSol(),s,m_n) > dist)) || ((score2 < m_pop.back()->getfitness()) && (distance(m_pop.back()->getSol(),s,m_n) == dist)))
			{
				score2 = m_pop.back()->getfitness();
				dist = distance(m_pop.back()->getSol(),s,m_n);
				delete ind;
				ind = m_pop.back();
				m_pop.pop_back();
				mutation = tmpmutation;
			}
			else
			{
				delete m_pop.back();
				m_pop.pop_back();
			}
		}
		m_pop.push_back(ind);

		propsame /= all;

		return mutation;
	}*/
/*
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
		NK *bestInd = nullptr;
		unsigned int minstep = 0;
		unsigned int maxstep = 0;
		unsigned int avgstep = 0;


		// INIT POP
		m_pop.push_back(new NK(m_n,1));
		evalPop(pb, m_s);
		data << "0 " << m_pop.back()->getfitness() << " " << m_pop.back()->size() << " " << pb.getnbeval() << "\n";
		bestInd = new NK(*m_pop.back());
		minstep = m_pop.back()->getsteps();
		avgstep += m_pop.back()->getsteps();


		// LS
		unsigned int it(1);
		for(; pb.getnbeval() < evalmax; ++it)
		{
			float same = 0.;

			unsigned int mutation = next(pb, visit, same);

			if(m_pop[0]->getfitness() > m_pop[1]->getfitness())
			{
				data << it << " " << m_pop[0]->getfitness() << " " << pb.getnbeval() << " " << pb.islocopt(m_pop[0]->getSol()) << " ";

				bool better = false;

				for(unsigned int i(0); i < visit && !better; ++i)
				{
					m_pop.push_back(new NK(m_n,1));
					evalPop(pb, m_pop[0]->getSol());

					if(m_pop[0]->getfitness() < m_pop[2]->getfitness())
						better = true;

					delete m_pop[2];
					m_pop.pop_back();
				}

				data << better << " " << m_pop[1]->getsteps() << std::endl;
			}

			delete m_pop[0];
			m_pop[0] = m_pop[1];
			m_pop.pop_back();

			if(best <= m_pop.back()->getfitness())
			{
				best = m_pop.back()->getfitness();
				delete bestInd;
				bestInd = new NK(*m_pop.back());
				for(unsigned int i(0); i < m_n; ++i) bestSol[i] = m_pop.back()->getSol()[i];
			}

			res << it << " " << m_pop[0]->getfitness() << " " << pb.getnbeval() << " " << m_pop[0]->getsteps() << std::endl;
			
			// SAVE
			
			//data << it << " " << m_pop.back()->getfitness() << " " << m_pop.back()->size() << " " << pb.getnbeval() << " " << same << " ";
			//data << m_pop.back()->propSat(m_pop.back()->getSol()) << " " << best << " " << mutation << " " << m_pop.back()->getnbvar();
			//data << " " << floatsw << " " << bestsw << " " << m_pop.back()->getsteps() << std::endl;

			minstep = (minstep > m_pop.back()->getsteps()) ? m_pop.back()->getsteps() : minstep;
			maxstep = (maxstep < m_pop.back()->getsteps()) ? m_pop.back()->getsteps() : maxstep;
			avgstep += m_pop.back()->getsteps();
		}
		data << std::endl;

		res << "nb eval (f obj) : " << pb.getnbeval() << std::endl;
		res << "fitness max : " << best << std::endl;
		res << "minimal step : " << minstep << std::endl;
		res << "maximal step : " << maxstep << std::endl;
		res << "average step : " << (double)avgstep/(double)it << std::endl;

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

		delete bestInd;
	}

	template<class PB>
	void evalPop(PB& pb, bool *s)
	{
		bool *ns = m_pop.back()->ls(s);
		m_pop.back()->setfitness(pb.evaluate(ns));
	}

	template<class PB>
	unsigned int next(PB& pb, unsigned int visit, float& propsame)
	{
		float all = 1.;
		propsame = 0.;
		bool *s = m_pop.back()->getSol();
		float score2 = m_pop.back()->getfitness();

		unsigned int mutation = 0;
		m_pop.push_back(new NK(*m_pop.back()));
		m_pop.back()->mutate(1,1);
		evalPop(pb, s);

		float score = m_pop.back()->getfitness();
		NK *ind = m_pop.back();
		m_pop.pop_back();
		while(!distance(ind->getSol(),s,m_n))
		{
			delete ind;

			m_pop.push_back(new NK(*m_pop.back()));
			m_pop.back()->mutate(1,1);
			evalPop(pb, s);

			score = m_pop.back()->getfitness();
			ind = m_pop.back();
			m_pop.pop_back();

			++all;
			++propsame;
		}

		NK *ind2 = ind;

		for(unsigned int i(0); i < visit; ++i)
		{
			m_pop.push_back(new NK(*m_pop.back()));
			m_pop.back()->mutate(1,1);

			++all;
			if(!distance(m_pop.back()->getSol(),s,m_n))
				++propsame;

			for(unsigned int j(0); !distance(m_pop.back()->getSol(),s,m_n) && j < 10; ++j)
			{
				m_pop.push_back(new NK(*m_pop.back()));
				m_pop.back()->mutate(1,1);
				delete m_pop[m_pop.size()-2];
				m_pop[m_pop.size()-2] = m_pop.back();
				m_pop.pop_back();
			}

			evalPop(pb, s);

			if(m_pop.back()->getfitness() > score2 && distance(m_pop.back()->getSol(),s,m_n))
			{
				if(ind != ind2)
					delete ind;
				ind = m_pop.back();
				score = ind->getfitness();
				m_pop.pop_back();
				//
				break;
			}
			else if((ind2->getsteps() < m_pop.back()->getsteps()) || (ind2->getsteps() == m_pop.back()->getsteps() && ind2->getfitness() < m_pop.back()->getfitness()))
			{
				if(ind != ind2)
					delete ind2;
				ind2 = m_pop.back();
				m_pop.pop_back();
			}
			else
			{
				delete m_pop.back();
				m_pop.pop_back();
			}
		}
		if(score > m_pop.back()->getfitness())
		{
			m_pop.push_back(ind);
			if(ind != ind2)
				delete ind2;
		}
		else
		{
			m_pop.push_back(ind2);
			if(ind != ind2)
				delete ind;
		}

		propsame /= all;

		return mutation;
	}*/

	template<class PB>
	void run(PB& pb, INIT in, unsigned int newSize, unsigned int visit, unsigned int evalmax, const std::string& file)
	{
		pb =pb;
		in = in;
		newSize = newSize;
		visit = visit;
		evalmax = evalmax;
		file.size();
	}

	//template<class PB>
	void run(NK& pb, INIT in, unsigned int newSize, unsigned int visit, unsigned int evalmax, const std::string& file)
	{
		in = in;
		newSize = newSize;
		evalmax = evalmax;
		// PARAM
		m_n = pb.getN();
		m_s = new bool[m_n];
		for(unsigned int i(0); i < m_n; ++i) m_s[i] = rand()%2;


		// OUTPUT
		std::ofstream data(file);
		std::ofstream res(file+".txt");
		std::vector<unsigned int> accepted;
		std::vector<unsigned int> proposed;


		// INIT POP
		NK *curr = new NK(m_n,1);
		curr->showvar();
		//curr->copylinks(pb);
		//curr->copymat(pb);
		curr->ls(m_s);
		curr->setfitness(pb.evaluate(curr->getSol()));

		bool better = true;
		for(unsigned int it(0); better; ++it)
		{
			better = false;
			NK *voisin = nullptr;
			for(unsigned int i(0); i < pb.getN(); ++i)
			{
				voisin = new NK(*curr);
				voisin->mutate(evalmax,newSize,curr->getSol(),visit);
				voisin->ls(curr->getSol());
				voisin->setfitness(pb.evaluate(voisin->getSol()));

				while(voisin->getsteps() >= proposed.size())
				{
					proposed.push_back(0);
					accepted.push_back(0);
				}
				++proposed[voisin->getsteps()];

				if(voisin->getfitness() > curr->getfitness())
				{
					++accepted[voisin->getsteps()];
					better = true;
					break;
				}
				else
				{
					delete voisin;
				}
			}

			if(better)
			{
				delete curr;
				curr = voisin;
				data << it << " " << pb.getnbeval() << " " << curr->getfitness() << " " << pb.islocopt(curr->getSol()) << " " << curr->getsteps() << std::endl;
			}
		}

		res << curr->getfitness() << std::endl << std::endl;
		unsigned int sum = 0;
		for(unsigned int i(0); i < accepted.size(); ++i) sum += accepted[i];
		for(unsigned int i(0); i < accepted.size(); ++i)
		{
			res << (double)accepted[i]/(double)sum << " ";
		}
		res << std::endl;
		sum = 0;
		for(unsigned int i(0); i < proposed.size(); ++i) sum += proposed[i];
		for(unsigned int i(0); i < proposed.size(); ++i)
		{
			res << (double)proposed[i]/(double)sum << " ";
		}

		curr->showvar();
		delete curr;
	}
/*
	template<class PB>
	void run(PB& pb, INIT in, unsigned int newSize, unsigned int visit, unsigned int evalmax, const std::string& file)
	{
		in = in;
		newSize = newSize;
		evalmax = evalmax;
		// PARAM
		m_n = pb.getN();
		m_s = new bool[m_n];
		for(unsigned int i(0); i < m_n; ++i) m_s[i] = rand()%2;


		// OUTPUT
		std::ofstream data(file);
		std::vector<unsigned int> fnset = {1,7,9,11,13};


		// INIT POP
		FnArray *curr = new FnArray(newSize,15,m_n);
		for(unsigned int i(0); i < newSize; ++i)
			curr->addRandom(fnset);
		curr->ls(m_s);
		curr->setfitness(pb.evaluate(curr->getSol()));

		bool better = true;
		for(unsigned int it(0); better; ++it)
		{
			better = false;
			FnArray *voisin = nullptr;
			for(unsigned int i(0); i < visit; ++i)
			{
				voisin = new FnArray(*curr);
				voisin->mutate(fnset);
				voisin->ls(curr->getSol());
				voisin->setfitness(pb.evaluate(voisin->getSol()));
				if(voisin->getfitness() > curr->getfitness())
				{
					better = true;
					break;
				}
				else
				{
					delete voisin;
				}
			}

			if(better)
			{
				delete curr;
				curr = voisin;
				data << it << " " << pb.getnbeval() << " " << curr->getfitness() << " " << pb.islocopt(curr->getSol()) << " " << curr->getsteps() << std::endl;
			}
		}
		delete curr;
	}*/
};

#endif
















